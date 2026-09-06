param(
    [string]$FreeCADCmd = 'C:\Program Files\FreeCAD 1.0\bin\freecadcmd.exe',
    [string]$KiCadCli = 'C:\Program Files\KiCad\10.0\bin\kicad-cli.exe'
)

$ErrorActionPreference = 'Continue'
$MechanicalDir = $PSScriptRoot
$ProjectDir = Split-Path $MechanicalDir
$Board = Join-Path $ProjectDir 'GamepadPMOD.kicad_pcb'

& $KiCadCli pcb export step --force --board-only --include-pads `
    --include-soldermask --include-silkscreen `
    -o (Join-Path $MechanicalDir 'GamepadPMOD_board.step') $Board
if ($LASTEXITCODE -ne 0) {
    throw "KiCad STEP export failed with exit code $LASTEXITCODE"
}

& $FreeCADCmd (Join-Path $MechanicalDir 'dual_snes_connector_gap_filler.py')
if ($LASTEXITCODE -ne 0) {
    throw "FreeCAD generation failed with exit code $LASTEXITCODE"
}

& $KiCadCli pcb render `
    --output (Join-Path $MechanicalDir 'GamepadPMOD_dual_connector_assembly.png') `
    --width 1920 --height 1080 --side top --background opaque `
    --quality high --floor --perspective --zoom 1.15 --rotate '-35,0,-25' `
    $Board
if ($LASTEXITCODE -ne 0) {
    throw "KiCad top render failed with exit code $LASTEXITCODE"
}

& $KiCadCli pcb render `
    --output (Join-Path $MechanicalDir 'GamepadPMOD_dual_connector_assembly_side.png') `
    --width 1920 --height 1080 --side right --background opaque `
    --quality high --floor --perspective --zoom 1.3 --rotate '0,0,0' `
    $Board
if ($LASTEXITCODE -ne 0) {
    throw "KiCad side render failed with exit code $LASTEXITCODE"
}
