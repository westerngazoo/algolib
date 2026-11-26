#!/usr/bin/env bash
# (Instructor) Antes de empaquetar una entrega: agrega al manifiesto las huellas de los
# esqueletos de labs tal como se entregan. El manifiesto acumula TODAS las versiones
# entregadas, así actualizar.sh reconoce cualquier esqueleto que el alumno no editó.
set -euo pipefail
cd "$(dirname "$0")/.."
M=herramientas/originales.sha256
touch "$M"
huella() { if command -v sha256sum >/dev/null 2>&1; then sha256sum "$1" | cut -d' ' -f1; else shasum -a 256 "$1" | cut -d' ' -f1; fi; }
{ find cpp/include/algolib -path '*/lab*/*' -type f; find rust/src -type f; find c/src -type f; } | sort | while read -r f; do
  echo "$(huella "$f")  $f"
done | cat "$M" - | sort -u > "$M.tmp" && mv "$M.tmp" "$M"
echo "manifiesto: $(wc -l < "$M") huellas"
