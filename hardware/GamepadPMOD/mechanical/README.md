# Dual straight-SNES connector gap filler

`dual_snes_connector_gap_filler.stl` is one consolidated two-piece set that
supports **both** straight SNES connectors on one Gamepad PMOD.

## Measured fit

The dimensions come from `SNES_Connector_Straight_Master_Assy.stp`, which
matches the board's seven-pin 4/4/4/6.5/4/4 mm pitch:

| Feature | STEP Z | Result |
|---------|-------:|-------:|
| PCB seating datum / pedestal bottom | -14.60 mm | — |
| Broad housing underside | -11.40 mm | — |
| Nominal housing-to-PCB gap | — | **3.20 mm** |
| Filler with vertical clearance | — | **3.10 mm** |

The matching
[vertical KiCad footprint](https://github.com/shuki25/kicad-custom-library/blob/master/footprints/SNES_Controller_Port_Vertical.kicad_mod)
applies a +14.60 mm Z offset to the STEP model. This places the pedestal
bottom at the PCB top surface and confirms the mounting datum used above.

The earlier 0.50 mm estimate from a different right-angle connector model does
not apply to this straight connector.

## Design and installation

Each connector's central semistadium-shaped pedestal reaches the PCB, leaving
a 3.20 mm-high ring-shaped cavity under the wider housing. The filler reproduces
the STEP model's exact outer and pedestal profiles with 0.15 mm horizontal
clearance. Matching halves for J1 and J2 are joined across the 4.50 mm space
between their housings by a bridge spanning the complete **32.25 mm straight
section** of the support profile. The result is split along the connector
pin-row direction into front and rear consolidated pieces. The two bridge
sections meet at the split without overlap, retaining post-assembly
installation while giving each half the widest possible load path.

Print flat without supports. Insert one half from each long side of the
connector until its inner profile seats around the central pedestal. A small
spot of flexible adhesive can retain a loose fit; keep adhesive away from
solder joints. Confirm the 3.20 mm gap on a production connector before batch
printing because STEP models do not define manufacturing tolerance.

## Files

- `dual_snes_connector_gap_filler.FCStd` — consolidated FreeCAD model
- `dual_snes_connector_gap_filler.step` — two-piece dual-connector solid
- `dual_snes_connector_gap_filler.stl` — one complete set per PCB
- `dual_snes_connector_gap_filler_kicad.step` — J1-local KiCad model
- `dual_snes_connector_gap_filler.py` — FreeCAD generator
- `GamepadPMOD_board.step` — PCB exported from the Elecrow branch
- `GamepadPMOD_dual_connector_assembly.FCStd` — visual fit assembly
- `GamepadPMOD_dual_connector_assembly.step` — portable assembly
- `GamepadPMOD_dual_connector_assembly.png` — top verification render
- `GamepadPMOD_dual_connector_assembly_side.png` — gap/profile render
- `render_assembly.ps1` — regenerate all models and renders

The design dimensions are collected at the top of the generator and recorded
in the `.FCStd` `Parameters` spreadsheet. Edit the generator and run:

```powershell
& 'C:\Program Files\FreeCAD 1.0\bin\freecadcmd.exe' dual_snes_connector_gap_filler.py
```

The visual assembly uses the Elecrow branch placements J1 `(22.50, 61.37,
-90°)` and J2 `(39.00, 61.37, -90°)`, the supplied connector STEP, and the
KiCad-exported board STEP. The board also references the connector STEP on J1
and J2 and the J1-local dual filler STEP, so KiCad's native 3D renderer checks
the same placements used by fabrication:

```powershell
.\render_assembly.ps1
```

The script exports the fully populated KiCad STEP and verifies that the STEP
pin centres exactly match all 14 PCB drills:

| Connector | Pin-centre X | Pin-centre Y |
|-----------|-------------:|--------------|
| J1 | 22.50 mm | 61.37, 65.37, 69.37, 73.37, 79.87, 83.87, 87.87 mm |
| J2 | 39.00 mm | 61.37, 65.37, 69.37, 73.37, 79.87, 83.87, 87.87 mm |

It also rejects filler-to-PCB or filler-to-connector intersections.
