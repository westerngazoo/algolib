#!/usr/bin/env python3
"""Copia los programas de una lección a carpetas propias, listos para compilar y modificar.

Uso:   python3 notas/herramientas/extraer_ejemplos.py L00 [L01 ...] [--forzar]
       make extraer-L00        (o `make extraer` para todas las lecciones)

Por cada grupo de pestañas de las notas crea `ejemplos/LNN/NN-<sección>/` con:
  programa.c, programa.cpp, programa.rs    los programas tal como aparecen en las notas
  salida-esperada.txt                       si las notas la incluyen (salida-de-las-notas.txt cuando
                                            cada lenguaje imprime algo distinto a propósito)
  Makefile                                  `make` compila y corre los tres y compara la salida;
                                            `make c`, `make cpp`, `make rust` corren uno solo

Las carpetas que ya existen NO se sobrescriben (pueden tener tus cambios); usa --forzar
para regenerarlas. Reutiliza el lector de bloques y las banderas de check_ejemplos.py,
así que cada programa se compila exactamente igual que en `make ejemplos`.
"""
import pathlib
import re
import shlex
import shutil
import sys
import unicodedata

from check_ejemplos import CMD, EXT, NOTAS, groups

RAIZ = NOTAS.parent / "ejemplos"
HEADING = re.compile(r"^(#{2,4})\s+(.*)$")


def slug(texto, largo=40):
    """'1.2 Cuantificadores: "para todo"' -> '1-2-cuantificadores-para-todo'."""
    s = unicodedata.normalize("NFKD", texto).encode("ascii", "ignore").decode()
    s = re.sub(r"[^a-zA-Z0-9]+", "-", s).strip("-").lower()
    return s[:largo].rstrip("-") or "seccion"


def titulo_previo(lines, idx):
    """Título del encabezado más cercano antes de la línea idx (0-based)."""
    for k in range(idx, -1, -1):
        m = HEADING.match(lines[k])
        if m:
            return m.group(2)
    return "inicio"


def makefile(blocks, expected):
    """Makefile de la carpeta: un objetivo por lenguaje y `todo`, que compara salidas."""
    reglas, objetivos = [], []
    for lang, cls, _ in blocks:
        if lang is None:
            continue
        src, exe = f"programa.{EXT[lang]}", f"prog_{lang}"
        compilar = shlex.join(CMD[lang](src, exe))
        if "no-compila" in cls:
            cuerpo = [f"\t@echo '== {lang}: este programa NO debe compilar; lee el error del compilador =='",
                      f"\t-{compilar}"]
        elif "no-ejecutar" in cls:
            cuerpo = [f"\t{compilar}",
                      f"\t@echo '== {lang}: compila, pero no se ejecuta (tiene comportamiento indefinido a propósito) =='"]
        else:
            cuerpo = [f"\t@{compilar}", f"\t@echo '== {lang} =='", f"\t@./{exe} | tee salida_{lang}.txt"]
            if expected is not None:
                cuerpo.append(f"\t@diff -q salida_{lang}.txt salida-esperada.txt >/dev/null "
                              f"&& echo '   (igual a salida-esperada.txt)' "
                              f"|| echo '   (DISTINTA de salida-esperada.txt)'")
        reglas.append(f"{lang}:\n" + "\n".join(cuerpo) + "\n")
        objetivos.append(lang)
    cabecera = ("# Generado por `make extraer-LNN`. Puedes editar los programas libremente.\n"
                "#   make        compila y corre los tres lenguajes\n"
                "#   make c | make cpp | make rust\n"
                "#   make limpiar\n\n"
                f".PHONY: todo {' '.join(objetivos)} limpiar\n\n"
                f"todo: {' '.join(objetivos)}\n\n")
    limpiar = "limpiar:\n\trm -rf prog_* salida_*.txt *.dSYM\n"
    return cabecera + "\n".join(reglas) + "\n" + limpiar


def extraer(leccion, forzar):
    mds = sorted(NOTAS.glob(f"{leccion}-*.md"))
    mds = [m for m in mds if "guion" not in m.name]
    if not mds:
        sys.exit(f"✗ no encuentro las notas de {leccion} en {NOTAS}")
    md = mds[0]
    lines = md.read_text(encoding="utf-8").split("\n")
    destino = RAIZ / leccion
    creadas = saltadas = 0
    usados = set()
    for n, (start, gcls, blocks, expected) in enumerate(groups(md), 1):
        if not any(lang for lang, _, _ in blocks):
            continue
        nombre = f"{n:02d}-{slug(titulo_previo(lines, start - 1))}"
        while nombre in usados:  # dos grupos bajo el mismo encabezado
            nombre += "-b"
        usados.add(nombre)
        carpeta = destino / nombre
        if carpeta.exists() and not forzar:
            saltadas += 1
            continue
        if carpeta.exists():  # --forzar: empezar de cero, sin archivos de una extracción anterior
            shutil.rmtree(carpeta)
        carpeta.mkdir(parents=True)
        for lang, _cls, code in blocks:
            if lang is not None:
                (carpeta / f"programa.{EXT[lang]}").write_text(code, encoding="utf-8")
        # En los grupos .no-compare cada lenguaje imprime algo distinto a propósito, y el
        # bloque de salida de las notas es un resumen: se guarda como referencia, sin comparar.
        comparar = expected if "no-compare" not in gcls else None
        if expected is not None:
            nombre_salida = "salida-esperada.txt" if comparar is not None else "salida-de-las-notas.txt"
            (carpeta / nombre_salida).write_text(expected, encoding="utf-8")
        (carpeta / "Makefile").write_text(makefile(blocks, comparar), encoding="utf-8")
        creadas += 1
    rel = destino.relative_to(NOTAS.parent)
    print(f"• {leccion}: {creadas} carpetas nuevas en {rel}/"
          + (f", {saltadas} ya existían (no se tocaron; usa --forzar para regenerarlas)" if saltadas else ""))
    if creadas:
        print(f"  prueba:  cd {rel}/{sorted(p.name for p in destino.iterdir())[0]} && make")


def main():
    args = sys.argv[1:]
    forzar = "--forzar" in args
    lecciones = [a for a in args if a != "--forzar"]
    if not lecciones:
        lecciones = sorted({p.name[:3] for p in NOTAS.glob("L[0-9][0-9]-*.md")})
    for l in lecciones:
        extraer(l.upper(), forzar)


if __name__ == "__main__":
    main()
