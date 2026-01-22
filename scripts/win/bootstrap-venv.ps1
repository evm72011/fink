$ErrorActionPreference = "Stop"

$Root     = Resolve-Path "$PSScriptRoot\..\.."
$VenvDir  = Join-Path $Root ".venv\win"
$Python   = Join-Path $VenvDir "Scripts\python.exe"
$ConanExe = Join-Path $VenvDir "Scripts\conan.exe"
$CMakeFormatExe = Join-Path $VenvDir "Scripts\cmake-format.exe"

# 1) recreate venv
if (Test-Path $VenvDir) {
  Remove-Item -Recurse -Force $VenvDir
}
py -3 -m venv $VenvDir

# 2) install conan
& $Python -m pip install -U pip
& $Python -m pip install -U conan cmake-format

# 3) print installed versions
Write-Host (& $ConanExe --version 2>&1)
Write-Host (& $CMakeFormatExe --version 2>&1)
