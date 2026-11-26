#!/usr/bin/env bash
# Actualiza tu copia de algolib con un zip nuevo del curso SIN tocar tu trabajo.
#
#   make actualizar ZIP=~/Downloads/algolib.zip
#   (o directamente)  herramientas/actualizar.sh ~/Downloads/algolib.zip [carpeta_destino]
#
# Qué es de quién:
#   TU ZONA   c/src/   cpp/include/algolib/labNN/   rust/src/   (tus implementaciones)
#             → un archivo que editaste NUNCA se sobrescribe. Si el curso trae una versión
#               distinta, se deja al lado como <archivo>.nuevo para que compares.
#             → un esqueleto que no has tocado (idéntico a una versión entregada,
#               según herramientas/originales.sha256) sí se actualiza.
#             → los archivos nuevos (p. ej. un lab que antes no existía) se agregan.
#   CURSO     notas/, tests, herramientas, Makefile, CMakeLists.txt, Cargo.toml, README…
#             → se reemplazan por la versión nueva.
#   Nunca se borra nada. Antes de empezar se guarda un respaldo completo en
#   ../algolib-respaldo-AAAAMMDD-HHMMSS.tar.gz
set -euo pipefail

ZIP=${1:?uso: actualizar.sh <algolib.zip> [carpeta_destino]}
DEST=${2:-$(cd "$(dirname "$0")/.." && pwd)}
DEST=$(cd "$DEST" && pwd)
[ -f "$DEST/Makefile" ] && [ -d "$DEST/cpp" ] && [ -d "$DEST/rust" ] || {
  echo "✗ $DEST no parece una copia de algolib (falta Makefile, cpp/ o rust/)"; exit 1; }

TMP=$(mktemp -d)
trap 'rm -rf -- "$TMP"' EXIT
unzip -q "$ZIP" -d "$TMP"
NEW=$(dirname "$(find "$TMP" -maxdepth 3 -name Makefile -path '*algolib*' | head -n 1)")
[ -d "$NEW/cpp" ] || { echo "✗ el zip no contiene una carpeta algolib válida"; exit 1; }

# 1. Respaldo
STAMP=$(date +%Y%m%d-%H%M%S)
BACKUP="$(dirname "$DEST")/algolib-respaldo-$STAMP.tar.gz"
tar -czf "$BACKUP" -C "$(dirname "$DEST")" --exclude='*/cpp/build' --exclude='*/c/build' --exclude='*/rust/target' --exclude='*/.git' "$(basename "$DEST")"
echo "• respaldo: $BACKUP"

huella() {
  if command -v sha256sum >/dev/null 2>&1; then sha256sum "$1" | cut -d' ' -f1
  else shasum -a 256 "$1" | cut -d' ' -f1; fi
}
MANIFIESTO="$NEW/herramientas/originales.sha256"

# ¿El archivo local es idéntico a alguna versión que el curso entregó? (= no lo has editado)
sin_editar() {
  [ -f "$MANIFIESTO" ] && grep -qx "$(huella "$2")  $1" "$MANIFIESTO"
}

# ¿El curso nunca ha cambiado este archivo? (solo existe una versión entregada y es esta)
curso_sin_cambios() {
  [ -f "$MANIFIESTO" ] || return 1
  local n
  n=$(grep -c "  $1\$" "$MANIFIESTO" || true)
  [ "$n" = 1 ] && grep -qx "$(huella "$2")  $1" "$MANIFIESTO"
}

es_zona_alumno() {
  case "$1" in
    cpp/include/algolib/lab*/*) return 0 ;;
    rust/src/*) return 0 ;;
    c/src/*) return 0 ;;
    *) return 1 ;;
  esac
}

act=0; agr=0; conserv=0
CONSERVADOS=""
cd "$NEW"
while IFS= read -r -d '' f; do
  rel=${f#./}
  case "$rel" in cpp/build/*|c/build/*|rust/target/*|.git/*) continue ;; esac
  dst="$DEST/$rel"
  if [ ! -e "$dst" ]; then
    mkdir -p "$(dirname "$dst")"; cp -p "$f" "$dst"; agr=$((agr + 1))
  elif cmp -s "$f" "$dst"; then
    :
  elif es_zona_alumno "$rel" && sin_editar "$rel" "$dst"; then
    cp -p "$f" "$dst"; act=$((act + 1))           # esqueleto que no habías tocado
  elif es_zona_alumno "$rel" && curso_sin_cambios "$rel" "$f"; then
    :                                               # lo editaste tú y el curso no lo cambió
  elif es_zona_alumno "$rel"; then
    cp -p "$f" "$dst.nuevo"; conserv=$((conserv + 1))
    CONSERVADOS="$CONSERVADOS\n    $rel   (versión del curso en $rel.nuevo)"
  else
    cp -p "$f" "$dst"; act=$((act + 1))
  fi
done < <(find . -type f -print0)

echo "• archivos del curso actualizados: $act"
echo "• archivos nuevos agregados:       $agr"
if [ "$conserv" -gt 0 ]; then
  echo "• tus archivos conservados (el curso trae otra versión): $conserv"
  printf "$CONSERVADOS\n"
  echo "  Compara con:  diff <archivo> <archivo>.nuevo   y borra el .nuevo cuando termines."
else
  echo "• ninguno de tus archivos de labs fue tocado"
fi
echo "✓ listo. Prueba con:  make lab01"
