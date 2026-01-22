$ErrorActionPreference = "Stop"

$Root = Resolve-Path "$PSScriptRoot\..\.."
$Conan = Join-Path $Root ".venv\win\Scripts\conan.exe"
$BuildDir = Join-Path $Root "build\win"

if (!(Test-Path $Conan)) { throw "Conan not found in venv: $Conan" }

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

& $Conan install "$Root" `
  -of "$BuildDir" `
  -s build_type=Debug `
  -b missing
