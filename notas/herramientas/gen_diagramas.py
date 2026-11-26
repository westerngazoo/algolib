#!/usr/bin/env python3
"""Genera los diagramas SVG de las notas (notas/diagramas/*.svg).

Uso:  python3 notas/herramientas/gen_diagramas.py
Cada SVG lleva fondo blanco propio, así se lee igual en tema claro y oscuro,
en GitHub (como <img>) y en las páginas HTML (incrustado).
"""
import math
import pathlib

OUT = pathlib.Path(__file__).resolve().parent.parent / "diagramas"
OUT.mkdir(exist_ok=True)

INK, MUTED, COB, PALE, LINE = "#111827", "#6b7280", "#1240e0", "#eef2ff", "#c7d2fe"
ACC, ACC_PALE, OK = "#c2410c", "#ffedd5", "#15803d"
FONT = "Inter, 'Segoe UI', Helvetica, Arial, sans-serif"
MONO = "'JetBrains Mono', 'DejaVu Sans Mono', Menlo, Consolas, monospace"


def esc(t):
    return str(t).replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def svg(w, h, body, title):
    return f"""<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {w} {h}" width="{w}" height="{h}" role="img" aria-label="{esc(title)}" font-family="{FONT}" font-size="14" fill="{INK}">
<title>{esc(title)}</title>
<defs>
  <marker id="ar" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" fill="{INK}"/></marker>
  <marker id="arc" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" fill="{COB}"/></marker>
  <marker id="ara" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" fill="{ACC}"/></marker>
</defs>
<rect width="{w}" height="{h}" rx="12" fill="#ffffff"/>
{body}
</svg>
"""


def text(x, y, t, size=14, anchor="start", fill=INK, weight=400, family=FONT, italic=False):
    st = ' font-style="italic"' if italic else ""
    return (f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" fill="{fill}" '
            f'font-weight="{weight}" font-family="{family}"{st}>{esc(t)}</text>')


def box(x, y, w, h, fill=PALE, stroke=COB, rx=10, sw=1.5, dash=None):
    d = f' stroke-dasharray="{dash}"' if dash else ""
    return f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" fill="{fill}" stroke="{stroke}" stroke-width="{sw}"{d}/>'


