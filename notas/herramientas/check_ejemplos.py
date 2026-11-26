#!/usr/bin/env python3
"""Compila y ejecuta todos los ejemplos de código de las notas.

Uso:   python3 notas/herramientas/check_ejemplos.py [archivo.md ...]
       make ejemplos

Reglas (en el Markdown):
  ::: {.tabs}            grupo de pestañas; cada bloque ```c / ```cpp / ```rust es un programa completo
  ```{.rust .no-compila} el bloque DEBE fallar al compilar (ej.: errores del borrow checker)
  ```{.cpp .no-ejecutar} se compila pero no se ejecuta (ej.: comportamiento indefinido a propósito)
  ::: {.tabs .no-compare} no se exige que las salidas coincidan entre lenguajes
  ```{.text .salida}     justo después del grupo: salida esperada (se compara con cada programa)

C y C++ se compilan con -Wall -Wextra y AddressSanitizer + UBSan; Rust en modo debug
(que detecta desbordamientos aritméticos).
"""
import pathlib
import re
import subprocess
import sys
import tempfile

NOTAS = pathlib.Path(__file__).resolve().parent.parent
SAN = ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
CMD = {
    "c": lambda src, out: ["gcc", "-std=c17", "-Wall", "-Wextra", *SAN, src, "-o", out, "-lm"],
    "cpp": lambda src, out: ["g++", "-std=c++20", "-Wall", "-Wextra", *SAN, src, "-o", out],
    "rust": lambda src, out: ["rustc", "--edition", "2021", "-A", "dead_code", "-A", "unused", src, "-o", out],
}
EXT = {"c": "c", "cpp": "cpp", "rust": "rs"}

FENCE = re.compile(r"^```\s*(\{[^}]*\}|[\w+-]+)?\s*$")


def parse_info(info):
    if not info:
        return None, set()
    if info.startswith("{"):
        parts = info.strip("{}").split()
        classes = {p[1:] for p in parts if p.startswith(".")}
        lang = next((c for c in classes if c in CMD), None)
        return lang, classes
    return (info if info in CMD else None), set()


def groups(md_path):
    """Devuelve [(línea, clases_grupo, [(lang, clases, código)], salida_esperada|None)]."""
    lines = md_path.read_text(encoding="utf-8").split("\n")
    i, out = 0, []
    while i < len(lines):
        m = re.match(r"^:::\s*\{([^}]*)\}\s*$", lines[i])
        if m and ".tabs" in m.group(1).split():
            gclasses = {p[1:] for p in m.group(1).split() if p.startswith(".")}
            start, i, blocks = i + 1, i + 1, []
            while i < len(lines) and lines[i].strip() != ":::":
                fm = FENCE.match(lines[i])
                if fm and lines[i].strip() != "```":
                    lang, cls = parse_info(fm.group(1))
                    j = i + 1
                    while lines[j].strip() != "```":
                        j += 1
                    blocks.append((lang, cls, "\n".join(lines[i + 1:j]) + "\n"))
                    i = j
                i += 1
            # salida esperada inmediatamente después
            k = i + 1
            while k < len(lines) and not lines[k].strip():
                k += 1
            expected = None
            if k < len(lines) and re.match(r"^```\s*\{[^}]*\.salida[^}]*\}\s*$", lines[k]):
                j = k + 1
                while lines[j].strip() != "```":
                    j += 1
                expected = "\n".join(lines[k + 1:j]) + "\n"
            out.append((start, gclasses, blocks, expected))
        i += 1
    return out


def run_group(md, line, gclasses, blocks, expected, tmp):
    errors, outputs = [], {}
    for n, (lang, cls, code) in enumerate(blocks):
        if lang is None:
            continue
        tag = f"{md.name}:{line} [{lang}]"
        src = tmp / f"ej_{line}_{n}.{EXT[lang]}"
        exe = tmp / f"ej_{line}_{n}"
        src.write_text(code, encoding="utf-8")
        cp = subprocess.run(CMD[lang](str(src), str(exe)), capture_output=True, text=True)
        if "no-compila" in cls:
            if cp.returncode == 0:
                errors.append(f"{tag}: debía NO compilar y compiló")
            continue
        if cp.returncode != 0:
            errors.append(f"{tag}: no compila\n{cp.stderr[-1500:]}")
            continue
        if cp.stderr.strip() and lang != "rust" and "no-ejecutar" not in cls:
            errors.append(f"{tag}: advertencias del compilador\n{cp.stderr[-800:]}")
        if "no-ejecutar" in cls:
            continue
        rp = subprocess.run([str(exe)], capture_output=True, text=True, timeout=30)
        if rp.returncode != 0:
            errors.append(f"{tag}: terminó con código {rp.returncode}\n{rp.stderr[-1500:]}")
            continue
        outputs[lang] = rp.stdout
    if "no-compare" not in gclasses:
        vals = list(outputs.items())
        for lang, o in vals[1:]:
            if o != vals[0][1]:
                errors.append(f"{md.name}:{line}: la salida de {lang} difiere de {vals[0][0]}:\n"
                              f"--- {vals[0][0]}\n{vals[0][1]}--- {lang}\n{o}")
        if expected is not None:
            for lang, o in outputs.items():
                if o != expected:
                    errors.append(f"{md.name}:{line} [{lang}]: salida distinta a la esperada:\n"
                                  f"--- esperada\n{expected}--- obtenida\n{o}")
    return errors, len([b for b in blocks if b[0]])


def main():
    files = [pathlib.Path(a) for a in sys.argv[1:]] or sorted(NOTAS.glob("L*.md"))
    total, errs = 0, []
    with tempfile.TemporaryDirectory() as td:
        tmp = pathlib.Path(td)
        for md in files:
            for line, gcls, blocks, expected in groups(md):
                e, n = run_group(md, line, gcls, blocks, expected, tmp)
                total += n
                errs += e
    for e in errs:
        print("✗", e, "\n")
    print(f"{total} programas revisados, {len(errs)} problemas")
    sys.exit(1 if errs else 0)


if __name__ == "__main__":
    main()
