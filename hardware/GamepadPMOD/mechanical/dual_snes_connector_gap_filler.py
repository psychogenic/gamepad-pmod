"""Generate and assemble a dual-SNES-connector PCB gap filler.

Run from FreeCAD:
    freecadcmd dual_snes_connector_gap_filler.py
"""

from pathlib import Path

import FreeCAD as App
import Mesh
import Part


# STEP-derived dimensions in millimetres.
GAP_HEIGHT = 3.20
FIT_CLEARANCE_Z = 0.10
FIT_CLEARANCE_XY = 0.15
OUTER_X_MIN = -19.375
OUTER_ARC_CENTRE_X = 13.025
OUTER_RADIUS = 6.15
PEDESTAL_X_MIN = -15.985
PEDESTAL_RADIUS = 2.76
PROFILE_SPLIT_X = -0.10

# Elecrow assembly branch placement.
PCB_TOP_Z = 1.60
MODEL_Z_OFFSET = 14.60
PIN1_LOCAL_X = -13.615
J1_X = 22.50
J2_X = 39.00
CONNECTOR_PIN1_BOARD_Y = -61.37


def semistadium(x_min, arc_centre_x, radius):
    rectangle = Part.makeBox(
        arc_centre_x - x_min,
        2 * radius,
        GAP_HEIGHT - FIT_CLEARANCE_Z,
        App.Vector(x_min, -radius, 0),
    )
    round_end = Part.makeCylinder(
        radius,
        GAP_HEIGHT - FIT_CLEARANCE_Z,
        App.Vector(arc_centre_x, 0, 0),
    )
    return rectangle.fuse(round_end)


def local_support_ring():
    outer = semistadium(
        OUTER_X_MIN + FIT_CLEARANCE_XY,
        OUTER_ARC_CENTRE_X,
        OUTER_RADIUS - FIT_CLEARANCE_XY,
    )
    pedestal = semistadium(
        PEDESTAL_X_MIN - FIT_CLEARANCE_XY,
        OUTER_ARC_CENTRE_X,
        PEDESTAL_RADIUS + FIT_CLEARANCE_XY,
    )
    return outer.cut(pedestal)


def split_ring():
    ring = local_support_ring()
    bounds = ring.BoundBox
    left_clip = Part.makeBox(
        PROFILE_SPLIT_X - bounds.XMin,
        bounds.YLength,
        bounds.ZLength,
        App.Vector(bounds.XMin, bounds.YMin, 0),
    )
    right_clip = Part.makeBox(
        bounds.XMax - PROFILE_SPLIT_X,
        bounds.YLength,
        bounds.ZLength,
        App.Vector(PROFILE_SPLIT_X, bounds.YMin, 0),
    )
    return ring.common(left_clip), ring.common(right_clip)


def place_at_connector(shape, connector_x):
    placed = shape.copy()
    placed.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 90)
    model_origin_y = CONNECTOR_PIN1_BOARD_Y - PIN1_LOCAL_X
    placed.translate(App.Vector(connector_x, model_origin_y, PCB_TOP_Z))
    return placed


def consolidated_half(local_half, near_split_x, far_split_x):
    j1 = place_at_connector(local_half, J1_X)
    j2 = place_at_connector(local_half, J2_X)

    # Join the facing outside bands of both connector rings near the split.
    model_origin_y = CONNECTOR_PIN1_BOARD_Y - PIN1_LOCAL_X
    bridge_y_min = model_origin_y + min(near_split_x, far_split_x)
    bridge = Part.makeBox(
        J2_X - J1_X - 2 * (OUTER_RADIUS - FIT_CLEARANCE_XY),
        abs(far_split_x - near_split_x),
        GAP_HEIGHT - FIT_CLEARANCE_Z,
        App.Vector(
            J1_X + OUTER_RADIUS - FIT_CLEARANCE_XY,
            bridge_y_min,
            PCB_TOP_Z,
        ),
    )
    return j1.fuse(bridge).fuse(j2)


def make_filler():
    left, right = split_ring()
    left_half = consolidated_half(
        left, OUTER_X_MIN + FIT_CLEARANCE_XY, PROFILE_SPLIT_X
    )
    right_half = consolidated_half(
        right, PROFILE_SPLIT_X, OUTER_ARC_CENTRE_X
    )
    return left_half, right_half


def make_kicad_filler():
    left, right = split_ring()
    pitch = J2_X - J1_X
    halves = []
    for half, x_min, x_max in (
        (left, OUTER_X_MIN + FIT_CLEARANCE_XY, PROFILE_SPLIT_X),
        (right, PROFILE_SPLIT_X, OUTER_ARC_CENTRE_X),
    ):
        second = half.copy()
        second.translate(App.Vector(0, pitch, 0))
        bridge = Part.makeBox(
            x_max - x_min,
            pitch - 2 * (OUTER_RADIUS - FIT_CLEARANCE_XY),
            GAP_HEIGHT - FIT_CLEARANCE_Z,
            App.Vector(
                x_min,
                OUTER_RADIUS - FIT_CLEARANCE_XY,
                0,
            ),
        )
        halves.append(half.fuse(bridge).fuse(second))
    return halves