def arrow(x1, y1, x2, y2, color=INK, marker="ar", sw=1.6, dash=None):
    d = f' stroke-dasharray="{dash}"' if dash else ""
    return f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{color}" stroke-width="{sw}" marker-end="url(#{marker})"{d}/>'


def write(name, content):
    (OUT / name).write_text(content, encoding="utf-8")
    print("escrito", OUT / name)


# 1. Ciclo del curso --------------------------------------------------------------
def ciclo_curso():
    W, H = 760, 250
    b = []
    steps = [
        ("Notas", ["teoría y", "demostraciones"]),
        ("Lab", ["C++ y Rust", "make labNN"]),
        ("Problem set", ["demostraciones y", "extensiones"]),
        ("Entrevistas", ["al cerrar", "cada unidad"]),
    ]
    bw, bh, gap, y = 150, 92, 46, 40
    x0 = (W - (4 * bw + 3 * gap)) / 2
    for i, (t, sub) in enumerate(steps):
        x = x0 + i * (bw + gap)
        hi = i == 1
        b.append(box(x, y, bw, bh, fill=COB if hi else PALE, stroke=COB))
        c = "#ffffff" if hi else INK
        b.append(text(x + bw / 2, y + 32, t, 17, "middle", c, 700))
        for j, s in enumerate(sub):
            b.append(text(x + bw / 2, y + 56 + 18 * j, s, 13, "middle", "#dbe4ff" if hi else MUTED))
        if i < 3:
            b.append(arrow(x + bw + 6, y + bh / 2, x + bw + gap - 6, y + bh / 2))
    # chips de estado bajo el lab
    lx = x0 + (bw + gap)
    chips = [("TODO", MUTED, "#f3f4f6"), ("FAIL", ACC, ACC_PALE), ("OK", OK, "#dcfce7")]
    cx = lx - 22
    for i, (t, col, bg) in enumerate(chips):
        xx = cx + i * 66
        b.append(box(xx, y + bh + 22, 54, 24, fill=bg, stroke=col, rx=12, sw=1.2))
        b.append(text(xx + 27, y + bh + 39, t, 12, "middle", col, 700, MONO))
        if i < 2:
            b.append(arrow(xx + 56, y + bh + 34, xx + 64, y + bh + 34, MUTED, "ar", 1.2))
    b.append(text(lx + bw / 2, y + bh + 66, "los tests pasan de pendiente a correcto", 12, "middle", MUTED))
    # vuelta
    xe = x0 + 3 * (bw + gap) + bw / 2
    xs = x0 + bw / 2
    yb = 228
    b.append(f'<path d="M{xe},{y+bh} V{yb-12} Q{xe},{yb} {xe-12},{yb} H{xs+12} Q{xs},{yb} {xs},{yb-12} V{y+bh+6}" '
             f'fill="none" stroke="{MUTED}" stroke-width="1.4" stroke-dasharray="5 4" marker-end="url(#ar)"/>')
    b.append(text(x0 + 3 * (bw + gap) - gap / 2, yb - 6, "siguiente lección", 12, "middle", MUTED))
    write("ciclo-curso.svg", svg(W, H, "\n".join(b), "Ciclo de cada lección: notas, lab, problem set, entrevistas"))


# 2. Modelo de memoria ------------------------------------------------------------
def memoria():
    W, H = 720, 380
    b = []
    x, w = 190, 200
    regs = [  # (y, h, titulo, fill, stroke, nota)
        (30, 70, "stack (pila)", PALE, COB, ["variables locales y marcos de llamada", "tamaño fijo (~8 MiB):", "la recursión profunda la desborda"]),
        (100, 110, "", "#ffffff", LINE, []),
        (210, 70, "heap (montículo)", ACC_PALE, ACC, ["memoria dinámica", "new / delete · Box · Vec · std::vector"]),
        (280, 40, "datos estáticos", "#f3f4f6", MUTED, ["variables globales y constantes"]),
        (320, 40, "código", "#f3f4f6", MUTED, ["instrucciones del programa"]),
    ]
    for (yy, hh, t, f, s, notas) in regs:
        b.append(f'<rect x="{x}" y="{yy}" width="{w}" height="{hh}" fill="{f}" stroke="{s}" stroke-width="1.4"/>')
        if t:
            b.append(text(x + w / 2, yy + hh / 2 + 5, t, 15, "middle", INK, 700))
        for j, n in enumerate(notas):
            b.append(text(x + w + 22, yy + 22 + 18 * j, n, 13, "start", MUTED if j else INK))
    b.append(arrow(x + w / 2, 104, x + w / 2, 138, COB, "arc", 2))
    b.append(text(x + w / 2 + 10, 128, "crece hacia abajo", 12, "start", COB))
    b.append(arrow(x + w / 2, 206, x + w / 2, 172, ACC, "ara", 2))
    b.append(text(x + w / 2 + 10, 186, "crece hacia arriba", 12, "start", ACC))
    b.append(text(x + w / 2, 160, "espacio libre", 13, "middle", MUTED, italic=True))
    b.append(text(x - 14, 40, "dirección alta", 13, "end", MUTED))
    b.append(text(x - 14, 356, "dirección baja", 13, "end", MUTED))
    b.append(arrow(x - 60, 340, x - 60, 52, MUTED, "ar", 1.2))
    write("memoria.svg", svg(W, H, "\n".join(b), "Mapa de memoria de un proceso: stack, heap, datos y código"))


# 3. Mapa de la lección 1 -----------------------------------------------------------
def mapa_l01():
    W, H = 760, 300
    rows = [
        [("§1", "Motivación"), ("§2", "Modelo de cómputo"), ("§3", "Notación asintótica")],
        [("§4", "TAD Pila"), ("§5", "Arreglo estático"), ("§6", "Arreglo dinámico")],
        [("§7", "Del papel al código"), ("§8", "Guía del lab"), ("§9–10", "Ejercicios")],
    ]
    bw, bh, gx, x0, y0, gy = 200, 58, 50, 40, 26, 40
    b = []
    pos = {}
    for r, row in enumerate(rows):
        order = row if r % 2 == 0 else row[::-1]
        for c, (n, t) in enumerate(order):
            x = x0 + c * (bw + gx)
            y = y0 + r * (bh + gy)
            core = n == "§6"
            b.append(box(x, y, bw, bh, fill=COB if core else PALE, stroke=COB, sw=2 if core else 1.4))
            b.append(text(x + 14, y + 24, n, 13, "start", "#dbe4ff" if core else COB, 700))
            b.append(text(x + 14, y + 44, t, 14, "start", "#ffffff" if core else INK, 600))
            pos[n] = (x, y)
    seq = [n for row in rows for (n, _) in row]
    for a_, b_ in zip(seq, seq[1:]):
        (x1, y1), (x2, y2) = pos[a_], pos[b_]
        if y1 == y2:
            if x2 > x1:
                b.append(arrow(x1 + bw + 4, y1 + bh / 2, x2 - 4, y2 + bh / 2))
            else:
                b.append(arrow(x1 - 4, y1 + bh / 2, x2 + bw + 4, y2 + bh / 2))
        else:
            b.append(arrow(x1 + bw / 2, y1 + bh + 3, x2 + bw / 2, y2 - 4))
    x, y = pos["§6"]
    b.append(text(x + bw / 2 + 10, y + bh + 22, "análisis amortizado: núcleo de la lección", 12, "start", COB, 700))
    write("mapa-l01.svg", svg(W, H, "\n".join(b), "Mapa de la lección 1"))


# 4. Definición de O grande -----------------------------------------------------------
def big_o():
    W, H = 720, 360
    L, R, T, B = 70, 680, 30, 300
    xs = [i / 4 for i in range(0, 4 * 100 + 1)]
    f = lambda t: 20 + 0.35 * t + 5 * math.sin(t / 3.5)   # f(n)
    cg = lambda t: 1.0 * t                                   # c·g(n)
    n0 = max(t for t in xs if f(t) > cg(t))
    n0 = math.ceil(n0)
    sx = lambda t: L + (R - L) * t / 100
    sy = lambda v: B - (B - T) * v / 110
    b = []
    b.append(f'<rect x="{sx(n0)}" y="{T}" width="{R-sx(n0)}" height="{B-T}" fill="{PALE}"/>')
    b.append(arrow(L, B, R + 12, B))
    b.append(arrow(L, B, L, T - 12))
    b.append(text(R + 4, B + 22, "n", 15, "end", INK, 600, italic=True))
    b.append(text(L - 12, T - 2, "costo", 13, "end", MUTED))
    pts_f = " ".join(f"{sx(t):.1f},{sy(f(t)):.1f}" for t in xs)
    b.append(f'<polyline points="{pts_f}" fill="none" stroke="{ACC}" stroke-width="2.6"/>')
    b.append(f'<line x1="{sx(0)}" y1="{sy(0)}" x2="{sx(100)}" y2="{sy(cg(100))}" stroke="{COB}" stroke-width="2.6"/>')
    b.append(f'<line x1="{sx(n0)}" y1="{T}" x2="{sx(n0)}" y2="{B}" stroke="{INK}" stroke-width="1.2" stroke-dasharray="5 4"/>')
    b.append(text(sx(n0), B + 22, "n₀", 15, "middle", INK, 700))
    b.append(text(sx(97), sy(cg(97)) + 26, "c·g(n)", 15, "end", COB, 700))
    b.append(text(sx(8), sy(f(8)) - 14, "f(n)", 15, "middle", ACC, 700))
    b.append(text(sx(90), sy(f(90)) - 14, "f(n)", 15, "middle", ACC, 700))
    b.append(text(sx(n0) + 14, T + 22, "a partir de n₀, f(n) ≤ c·g(n) siempre", 13, "start", COB, 600))
    b.append(text(sx(n0 / 2), T + 22, "aquí puede fallar:", 12, "middle", MUTED))
    b.append(text(sx(n0 / 2), T + 38, "no importa", 12, "middle", MUTED))
    write("big-o.svg", svg(W, H, "\n".join(b), "Definición de O grande: a partir de n0, f queda por debajo de c por g"))


# 5. Crecimiento del arreglo dinámico ------------------------------------------------
def crecimiento():
    W, H = 760, 250
    b = []
    cw, ch, y = 46, 46, 70
    xl = 30
    b.append(text(xl, 40, "Antes: k = 4, m = 4 (lleno)", 14, "start", INK, 700))
    for i, c in enumerate("abcd"):
        b.append(box(xl + i * cw, y, cw, ch, fill=PALE, stroke=COB, rx=0))
        b.append(text(xl + i * cw + cw / 2, y + 29, c, 17, "middle", INK, 600, MONO))
    b.append(box(xl - 6, y - 6, 4 * cw + 12, ch + 12, fill="none", stroke=MUTED, rx=6, sw=1.2, dash="5 4"))
    b.append(text(xl + 2 * cw, y + ch + 30, "bloque viejo: se libera (3)", 12, "middle", MUTED))
    xr = 370
    b.append(text(xr, 40, "Después: k = 5, m = 8", 14, "start", INK, 700))
    for i in range(8):
        if i < 4:
            f, s, t = PALE, COB, "abcd"[i]
        elif i == 4:
            f, s, t = ACC_PALE, ACC, "e"
        else:
            f, s, t = "#ffffff", LINE, ""
        b.append(box(xr + i * cw, y, cw, ch, fill=f, stroke=s, rx=0))
        if t:
            b.append(text(xr + i * cw + cw / 2, y + 29, t, 17, "middle", INK, 600, MONO))
    b.append(text(xr + 2 * cw, y + ch + 30, "copiados (2): 4 operaciones", 12, "middle", COB))
    b.append(text(xr + 4.5 * cw, y + ch + 48, "escrito (4)", 12, "middle", ACC))
    b.append(text(xr + 6.5 * cw, y + ch + 30, "libres", 12, "middle", MUTED))
    b.append(arrow(xl + 4 * cw + 16, y + ch / 2, xr - 14, y + ch / 2, INK, "ar", 2))
    b.append(text((xl + 4 * cw + xr) / 2, y - 10, "push(e)", 15, "middle", INK, 700, MONO))
    steps = ["(1) reservar 8 celdas", "(2) copiar los 4 elementos", "(3) liberar el bloque viejo", "(4) escribir e"]
    for j, s in enumerate(steps):
        b.append(text(30 + j * 182, 222, s, 13, "start", INK))
    write("crecimiento-arreglo.svg", svg(W, H, "\n".join(b), "Push que provoca duplicación: de capacidad 4 a 8"))


# 6. Costo por push y promedio acumulado -----------------------------------------
def costos_push():
    N = 33
    costs, cap, k = [], 0, 0
    for _ in range(N):
        c = 1
        if k == cap:
            c += k
            cap = max(1, 2 * cap)
        k += 1
        costs.append(c)
    W, H = 760, 340
    L, R, T, B = 60, 740, 30, 280
    ymax = 34
    sx = lambda i: L + (R - L) * (i + 0.5) / N
    sy = lambda v: B - (B - T) * v / ymax
    bw = (R - L) / N * 0.62
    b = []
    for v in (0, 3, 10, 20, 30):
        b.append(f'<line x1="{L}" y1="{sy(v)}" x2="{R}" y2="{sy(v)}" stroke="#e5e7eb"/>')
        b.append(text(L - 8, sy(v) + 4, v, 12, "end", MUTED))
    for i, c in enumerate(costs):
        cara = c > 1
        b.append(f'<rect x="{sx(i)-bw/2:.1f}" y="{sy(c):.1f}" width="{bw:.1f}" height="{B-sy(c):.1f}" '
                 f'fill="{ACC if cara else LINE}"/>')
        if cara and c >= 5:
            b.append(text(sx(i), sy(c) - 6, c, 12, "middle", ACC, 700))
    acc, pts = 0, []
    for i, c in enumerate(costs):
        acc += c
        pts.append(f"{sx(i):.1f},{sy(acc/(i+1)):.1f}")
    b.append(f'<line x1="{L}" y1="{sy(3)}" x2="{R}" y2="{sy(3)}" stroke="{INK}" stroke-width="1.4" stroke-dasharray="6 4"/>')
    b.append(f'<polyline points="{" ".join(pts)}" fill="none" stroke="{COB}" stroke-width="2.6"/>')
    for i in (0, 8, 16, 24, 32):
        b.append(text(sx(i), B + 20, i + 1, 12, "middle", MUTED))
    b.append(text((L + R) / 2, B + 44, "número de push", 13, "middle", MUTED))
    b.append(text(sx(18), sy(3) - 8, "cota amortizada = 3", 13, "start", INK, 700))
    b.append(text(sx(18), sy(28), "barras: costo de cada push", 13, "start", ACC, 600))
    b.append(text(sx(18), sy(28) + 18, "(caro cuando duplica)", 12, "start", ACC))
    b.append(text(sx(18), sy(20), "línea azul: costo acumulado ÷ n", 13, "start", COB, 600))
    write("costos-push.svg", svg(W, H, "\n".join(b), "Costo de cada push y promedio acumulado, siempre por debajo de 3"))


# 7. Contracción: umbral ingenuo vs histéresis --------------------------------------
def contraccion():
    def simular(umbral_div):
        k, m, out = 8, 8, []
        for op in ["push", "pop"] * 6:
            c = 1
            if op == "push":
                if k == m:
                    c += k
                    m *= 2
                k += 1
            else:
                k -= 1
                if m >= 2 and k == m // umbral_div:
                    c += k
                    m //= 2
            out.append((op, c, m))
        return out

    W, H = 760, 350
    b = []
    panels = [("Contraer cuando k = m/2  (incorrecto)", simular(2), ACC, 40),
              ("Contraer cuando k = m/4  (histéresis)", simular(4), COB, 185)]
    L, R = 60, 740
    for title, data, col, y0 in panels:
        b.append(text(L, y0 + 4, title, 14, "start", col, 700))
        n = len(data)
        bw = (R - L) / n
        base = y0 + 105
        total = 0
        for i, (op, c, m) in enumerate(data):
            h = 9 * c
            total += c
            b.append(f'<rect x="{L+i*bw+6:.1f}" y="{base-h}" width="{bw-12:.1f}" height="{h}" fill="{col if c>1 else LINE}"/>')
            b.append(text(L + i * bw + bw / 2, base + 16, op, 11, "middle", MUTED, family=MONO))
            b.append(text(L + i * bw + bw / 2, base - h - 5, c, 11, "middle", col if c > 1 else MUTED, 700))
        b.append(text(R, y0 + 4, f"costo total: {total}", 13, "end", INK, 700))
    b.append(text(L, 338, "Inicio: k = m = 8. Cada barra es el costo de una operación (copias + 1).", 12, "start", MUTED))
    write("contraccion.svg", svg(W, H, "\n".join(b), "Secuencia push/pop alternada: umbral ingenuo contra histéresis"))


# 8. 2 + sin n: Θ(n) sin límite ------------------------------------------------------
def seno():
    W, H = 720, 260
    L, R, T, B = 60, 700, 30, 210
    N = 60
    sx = lambda n: L + (R - L) * n / N
    sy = lambda v: B - (B - T) * v / 3.6
    b = []
    for v, lab in ((1, "1"), (3, "3")):
        b.append(f'<line x1="{L}" y1="{sy(v)}" x2="{R}" y2="{sy(v)}" stroke="{COB}" stroke-width="1.4" stroke-dasharray="6 4"/>')
        b.append(text(L - 8, sy(v) + 4, lab, 13, "end", COB, 700))
    b.append(arrow(L, B, R + 10, B))
    b.append(arrow(L, B, L, T - 10))
    for n in range(1, N + 1):
        b.append(f'<circle cx="{sx(n):.1f}" cy="{sy(2+math.sin(n)):.1f}" r="3.4" fill="{ACC}"/>')
    b.append(text(R, B + 22, "n", 14, "end", INK, 600, italic=True))
    b.append(text(L + 6, T - 4, "f(n)/n = 2 + sin n", 13, "start", ACC, 700))
    b.append(text(R, T - 4, "siempre entre 1 y 3 → Θ(n), pero el cociente no converge", 12, "end", MUTED))
    write("seno.svg", svg(W, H, "\n".join(b), "El cociente 2 + sin n oscila entre 1 y 3 sin converger"))


# 9. Vocabulario de grafos ----------------------------------------------------------
def grafos():
    W, H = 780, 300
    b = []

    def nodo(x, y, t, fill=PALE, stroke=COB):
        b.append(f'<circle cx="{x}" cy="{y}" r="17" fill="{fill}" stroke="{stroke}" stroke-width="1.8"/>')
        b.append(text(x, y + 5, t, 14, "middle", INK, 700))

    def arista(p, q, color=MUTED, sw=2.2, flecha=False):
        (x1, y1), (x2, y2) = p, q
        dx, dy = x2 - x1, y2 - y1
        d = math.hypot(dx, dy)
        ux, uy = dx / d, dy / d
        a = (x1 + ux * 18, y1 + uy * 18)
        c = (x2 - ux * (22 if flecha else 18), y2 - uy * (22 if flecha else 18))
        m = ' marker-end="url(#ar)"' if flecha else ""
        b.append(f'<line x1="{a[0]:.1f}" y1="{a[1]:.1f}" x2="{c[0]:.1f}" y2="{c[1]:.1f}" stroke="{color}" stroke-width="{sw}"{m}/>')

    # Panel 1: grafo no dirigido
    P = {0: (70, 100), 1: (190, 80), 2: (130, 190), 3: (230, 220), 4: (230, 140)}
    E = [(0, 1), (0, 2), (1, 2), (2, 3), (3, 4)]
    for u, v in E:
        ciclo = {u, v} <= {0, 1, 2}
        arista(P[u], P[v], ACC if ciclo else MUTED, 3 if ciclo else 2.2)
    for k, (x, y) in P.items():
        nodo(x, y, k, ACC_PALE if k == 2 else PALE, ACC if k == 2 else COB)
    b.append(text(20, 30, "Grafo no dirigido", 15, "start", INK, 700))
    b.append(text(20, 262, "n = 5 vértices, m = 5 aristas", 12, "start", MUTED))
    b.append(text(20, 280, "naranja: ciclo 0–1–2 · grado(2) = 3", 12, "start", ACC))

    # Panel 2: árbol
    T = {0: (400, 70), 1: (345, 140), 2: (455, 140), 3: (310, 215), 4: (380, 215)}
    for u, v in [(0, 1), (0, 2), (1, 3), (1, 4)]:
        arista(T[u], T[v])
    for k, (x, y) in T.items():
        nodo(x, y, k)
    b.append(text(290, 30, "Árbol", 15, "start", INK, 700))
    b.append(text(290, 262, "conexo y sin ciclos", 12, "start", MUTED))
    b.append(text(290, 280, "5 vértices → 4 aristas (n − 1)", 12, "start", MUTED))

    # Panel 3: DAG
    D = {0: (560, 90), 1: (560, 200), 2: (650, 145), 3: (740, 90), 4: (740, 200)}
    for u, v in [(0, 2), (1, 2), (2, 3), (2, 4), (0, 3)]:
        arista(D[u], D[v], INK, 1.8, True)
    for k, (x, y) in D.items():
        nodo(x, y, k)
    b.append(text(540, 30, "DAG (dirigido, acíclico)", 15, "start", INK, 700))
    b.append(text(540, 262, "las flechas nunca forman ciclo", 12, "start", MUTED))
    b.append(text(540, 280, "orden topológico: 0 1 2 3 4", 12, "start", MUTED))
    write("grafos.svg", svg(W, H, "\n".join(b), "Un grafo no dirigido con un ciclo, un árbol y un DAG"))


if __name__ == "__main__":
    ciclo_curso(); memoria(); mapa_l01(); big_o(); crecimiento(); costos_push(); contraccion(); seno(); grafos()
