"""Verify KiCad's populated STEP placement and FreeCAD interference."""

from pathlib import Path
import tempfile

import FreeCAD as App
import Part


output_dir = Path(__file__).resolve().parent
populated_step = Path(tempfile.gettempdir()) / "GamepadPMOD_populated.step"
shape = Part.read(str(populated_step))

pin_centres = []
for solid in shape.Solids:
    bounds = solid.BoundBox
    if (
        abs(bounds.XLength - 1.20) < 0.02
        and abs(bounds.YLength - 1.20) < 0.02
        and bounds.ZLength > 15
    ):
        pin_centres.append(
            (
                round((bounds.XMin + bounds.XMax) / 2, 3),
                round((bounds.YMin + bounds.YMax) / 2, 3),
            )
        )

pin_offsets = (0, 4, 8, 12, 18.5, 22.5, 26.5)
expected = sorted(
    (connector_x, round(-61.37 - offset, 3))
    for connector_x in (22.5, 39.0)
    for offset in pin_offsets
)
actual = sorted(pin_centres)
if actual != expected:
    raise RuntimeError(
        f"Connector pins do not match PCB drills.\nExpected: {expected}\nActual: {actual}"
    )

document = App.openDocument(
    str(output_dir / "GamepadPMOD_dual_connector_assembly.FCStd")
)
document.recompute()
fillers = (document.FillerRearHalf.Shape, document.FillerFrontHalf.Shape)
connectors = (document.J1.Shape, document.J2.Shape)

if any(filler.common(document.PCB.Shape).Volume > 1e-6 for filler in fillers):
    raise RuntimeError("Filler intersects the PCB")
if any(
    filler.common(connector).Volume > 1e-6
    for filler in fillers
    for connector in connectors
):
    raise RuntimeError("Filler intersects a connector")
if any(len(filler.Solids) != 1 or not filler.isValid() for filler in fillers):
    raise RuntimeError("A consolidated filler half is not one valid solid")

print("Verified 14 connector pins on PCB drills and zero filler interference")
