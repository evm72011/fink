#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
VENV_DIR="$ROOT/.venv/wsl"
PYTHON="$VENV_DIR/bin/python"
CONAN="$VENV_DIR/bin/conan"
CMAKE_FORMAT="$VENV_DIR/bin/cmake-format"

echo "Removing venv (if exists)..."
rm -rf "$VENV_DIR"

echo "Creating venv..."
python3 -m venv "$VENV_DIR"

echo "Installing pip..."
"$PYTHON" -m pip install -U pip

echo "Installing conan..."
"$PYTHON" -m pip install -U conan cmake-format

echo "Conan version:"
"$CONAN" --version

echo "cmake-format version:"
"$CMAKE_FORMAT" --version