def add_parameters(document):
    sheet = document.addObject("Spreadsheet::Sheet", "Parameters")
    sheet.Label = "Regenerate after changing script parameters"
    rows = (
        ("Housing-to-PCB gap", GAP_HEIGHT),
        ("Vertical fit clearance", FIT_CLEARANCE_Z),
        ("Profile fit clearance", FIT_CLEARANCE_XY),
        ("Connector centre spacing", J2_X - J1_X),
        ("PCB top Z", PCB_TOP_Z),
        ("Connector model Z offset", MODEL_Z_OFFSET),
        (
            "Full straight bridge length",
            OUTER_ARC_CENTRE_X - OUTER_X_MIN - FIT_CLEARANCE_XY,
        ),
    )
    sheet.set("A1", "Parameter")
    sheet.set("B1", "Value")
    for row, (label, value) in enumerate(rows, start=2):
        sheet.set(f"A{row}", label)
        sheet.set(f"B{row}", f"{value} mm")
    sheet.setColumnWidth("A", 190)
    sheet.setColumnWidth("B", 100)


def add_feature(document, name, label, shape, color):
    feature = document.addObject("PartDesign::Feature", name)
    feature.Label = label
    feature.Shape = shape
    if feature.ViewObject:
        feature.ViewObject.ShapeColor = color
    return feature


def connector_placement(connector_x):
    model_origin_y = CONNECTOR_PIN1_BOARD_Y - PIN1_LOCAL_X
    return App.Placement(
        App.Vector(connector_x, model_origin_y, PCB_TOP_Z + MODEL_Z_OFFSET),
        App.Rotation(App.Vector(0, 0, 1), 90),
    )


def main():
    output_dir = Path(__file__).resolve().parent
    left_shape, right_shape = make_filler()

    filler_doc = App.newDocument("DualSNESConnectorGapFiller")
    add_parameters(filler_doc)
    left = add_feature(
        filler_doc, "RearHalf", "Rear dual-connector half", left_shape, (0.95, 0.55, 0.10)
    )
    right = add_feature(
        filler_doc,
        "FrontHalf",
        "Front dual-connector half",
        right_shape,
        (1.00, 0.70, 0.15),
    )
    assembly = filler_doc.addObject("App::Part", "DualFillerAssembly")
    assembly.Label = "Consolidated dual-connector filler"
    assembly.addObject(left)
    assembly.addObject(right)
    filler_doc.recompute()

    filler_doc.saveAs(str(output_dir / "dual_snes_connector_gap_filler.FCStd"))
    Part.export(
        [left, right], str(output_dir / "dual_snes_connector_gap_filler.step")
    )
    Mesh.export(
        [left, right], str(output_dir / "dual_snes_connector_gap_filler.stl")
    )

    # KiCad's file coordinates use the footprint's unrotated local frame.
    # J2 is +16.5 mm along J1's local Y axis after the -90 degree placement.
    kicad_left, kicad_right = make_kicad_filler()
    kicad_left_feature = add_feature(
        filler_doc,
        "KiCadRearHalf",
        "KiCad-local rear half",
        kicad_left,
        (0.95, 0.40, 0.05),
    )
    kicad_right_feature = add_feature(
        filler_doc,
        "KiCadFrontHalf",
        "KiCad-local front half",
        kicad_right,
        (1.00, 0.65, 0.10),
    )
    Part.export(
        [kicad_left_feature, kicad_right_feature],
        str(output_dir / "dual_snes_connector_gap_filler_kicad.step"),
    )

    assembly_doc = App.newDocument("GamepadPMODMechanicalAssembly")
    board_shape = Part.read(str(output_dir / "GamepadPMOD_board.step"))
    board = add_feature(
        assembly_doc, "PCB", "Elecrow Gamepad PMOD PCB", board_shape, (0.10, 0.45, 0.18)
    )

    connector_shape = Part.read(
        str(output_dir / "SNES_Connector_Straight_Master_Assy.stp")
    )
    for name, connector_x in (("J1", J1_X), ("J2", J2_X)):
        connector = add_feature(
            assembly_doc,
            name,
            f"{name} straight SNES connector",
            connector_shape,
            (0.45, 0.47, 0.50),
        )
        connector.Placement = connector_placement(connector_x)

    assembly_left = add_feature(
        assembly_doc,
        "FillerRearHalf",
        "Rear consolidated filler half",
        left_shape,
        (0.95, 0.40, 0.05),
    )
    assembly_right = add_feature(
        assembly_doc,
        "FillerFrontHalf",
        "Front consolidated filler half",
        right_shape,
        (1.00, 0.65, 0.10),
    )
    assembly_doc.recompute()
    assembly_doc.saveAs(str(output_dir / "GamepadPMOD_dual_connector_assembly.FCStd"))
    Part.export(
        [board, assembly_left, assembly_right, assembly_doc.J1, assembly_doc.J2],
        str(output_dir / "GamepadPMOD_dual_connector_assembly.step"),
    )

    print(
        "Generated consolidated filler and assembly: "
        f"{len(left_shape.Solids) + len(right_shape.Solids)} solids, "
        f"{GAP_HEIGHT - FIT_CLEARANCE_Z:.2f} mm filler thickness"
    )


main()
