#!/usr/bin/env bash
set -euo pipefail

ROOT="${1:-.}"
CFG="${2:-.cmake-format.yaml}"

# Resolve cmake-format executable:
# 1) from venv via VENV_DIR (preferred)
# 2) fallback to PATH
CMAKE_FORMAT=""

if [[ -n "${VENV_DIR:-}" ]]; then
  if [[ -x "${VENV_DIR}/bin/cmake-format" ]]; then
    CMAKE_FORMAT="${VENV_DIR}/bin/cmake-format"
  elif [[ -x "${VENV_DIR}/Scripts/cmake-format.exe" ]]; then
    CMAKE_FORMAT="${VENV_DIR}/Scripts/cmake-format.exe"
  fi
fi

if [[ -z "$CMAKE_FORMAT" ]]; then
  if command -v cmake-format >/dev/null 2>&1; then
    CMAKE_FORMAT="cmake-format"
  else
    echo "cmake-format not found (set VENV_DIR to your venv)" >&2
    exit 1
  fi
fi

if [[ ! -f "$CFG" ]]; then
  echo "Config file not found: $CFG" >&2
  exit 1
fi

cd "$ROOT"

mapfile -t FILES < <(
  find . \
    -type d \( \
      -name build -o \
      -name .venv -o \
      -name external -o -name _deps -o \
      -name cmake-build-\* -o \
      -name .git \
    \) -prune -o \
    -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \) -print
)

TOTAL="${#FILES[@]}"
if [[ "$TOTAL" -eq 0 ]]; then
  echo "No CMake files found."
  exit 0
fi

echo "Formatting $TOTAL file(s) using config: $CFG"

i=0
for f in "${FILES[@]}"; do
  i=$((i+1))
  echo "[$i/$TOTAL] $f"
  "$CMAKE_FORMAT" -c "$CFG" -i "$f"
done

echo "Done."
