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


# Lección 2 ---------------------------------------------------------------------------
def mapa_l02():
    W, H = 760, 300
    rows = [
        [("§1–2", "TAD Secuencia"), ("§3", "Tamaño y alineación"), ("§4", "Insertar y borrar")],
        [("§5", "reserve / shrink"), ("§6", "Memoria vs. objeto"), ("§7", "Reubicar")],
        [("§8", "Rust por dentro"), ("§9–10", "Paradigmas y lab"), ("§11–12", "Ejercicios")],
    ]
    bw, bh, gx, x0, y0, gy = 200, 58, 50, 40, 26, 40
    b, pos = [], {}
    for r, row in enumerate(rows):
        order = row if r % 2 == 0 else row[::-1]
        for c, (n, t) in enumerate(order):
            x, y = x0 + c * (bw + gx), y0 + r * (bh + gy)
            core = n == "§7"
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
    x, y = pos["§7"]
    b.append(text(x + bw / 2 + 10, y + bh + 22, "reubicación: núcleo de la lección", 12, "start", COB, 700))
    write("mapa-l02.svg", svg(W, H, "\n".join(b), "Mapa de la lección 2"))


def corrimiento():
    W, H = 860, 250
    cw, ch, x0 = 58, 46, 96
    b = []
    def fila(y, vals, etiqueta, resalta=None, crudas=()):
        b.append(text(x0 - 14, y + ch / 2 + 5, etiqueta, 13, "end", MUTED))
        for j, v in enumerate(vals):
            x = x0 + j * cw
            cruda = j in crudas
            f = ACC_PALE if j == resalta else ("#ffffff" if cruda else PALE)
            st = ACC if j == resalta else (LINE if cruda else COB)
            b.append(box(x, y, cw - 6, ch, fill=f, stroke=st, rx=6, dash="4 3" if cruda else None))
            b.append(text(x + (cw - 6) / 2, y + ch / 2 + 5, v, 15, "middle", MUTED if cruda else INK, 600, MONO))
            b.append(text(x + (cw - 6) / 2, y - 6, str(j), 11, "middle", MUTED, 400, MONO))
    fila(40, ["x0", "x1", "x2", "x3", "x4", "", "", ""], "antes", crudas=(5, 6, 7))
    fila(160, ["x0", "x1", "x", "x2", "x3", "x4", "", ""], "después", resalta=2, crudas=(6, 7))
    # flechas del corrimiento, del último al primero
    for n, j in enumerate([4, 3, 2]):
        xa = x0 + j * cw + (cw - 6) / 2
        xb = x0 + (j + 1) * cw + (cw - 6) / 2
        b.append(arrow(xa, 40 + ch + 4, xb, 160 - 18, COB, "arc", 1.6))
        b.append(text((xa + xb) / 2 + 8, 118, f"{n + 1}.º", 12, "start", COB, 700))
    b.append(text(x0 + 8 * cw + 24, 70, "insert(2, x) con k = 5, m = 8", 14, "start", INK, 700))
    b.append(text(x0 + 8 * cw + 24, 92, "se mueven k − i = 3 elementos,", 13, "start", MUTED))
    b.append(text(x0 + 8 * cw + 24, 110, "empezando por el último", 13, "start", MUTED))
    b.append(text(x0 + 8 * cw + 24, 190, "la celda 5 estaba cruda:", 13, "start", MUTED))
    b.append(text(x0 + 8 * cw + 24, 208, "en C++ se construye, no se asigna", 13, "start", MUTED))
    write("corrimiento.svg", svg(W, H, "\n".join(b), "Corrimiento a la derecha al insertar en la posición 2"))


# Lección 3 ---------------------------------------------------------------------------
def _mapa(nombre, titulo, rows, core, nota):
    bw, bh, gx, x0, y0, gy = 200, 58, 50, 40, 26, 40
    W, H = 760, 300
    b, pos = [], {}
    for r, row in enumerate(rows):
        order = row if r % 2 == 0 else row[::-1]
        for c, (n, t) in enumerate(order):
            x, y = x0 + c * (bw + gx), y0 + r * (bh + gy)
            hi = n == core
            b.append(box(x, y, bw, bh, fill=COB if hi else PALE, stroke=COB, sw=2 if hi else 1.4))
            b.append(text(x + 14, y + 24, n, 13, "start", "#dbe4ff" if hi else COB, 700))
            b.append(text(x + 14, y + 44, t, 14, "start", "#ffffff" if hi else INK, 600))
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
    x, y = pos[core]
    b.append(text(x + bw / 2 + 10, y + bh + 22, nota, 12, "start", COB, 700))
    write(nombre, svg(W, H, "\n".join(b), titulo))


def mapa_l03():
    _mapa("mapa-l03.svg", "Mapa de la lección 3", [
        [("§1", "Motivación"), ("§2", "Lista simple"), ("§3", "¿Quién es dueño?")],
        [("§4", "Lista doble"), ("§5", "Ownership del ciclo"), ("§6", "Option<Box<Node>>")],
        [("§7", "Localidad"), ("§8–9", "Paradigmas y lab"), ("§10–11", "Ejercicios")],
    ], "§5", "ownership con ciclos: núcleo de la lección")


def _nodo(b, x, y, val, w=96, h=44, campos=("v", "next")):
    b.append(box(x, y, w, h, fill=PALE, stroke=COB, rx=6))
    b.append(f'<line x1="{x + w * 0.55}" y1="{y}" x2="{x + w * 0.55}" y2="{y + h}" stroke="{COB}" stroke-width="1.2"/>')
    b.append(text(x + w * 0.275, y + h / 2 + 5, val, 15, "middle", INK, 600, MONO))


def lista_simple():
    W, H = 760, 150
    b = []
    b.append(box(24, 50, 70, 44, fill="#f3f4f6", stroke=MUTED, rx=6))
    b.append(text(59, 77, "head", 14, "middle", INK, 600, MONO))
    b.append(text(59, 116, "size = 4", 12, "middle", MUTED, 400, MONO))
    xs = [140, 290, 440, 590]
    b.append(arrow(94, 72, xs[0] - 4, 72))
    for i, x in enumerate(xs):
        _nodo(b, x, 50, f"v{i}")
        cx = x + 96 * 0.775
        if i < 3:
            b.append(f'<circle cx="{cx}" cy="72" r="3.5" fill="{INK}"/>')
            b.append(arrow(cx, 72, xs[i + 1] - 4, 72))
        else:
            b.append(text(cx, 77, "∅", 16, "middle", ACC, 700))
    b.append(text(380, 30, "cada nodo es DUEÑO del siguiente (unique_ptr / Box)", 13, "middle", MUTED))
    write("lista-simple.svg", svg(W, H, "\n".join(b), "Lista simplemente ligada de 4 nodos"))


def ownership_listas():
    W, H = 760, 300
    b = []
    b.append(text(30, 34, "Lista simple: el grafo de ownership es una cadena (un árbol)", 14, "start", INK, 700))
    xs = [60, 210, 360, 510]
    b.append(box(xs[0], 52, 90, 40, fill="#f3f4f6", stroke=MUTED, rx=6))
    b.append(text(xs[0] + 45, 77, "lista", 13, "middle", INK, 600, MONO))
    for i, x in enumerate(xs[1:]):
        b.append(box(x, 52, 90, 40, rx=6))
        b.append(text(x + 45, 77, f"nodo {i}", 13, "middle", INK, 600, MONO))
    for i in range(3):
        b.append(arrow(xs[i] + 92, 72, xs[i + 1] - 4, 72, COB, "arc", 2))
    b.append(text(640, 70, "flecha = posee", 12, "start", COB, 700))
    b.append(text(30, 150, "Lista doble: apuntar ≠ poseer", 14, "start", INK, 700))
    b.append(box(xs[0], 230, 90, 40, fill="#f3f4f6", stroke=MUTED, rx=6))
    b.append(text(xs[0] + 45, 255, "lista", 13, "middle", INK, 600, MONO))
    for i, x in enumerate(xs[1:]):
        b.append(box(x, 168, 90, 40, rx=6))
        b.append(text(x + 45, 193, f"nodo {i}", 13, "middle", INK, 600, MONO))
    for i in range(1, 3):
        b.append(arrow(xs[i] + 92, 182, xs[i + 1] - 4, 182, MUTED, "ar", 1.3))
        b.append(arrow(xs[i + 1] - 4, 196, xs[i] + 92, 196, MUTED, "ar", 1.3))
    for x in xs[1:]:
        b.append(arrow(xs[0] + 92, 250, x + 45, 212, COB, "arc", 2))
    b.append(text(640, 186, "prev / next:", 12, "start", MUTED, 700))
    b.append(text(640, 203, "solo apuntan", 12, "start", MUTED))
    b.append(text(640, 245, "la lista posee", 12, "start", COB, 700))
    b.append(text(640, 262, "todos los nodos", 12, "start", COB))
    write("ownership-listas.svg", svg(W, H, "\n".join(b), "Grafos de ownership de la lista simple y de la lista doble"))


# Lección 4 ---------------------------------------------------------------------------
def mapa_l04():
    _mapa("mapa-l04.svg", "Mapa de la lección 4", [
        [("§1", "Cola y buffer circular"), ("§2", "TAD Diccionario"), ("§3", "Encadenamiento")],
        [("§4", "Hashing universal"), ("§5", "Direccionamiento abierto"), ("§6", "Redimensionar")],
        [("§7", "Funciones hash reales"), ("§8–9", "Paradigmas y lab"), ("§10–11", "Ejercicios")],
    ], "§4", "aleatorizar h: núcleo de la lección")


def buffer_circular():
    W, H = 820, 270
    cw, ch, x0 = 58, 44, 120
    b = []
    def fila(y, celdas, etiqueta, head, m):
        b.append(text(x0 - 14, y + ch / 2 + 5, etiqueta, 13, "end", MUTED))
        for j in range(m):
            x = x0 + j * cw
            v = celdas.get(j, "")
            libre = v == ""
            b.append(box(x, y, cw - 6, ch, fill="#ffffff" if libre else PALE,
                         stroke=LINE if libre else COB, rx=6, dash="4 3" if libre else None))
            b.append(text(x + (cw - 6) / 2, y + ch / 2 + 5, v, 14, "middle", INK, 600, MONO))
            b.append(text(x + (cw - 6) / 2, y - 6, str(j), 11, "middle", MUTED, 400, MONO))
        hx = x0 + head * cw + (cw - 6) / 2
        b.append(arrow(hx, y + ch + 26, hx, y + ch + 4, ACC, "ara", 1.6))
        b.append(text(hx, y + ch + 40, "head", 12, "middle", ACC, 700, MONO))
    fila(40, {2: "e0", 3: "e1", 0: "e2", 1: "e3"}, "m = 4, lleno", 2, 4)
    b.append(text(x0 + 4 * cw + 20, 58, "el elemento i está en (head + i) mod m", 13, "start", INK, 600))
    b.append(text(x0 + 4 * cw + 20, 78, "e2 y e3 dieron la vuelta a las celdas 0 y 1", 12, "start", MUTED))
    fila(170, {0: "e0", 1: "e1", 2: "e2", 3: "e3", 4: "e4"}, "m = 8, tras push", 0, 8)
    b.append(text(x0 + 8 * cw + 16, 188, "desenrollado:", 13, "start", INK, 600))
    b.append(text(x0 + 8 * cw + 16, 206, "e_i en la celda i", 12, "start", MUTED))
    b.append(text(x0 + 8 * cw + 16, 222, "head = 0", 12, "start", MUTED))
    write("buffer-circular.svg", svg(W, H, "\n".join(b), "Buffer circular lleno con head = 2 y el mismo contenido tras crecer"))


# Lección 5 ---------------------------------------------------------------------------
def mapa_l05():
    _mapa("mapa-l05.svg", "Mapa de la lección 5", [
        [("§1", "El problema"), ("§2", "Inserción"), ("§3", "Mergesort")],
        [("§4", "Cota inferior"), ("§5", "Quicksort"), ("§6", "Heapsort")],
        [("§7", "Ordenar en lineal"), ("§8–10", "Resumen y lab"), ("§11–12", "Ejercicios")],
    ], "§4", "Ω(n lg n): núcleo de la lección")


def arbol_mergesort():
    W, H = 840, 300
    b = []
    niveles = [(1, "n", "cn"), (2, "n/2", "cn"), (4, "n/4", "cn"), (8, "n/8", "cn")]
    x0, ancho, y0, dy = 40, 560, 30, 62
    for k, (cnt, etq, costo) in enumerate(niveles):
        y = y0 + k * dy
        w = ancho / cnt
        for i in range(cnt):
            x = x0 + i * w
            b.append(box(x + 4, y, w - 8, 36, fill=PALE, stroke=COB, rx=6))
            b.append(text(x + w / 2, y + 23, etq, 13 if cnt < 8 else 11, "middle", INK, 600, MONO))
            if k + 1 < len(niveles):
                for h in (0, 1):
                    cx = x0 + (2 * i + h) * (w / 2) + w / 4
                    b.append(f'<line x1="{x + w / 2}" y1="{y + 36}" x2="{cx}" y2="{y + dy}" stroke="{LINE}" stroke-width="1.3"/>')
        b.append(text(x0 + ancho + 30, y + 23, f"{cnt} × c·{etq} = {costo}", 13, "start", COB, 700, MONO))
    b.append(text(x0 + ancho / 2, y0 + 4 * dy + 10, "⋮  (lg n + 1 niveles)", 13, "middle", MUTED))
    b.append(text(x0 + ancho + 30, y0 + 4 * dy + 10, "total: cn(lg n + 1)", 14, "start", ACC, 700, MONO))
    write("arbol-mergesort.svg", svg(W, H, "\n".join(b), "Árbol de recursión de mergesort: cada nivel cuesta cn"))


# Lección 6 ---------------------------------------------------------------------------
def mapa_l06():
    _mapa("mapa-l06.svg", "Mapa de la lección 6", [
        [("§1", "Cola de prioridad"), ("§2", "Árbol en un arreglo"), ("§3", "sift_up / sift_down")],
        [("§4", "Heapify en O(n)"), ("§5", "Aplicaciones"), ("§6", "Paradigmas")],
        [("§7", "Guía del lab"), ("§8", "Comprensión"), ("§9", "Problem set")],
    ], "§4", "construir en lineal: núcleo de la lección")


def heap_arreglo():
    W, H = 760, 330
    vals = [1, 3, 2, 7, 4, 5, 6, 9, 8]
    b = []
    pos = {}
    niveles = [[0], [1, 2], [3, 4, 5, 6], [7, 8]]
    for d, idxs in enumerate(niveles):
        n_nivel = 2 ** d
        for i in idxs:
            t = i - (2 ** d - 1)
            x = 40 + (t + 0.5) * (680 / n_nivel)
            y = 30 + d * 58
            pos[i] = (x, y)
    for i in range(1, len(vals)):
        (x1, y1), (x2, y2) = pos[(i - 1) // 2], pos[i]
        b.append(f'<line x1="{x1}" y1="{y1 + 16}" x2="{x2}" y2="{y2 - 16}" stroke="{LINE}" stroke-width="1.5"/>')
    for i, v in enumerate(vals):
        x, y = pos[i]
        b.append(f'<circle cx="{x}" cy="{y}" r="17" fill="{COB if i == 0 else PALE}" stroke="{COB}" stroke-width="1.5"/>')
        b.append(text(x, y + 5, str(v), 14, "middle", "#ffffff" if i == 0 else INK, 700, MONO))
        b.append(text(x + 22, y - 10, str(i), 10, "start", MUTED, 400, MONO))
    y0, cw, x0 = 262, 60, 110
    b.append(text(x0 - 14, y0 + 25, "a =", 14, "end", INK, 600, MONO))
    for i, v in enumerate(vals):
        x = x0 + i * cw
        b.append(box(x, y0, cw - 6, 38, fill=COB if i == 0 else PALE, stroke=COB, rx=6))
        b.append(text(x + (cw - 6) / 2, y0 + 25, str(v), 14, "middle", "#ffffff" if i == 0 else INK, 700, MONO))
        b.append(text(x + (cw - 6) / 2, y0 + 54, str(i), 11, "middle", MUTED, 400, MONO))
    b.append(text(700, 210, "padre(i) = ⌊(i−1)/2⌋", 12, "end", COB, 700, MONO))
    b.append(text(700, 228, "hijos: 2i+1, 2i+2", 12, "end", COB, 700, MONO))
    write("heap-arreglo.svg", svg(W, H, "\n".join(b), "Un min-heap de 9 elementos como árbol y como arreglo"))


# Lección 7 ---------------------------------------------------------------------------
def mapa_l07():
    _mapa("mapa-l07.svg", "Mapa de la lección 7", [
        [("§1", "Diccionario ordenado"), ("§2", "BST"), ("§3", "Rotaciones")],
        [("§4", "AVL"), ("§5", "Tomar y devolver"), ("§6", "Otros balanceados")],
        [("§7", "Paradigmas"), ("§8", "Guía del lab"), ("§9–10", "Ejercicios")],
    ], "§4", "altura < 1.44 lg n: núcleo de la lección")


def _nodo_c(b, x, y, t, fill=PALE, color=INK):
    b.append(f'<circle cx="{x}" cy="{y}" r="18" fill="{fill}" stroke="{COB}" stroke-width="1.5"/>')
    b.append(text(x, y + 5, t, 14, "middle", color, 700, MONO))


def _tri(b, x, y, t, alto=46, fill="#ffffff"):
    b.append(f'<path d="M{x},{y} L{x - 24},{y + alto} L{x + 24},{y + alto} Z" fill="{fill}" stroke="{MUTED}" stroke-width="1.3"/>')
    b.append(text(x, y + alto - 10, t, 13, "middle", INK, 700, MONO))


def _arista(b, x1, y1, x2, y2):
    b.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{MUTED}" stroke-width="1.4"/>')


def rotacion():
    W, H = 760, 250
    b = []
    # izquierda: y con hijo x
    _arista(b, 180, 50, 120, 110); _arista(b, 180, 50, 240, 120)
    _arista(b, 120, 110, 80, 160); _arista(b, 120, 110, 160, 160)
    _nodo_c(b, 180, 50, "y"); _nodo_c(b, 120, 110, "x")
    _tri(b, 80, 140, "A"); _tri(b, 160, 140, "B"); _tri(b, 240, 120, "C")
    # derecha: x con hijo y
    _arista(b, 560, 50, 500, 120); _arista(b, 560, 50, 620, 110)
    _arista(b, 620, 110, 580, 160); _arista(b, 620, 110, 660, 160)
    _nodo_c(b, 560, 50, "x"); _nodo_c(b, 620, 110, "y")
    _tri(b, 500, 120, "A"); _tri(b, 580, 140, "B"); _tri(b, 660, 140, "C")
    b.append(arrow(310, 90, 440, 90, COB, "arc", 2))
    b.append(text(375, 80, "rotate_right(y)", 13, "middle", COB, 700, MONO))
    b.append(arrow(440, 130, 310, 130, ACC, "ara", 2))
    b.append(text(375, 150, "rotate_left(x)", 13, "middle", ACC, 700, MONO))
    b.append(text(380, 228, "in-order en ambos: A < x < B < y < C", 14, "middle", INK, 600))
    write("rotacion.svg", svg(W, H, "\n".join(b), "Rotación a la derecha y a la izquierda; el orden A x B y C se conserva"))


def avl_casos():
    W, H = 800, 520
    b = []
    b.append(text(20, 28, "Caso izquierda-izquierda: una rotación a la derecha en t", 14, "start", INK, 700))
    _arista(b, 170, 60, 110, 115); _arista(b, 170, 60, 230, 120)
    _arista(b, 110, 115, 70, 160); _arista(b, 110, 115, 150, 160)
    _nodo_c(b, 170, 60, "t", fill=ACC_PALE); _nodo_c(b, 110, 115, "x")
    _tri(b, 70, 135, "A", alto=70, fill=ACC_PALE); _tri(b, 150, 135, "B", alto=50); _tri(b, 230, 120, "C", alto=40)
    b.append(text(70, 222, "k+1", 12, "middle", ACC, 700, MONO)); b.append(text(150, 202, "≤ k+1", 12, "middle", MUTED, 400, MONO))
    b.append(text(230, 178, "k", 12, "middle", MUTED, 400, MONO))
    b.append(arrow(300, 120, 400, 120, COB, "arc", 2))
    _arista(b, 560, 60, 500, 115); _arista(b, 560, 60, 620, 115)
    _arista(b, 620, 115, 580, 160); _arista(b, 620, 115, 660, 160)
    _nodo_c(b, 560, 60, "x"); _nodo_c(b, 620, 115, "t")
    _tri(b, 500, 115, "A", alto=70, fill=ACC_PALE); _tri(b, 580, 140, "B", alto=50); _tri(b, 660, 140, "C", alto=40)
    b.append(text(700, 80, "balanceado", 13, "start", OK, 700))
    y0 = 270
    b.append(text(20, y0, "Caso izquierda-derecha: rotación doble (izquierda en x, derecha en t)", 14, "start", INK, 700))
    _arista(b, 170, y0 + 32, 110, y0 + 87); _arista(b, 170, y0 + 32, 230, y0 + 92)
    _arista(b, 110, y0 + 87, 70, y0 + 132); _arista(b, 110, y0 + 87, 150, y0 + 132)
    _nodo_c(b, 170, y0 + 32, "t", fill=ACC_PALE); _nodo_c(b, 110, y0 + 87, "x"); _nodo_c(b, 150, y0 + 132, "y", fill=ACC_PALE)
    _tri(b, 70, y0 + 107, "A", alto=45); _tri(b, 230, y0 + 92, "C", alto=45)
    _arista(b, 150, y0 + 150, 125, y0 + 180); _arista(b, 150, y0 + 150, 175, y0 + 180)
    _tri(b, 125, y0 + 180, "B₁", alto=40); _tri(b, 175, y0 + 180, "B₂", alto=40)
    b.append(arrow(300, y0 + 110, 400, y0 + 110, COB, "arc", 2))
    _arista(b, 580, y0 + 40, 510, y0 + 100); _arista(b, 580, y0 + 40, 650, y0 + 100)
    _arista(b, 510, y0 + 100, 480, y0 + 140); _arista(b, 510, y0 + 100, 540, y0 + 140)
    _arista(b, 650, y0 + 100, 620, y0 + 140); _arista(b, 650, y0 + 100, 680, y0 + 140)
    _nodo_c(b, 580, y0 + 40, "y", fill=ACC_PALE); _nodo_c(b, 510, y0 + 100, "x"); _nodo_c(b, 650, y0 + 100, "t")
    _tri(b, 480, y0 + 135, "A", alto=45); _tri(b, 540, y0 + 135, "B₁", alto=40)
    _tri(b, 620, y0 + 135, "B₂", alto=40); _tri(b, 680, y0 + 135, "C", alto=45)
    b.append(text(720, y0 + 60, "balanceado", 13, "start", OK, 700))
    write("avl-casos.svg", svg(W, H, "\n".join(b), "Los dos casos de rebalanceo AVL cuando el lado izquierdo es más alto"))


# Lección 8 ---------------------------------------------------------------------------
def mapa_l08():
    _mapa("mapa-l08.svg", "Mapa de la lección 8", [
        [("§1", "Estadísticos de orden"), ("§2", "Teorema de aumentación"), ("§3", "select y rank")],
        [("§4", "Treaps"), ("§5", "Otras aumentaciones"), ("§6", "Paradigmas")],
        [("§7", "Guía del lab"), ("§8", "Comprensión"), ("§9", "Problem set")],
    ], "§2", "f(x) desde los hijos: núcleo de la lección")


def select_size():
    W, H = 900, 330
    b = []
    pos = {}
    def construir(lo, hi, d, x0, x1):
        if lo >= hi: return None
        m = (lo + hi) // 2
        x, y = (x0 + x1) / 2, 40 + d * 70
        pos[m] = (x, y, hi - lo)
        l = construir(lo, m, d + 1, x0, x)
        r = construir(m + 1, hi, d + 1, x, x1)
        for c in (l, r):
            if c is not None:
                cx, cy, _ = pos[c]
                b.append(f'<line x1="{x}" y1="{y + 18}" x2="{cx}" y2="{cy - 18}" stroke="{LINE}" stroke-width="1.4"/>')
        return m
    construir(0, 15, 0, 20, 660)
    camino = {7: "k=10, r=7 → der, k=2", 11: "k=2, r=3 → izq", 9: "k=2, r=1 → der, k=0", 10: "k=0, r=0 → ¡100!"}
    for m, (x, y, t) in pos.items():
        on = m in camino
        b.append(f'<circle cx="{x}" cy="{y}" r="19" fill="{ACC_PALE if on else PALE}" stroke="{ACC if on else COB}" stroke-width="{2 if on else 1.3}"/>')
        b.append(text(x, y + 4, str(10 * m), 12, "middle", INK, 700, MONO))
        b.append(text(x, y + 33, f"s={t}", 10, "middle", MUTED, 400, MONO))
    ys = 60
    for m in (7, 11, 9, 10):
        b.append(text(680, ys, camino[m], 12, "start", ACC, 700, MONO))
        ys += 22
    b.append(text(680, ys + 10, "s = size del subárbol", 12, "start", MUTED))
    write("select-size.svg", svg(W, H, "\n".join(b), "select(10) en un árbol de 15 claves, guiado por los tamaños de los subárboles"))


# Lección 9 ---------------------------------------------------------------------------
def mapa_l09():
    _mapa("mapa-l09.svg", "Mapa de la lección 9", [
        [("§1", "Grafos y representación"), ("§2", "BFS"), ("§3", "DFS")],
        [("§4", "Orden topológico"), ("§5", "SCC"), ("§6", "Paradigmas")],
        [("§7", "Guía del lab"), ("§8", "Comprensión"), ("§9", "Problem set")],
    ], "§3", "DFS y sus tiempos: núcleo de la lección")


def bfs_dfs():
    W, H = 820, 330
    b = []
    aristas = [(0, 1), (0, 2), (1, 3), (2, 3), (2, 4), (3, 5), (4, 5), (4, 6)]
    arbol_dfs = {(0, 1), (1, 3), (3, 5), (0, 2), (2, 4), (4, 6)}
    capas = {0: 0, 1: 1, 2: 1, 3: 2, 4: 2, 5: 3, 6: 3}
    pos = {0: (70, 150), 1: (190, 90), 2: (190, 210), 3: (310, 90), 4: (310, 210), 5: (430, 120), 6: (430, 260)}
    for c in range(4):
        x = 70 + 120 * c
        b.append(box(x - 40, 40, 80, 260, fill="#f8fafc" if c % 2 else "#ffffff", stroke=LINE, rx=10))
        b.append(text(x, 32, f"capa {c}", 12, "middle", MUTED, 600))
    for (u, v) in aristas:
        (x1, y1), (x2, y2) = pos[u], pos[v]
        dx, dy = x2 - x1, y2 - y1
        L = (dx * dx + dy * dy) ** 0.5
        ex, ey = x2 - dx / L * 22, y2 - dy / L * 22
        sx, sy = x1 + dx / L * 20, y1 + dy / L * 20
        on = (u, v) in arbol_dfs
        b.append(arrow(sx, sy, ex, ey, ACC if on else MUTED, "ara" if on else "ar", 2.4 if on else 1.3, None if on else "5 4"))
    for v, (x, y) in pos.items():
        b.append(f'<circle cx="{x}" cy="{y}" r="19" fill="{PALE}" stroke="{COB}" stroke-width="1.6"/>')
        b.append(text(x, y + 5, str(v), 15, "middle", INK, 700, MONO))
    b.append(text(520, 80, "BFS: distancia = capa", 14, "start", INK, 700))
    b.append(text(520, 102, "0 1 1 2 2 3 3", 13, "start", COB, 700, MONO))
    b.append(text(520, 150, "DFS: aristas de árbol", 14, "start", ACC, 700))
    b.append(text(520, 172, "preorden 0 1 3 5 2 4 6", 13, "start", ACC, 700, MONO))
    b.append(text(520, 220, "punteadas: no son de árbol", 12, "start", MUTED))
    b.append(text(520, 238, "(2→3 y 4→5 son cruzadas)", 12, "start", MUTED))
    write("bfs-dfs.svg", svg(W, H, "\n".join(b), "El grafo del ejemplo con sus capas BFS y su árbol DFS"))


# Lección 10 --------------------------------------------------------------------------
def mapa_l10():
    _mapa("mapa-l10.svg", "Mapa de la lección 10", [
        [("§1", "El problema"), ("§2", "Relajación"), ("§3", "DAG")],
        [("§4", "Bellman-Ford"), ("§5", "Dijkstra"), ("§6", "Floyd-Warshall")],
        [("§7", "Infinito y paradigmas"), ("§8–9", "¿Cuál uso? y lab"), ("§10–11", "Ejercicios")],
    ], "§2", "relajar en el orden correcto: núcleo")


def relajacion():
    W, H = 760, 200
    b = []
    def par(x0, du, dv, titulo, resalta):
        b.append(text(x0 + 130, 30, titulo, 14, "middle", INK, 700))
        b.append(f'<circle cx="{x0 + 40}" cy="110" r="26" fill="{PALE}" stroke="{COB}" stroke-width="1.6"/>')
        b.append(text(x0 + 40, 116, "u", 16, "middle", INK, 700, MONO))
        b.append(text(x0 + 40, 160, f"d = {du}", 13, "middle", COB, 700, MONO))
        b.append(f'<circle cx="{x0 + 220}" cy="110" r="26" fill="{ACC_PALE if resalta else PALE}" stroke="{ACC if resalta else COB}" stroke-width="1.6"/>')
        b.append(text(x0 + 220, 116, "v", 16, "middle", INK, 700, MONO))
        b.append(text(x0 + 220, 160, f"d = {dv}", 13, "middle", ACC if resalta else COB, 700, MONO))
        b.append(arrow(x0 + 68, 110, x0 + 190, 110, INK, "ar", 1.8))
        b.append(text(x0 + 130, 100, "w = 2", 13, "middle", INK, 600, MONO))
    par(20, 5, 9, "antes", False)
    b.append(arrow(320, 110, 420, 110, COB, "arc", 2))
    b.append(text(370, 95, "relax(u, v)", 12, "middle", COB, 700, MONO))
    b.append(text(370, 140, "5 + 2 < 9", 12, "middle", MUTED, 400, MONO))
    par(450, 5, 7, "después", True)
    write("relajacion.svg", svg(W, H, "\n".join(b), "Relajar la arista u→v baja d[v] de 9 a 7"))


# Lección 11 --------------------------------------------------------------------------
def mapa_l11():
    _mapa("mapa-l11.svg", "Mapa de la lección 11", [
        [("§1", "Conjuntos disjuntos"), ("§2", "Bosque, tamaño, compresión"), ("§3", "Propiedad del corte")],
        [("§4", "Kruskal"), ("§5", "Prim"), ("§6", "find muta")],
        [("§7", "Guía del lab"), ("§8", "Comprensión"), ("§9", "Problem set")],
    ], "§3", "intercambio: núcleo de la lección")


def corte():
    W, H = 760, 300
    b = []
    b.append(box(30, 40, 300, 230, fill="#f8fafc", stroke=LINE, rx=16))
    b.append(box(430, 40, 300, 230, fill="#fff7ed", stroke=LINE, rx=16))
    b.append(text(180, 30, "S", 15, "middle", COB, 700))
    b.append(text(580, 30, "V \\ S", 15, "middle", ACC, 700))
    pos = {"u": (250, 110), "x": (150, 200), "a": (100, 100), "v": (520, 120), "y": (600, 210), "b": (660, 100)}
    arbol = [("a", "u"), ("a", "x"), ("x", "y"), ("y", "v"), ("v", "b")]
    for p_, q_ in arbol:
        (x1, y1), (x2, y2) = pos[p_], pos[q_]
        cruza = (p_, q_) == ("x", "y")
        b.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{ACC if cruza else COB}" stroke-width="{3 if cruza else 2.2}"/>')
    (x1, y1), (x2, y2) = pos["u"], pos["v"]
    b.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{OK}" stroke-width="3" stroke-dasharray="7 5"/>')
    for n_, (x, y) in pos.items():
        b.append(f'<circle cx="{x}" cy="{y}" r="17" fill="#ffffff" stroke="{COB}" stroke-width="1.6"/>')
        b.append(text(x, y + 5, n_, 14, "middle", INK, 700, MONO))
    b.append(text(385, 100, "e (ligera)", 13, "middle", OK, 700))
    b.append(text(375, 225, "e′ ∈ T", 13, "middle", ACC, 700))
    b.append(text(380, 288, "T′ = T − e′ + e   y   w(e) ≤ w(e′)  ⇒  w(T′) ≤ w(T)", 13, "middle", INK, 600))
    write("corte.svg", svg(W, H, "\n".join(b), "Propiedad del corte: intercambiar e′ por la arista ligera e"))


# Lección 12 --------------------------------------------------------------------------
def mapa_l12():
    _mapa("mapa-l12.svg", "Mapa de la lección 12", [
        [("§1", "Recursión que repite"), ("§2", "SRTBOT"), ("§3", "LCS")],
        [("§4", "Distancia de edición"), ("§5", "Mochila y monedas"), ("§6", "LIS")],
        [("§7", "Subproblemas típicos"), ("§8", "UTF-8"), ("§9–11", "Lab y ejercicios")],
    ], "§2", "encontrar los subproblemas: núcleo")


def tabla_lcs():
    a, b = "ABCBDAB", "BDCABA"
    n, m = len(a), len(b)
    L = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        for j in range(m - 1, -1, -1):
            L[i][j] = 1 + L[i + 1][j + 1] if a[i] == b[j] else max(L[i + 1][j], L[i][j + 1])
    camino, i, j = set(), 0, 0
    tomadas = set()
    while i < n and j < m:
        camino.add((i, j))
        if a[i] == b[j]:
            tomadas.add((i, j)); i += 1; j += 1
        elif L[i + 1][j] >= L[i][j + 1]:
            i += 1
        else:
            j += 1
    camino.add((i, j))
    cw, ch, x0, y0 = 52, 34, 90, 60
    W, H = x0 + (m + 1) * cw + 230, y0 + (n + 1) * ch + 40
    b_ = []
    for j, c in enumerate(b + " "):
        b_.append(text(x0 + j * cw + cw / 2, y0 - 12, c if c != " " else "∅", 14, "middle", COB, 700, MONO))
    for i, c in enumerate(a + " "):
        b_.append(text(x0 - 16, y0 + i * ch + ch / 2 + 5, c if c != " " else "∅", 14, "middle", COB, 700, MONO))
    for i in range(n + 1):
        for j in range(m + 1):
            x, y = x0 + j * cw, y0 + i * ch
            en = (i, j) in camino
            tom = (i, j) in tomadas
            b_.append(box(x + 2, y + 2, cw - 4, ch - 4, fill=ACC_PALE if tom else (PALE if en else "#ffffff"),
                          stroke=ACC if tom else (COB if en else LINE), rx=4, sw=1.2))
            b_.append(text(x + cw / 2, y + ch / 2 + 5, str(L[i][j]), 13, "middle", INK, 700 if en else 400, MONO))
    xt = x0 + (m + 1) * cw + 24
    b_.append(text(xt, y0 + 20, "L[i][j] = LCS de a[i..] y b[j..]", 13, "start", INK, 600))
    b_.append(text(xt, y0 + 44, "a = ABCBDAB (filas)", 12, "start", MUTED, 400, MONO))
    b_.append(text(xt, y0 + 62, "b = BDCABA (columnas)", 12, "start", MUTED, 400, MONO))
    b_.append(text(xt, y0 + 96, "camino de reconstrucción", 12, "start", COB, 700))
    b_.append(text(xt, y0 + 116, "naranja: a[i] = b[j], se toma", 12, "start", ACC, 700))
    b_.append(text(xt, y0 + 140, "resultado: BDAB", 13, "start", ACC, 700, MONO))
    write("tabla-lcs.svg", svg(W, H, "\n".join(b_), "Tabla de LCS de ABCBDAB y BDCABA con el camino de reconstrucción"))


# Lección 13 --------------------------------------------------------------------------
def mapa_l13():
    _mapa("mapa-l13.svg", "Mapa de la lección 13", [
        [("§1", "Tries"), ("§2", "Consultas de rango"), ("§3", "Fenwick")],
        [("§4", "Árbol de segmentos"), ("§5", "¿Cuál uso?"), ("§6", "Paradigmas")],
        [("§7", "Guía del lab"), ("§8", "Comprensión"), ("§9", "Problem set")],
    ], "§3", "índices en binario: núcleo de la lección")


def trie_diagrama():
    W, H = 820, 330
    b = []
    palabras = ["casa", "cama", "camion", "cas", "perro"]
    hijos, fin = {"": []}, set(palabras)
    for w in palabras:
        for k in range(1, len(w) + 1):
            p_, q_ = w[:k - 1], w[:k]
            hijos.setdefault(p_, [])
            if q_ not in hijos[p_]: hijos[p_].append(q_)
            hijos.setdefault(q_, [])
    for k in hijos: hijos[k].sort()
    pos, sig = {}, [40]
    def colocar(x, d):
        if not hijos[x]:
            pos[x] = (sig[0], 30 + d * 44); sig[0] += 70
            return
        for h in hijos[x]: colocar(h, d + 1)
        xs = [pos[h][0] for h in hijos[x]]
        pos[x] = ((min(xs) + max(xs)) / 2, 30 + d * 44)
    colocar("", 0)
    cuenta = {x: sum(1 for w in palabras if w.startswith(x)) for x in hijos}
    for x, hs in hijos.items():
        for h in hs:
            (x1, y1), (x2, y2) = pos[x], pos[h]
            b.append(f'<line x1="{x1}" y1="{y1 + 13}" x2="{x2}" y2="{y2 - 13}" stroke="{LINE}" stroke-width="1.5"/>')
            b.append(text((x1 + x2) / 2 - 8, (y1 + y2) / 2 + 4, h[-1], 13, "end", COB, 700, MONO))
    for x, (px, py) in pos.items():
        f = x in fin
        b.append(f'<circle cx="{px}" cy="{py}" r="13" fill="{ACC_PALE if f else PALE}" stroke="{ACC if f else COB}" stroke-width="{2 if f else 1.3}"/>')
        b.append(text(px, py + 4, str(cuenta[x]), 11, "middle", INK, 700, MONO))
    b.append(text(560, 60, "número en el nodo: cuenta", 13, "start", INK, 600))
    b.append(text(560, 80, "(palabras con ese prefijo)", 12, "start", MUTED))
    b.append(text(560, 110, "naranja: fin de palabra", 13, "start", ACC, 700))
    b.append(text(560, 140, "count_with_prefix(\"ca\") = 4", 12, "start", COB, 700, MONO))
    write("trie.svg", svg(W, H, "\n".join(b), "Un trie con casa, cama, camion, cas y perro, con la cuenta en cada nodo"))


def fenwick_diagrama():
    W, H = 900, 290
    n, cw, x0, y0 = 16, 44, 40, 40
    b = []
    for j in range(1, n + 1):
        lb = j & -j
        nivel = lb.bit_length() - 1
        y = y0 + (4 - nivel) * 36
        x1, x2 = x0 + (j - lb) * cw, x0 + j * cw
        b.append(box(x1 + 2, y, x2 - x1 - 4, 26, fill=PALE, stroke=COB, rx=6, sw=1.2))
        b.append(text((x1 + x2) / 2, y + 18, f"bit[{j}]" if lb > 1 else str(j), 11, "middle", INK, 600, MONO))
    for i in range(n):
        b.append(text(x0 + i * cw + cw / 2, y0 + 5 * 36 + 18, str(i + 1), 11, "middle", MUTED, 400, MONO))
    b.append(text(x0, y0 + 5 * 36 + 40, "posición interna (1..16); cada celda cubre (j − lowbit(j), j]", 12, "start", MUTED))
    b.append(text(x0 + n * cw + 16, y0 + 14, "lowbit 16", 11, "start", COB))
    b.append(text(x0 + n * cw + 16, y0 + 4 * 36 + 14, "lowbit 1", 11, "start", COB))
    write("fenwick.svg", svg(W, H, "\n".join(b), "Los bloques de un árbol de Fenwick de 16 posiciones"))



# Gráficas de funciones (L00 §1 y L01 §3.3.8) ------------------------------------------
def _ejes(b, L, R, T, B, sx, sy, xticks, yticks, ylabel):
    """Ejes con marcas enteras y cuadrícula tenue."""
    for v in yticks:
        b.append(f'<line x1="{L}" y1="{sy(v):.1f}" x2="{R}" y2="{sy(v):.1f}" stroke="#eef0f4" stroke-width="1"/>')
        b.append(text(L - 8, sy(v) + 4, v, 12, "end", MUTED))
    for v in xticks:
        b.append(f'<line x1="{sx(v):.1f}" y1="{B}" x2="{sx(v):.1f}" y2="{B + 5}" stroke="{MUTED}" stroke-width="1"/>')
        b.append(text(sx(v), B + 20, v, 12, "middle", MUTED))
    b.append(arrow(L, B, R + 14, B))
    b.append(arrow(L, B, L, T - 14))
    b.append(text(R + 22, B + 5, "n", 15, "start", INK, 600, italic=True))
    b.append(text(L - 10, T - 20, ylabel, 13, "end", MUTED))


def _curva(fn, x0, x1, sx, sy, paso=0.05):
    pts, t = [], x0
    while t <= x1 + 1e-9:
        pts.append(f"{sx(t):.1f},{sy(fn(t)):.1f}")
        t += paso
    return " ".join(pts)


def testigos_3n5():
    W, H = 800, 420
    L, R, T, B = 70, 600, 40, 360
    XM, YM = 12, 100
    sx = lambda t: L + (R - L) * t / XM
    sy = lambda v: B - (B - T) * v / YM
    b = []
    b.append(f'<rect x="{sx(5):.1f}" y="{T}" width="{R - sx(5):.1f}" height="{B - T}" fill="{PALE}"/>')
    _ejes(b, L, R, T, B, sx, sy, range(0, XM + 1), range(0, YM + 1, 20), "pasos")
    b.append(f'<line x1="{sx(5):.1f}" y1="{T}" x2="{sx(5):.1f}" y2="{B}" stroke="{COB}" stroke-width="1.3" stroke-dasharray="5 4"/>')
    b.append(text(sx(5), T - 8, "n₀ = 5 (para c = 4)", 13, "middle", COB, 700))
    b.append(f'<polyline points="{_curva(lambda t: 8 * t, 0, 12, sx, sy)}" fill="none" stroke="{OK}" stroke-width="2.4"/>')
    b.append(f'<polyline points="{_curva(lambda t: 4 * t, 0, 12, sx, sy)}" fill="none" stroke="{COB}" stroke-width="2.4"/>')
    b.append(f'<polyline points="{_curva(lambda t: 3 * t + 5, 0, 12, sx, sy)}" fill="none" stroke="{ACC}" stroke-width="3"/>')
    for (x, y, col) in [(5, 20, COB), (1, 8, OK)]:
        b.append(f'<circle cx="{sx(x):.1f}" cy="{sy(y):.1f}" r="5" fill="#ffffff" stroke="{col}" stroke-width="2.2"/>')
    b.append(text(sx(5) + 10, sy(20) + 22, "se cruzan en n = 5", 12, "start", COB))
    b.append(f'<line x1="{sx(1):.1f}" y1="{sy(8) - 6:.1f}" x2="{sx(0.9):.1f}" y2="{sy(31):.1f}" stroke="{OK}" stroke-width="1"/>')
    b.append(text(sx(0.25), sy(40), "8n tapa a 3n + 5", 12, "start", OK, 600))
    b.append(text(sx(0.25), sy(40) + 15, "desde n = 1", 12, "start", OK, 600))
    b.append(text(sx(3), sy(82), "antes de n = 5, 4n queda", 12, "middle", MUTED))
    b.append(text(sx(3), sy(82) + 15, "por debajo de 3n + 5", 12, "middle", MUTED))
    # leyenda
    lx, ly = R + 22, T + 30
    for i, (t, col, sw) in enumerate([("3n + 5", ACC, 3), ("8n  (c = 8)", OK, 2.4), ("4n  (c = 4)", COB, 2.4)]):
        y = ly + 26 * i
        b.append(f'<line x1="{lx}" y1="{y}" x2="{lx + 24}" y2="{y}" stroke="{col}" stroke-width="{sw}"/>')
        b.append(text(lx + 30, y + 4, t, 13, "start", col, 700))
    b.append(text(lx, ly + 100, "pocas copias:", 12, "start", MUTED))
    b.append(text(lx, ly + 116, "hay que esperar", 12, "start", MUTED))
    b.append(text(lx, ly + 140, "muchas copias:", 12, "start", MUTED))
    b.append(text(lx, ly + 156, "tapa desde n = 1", 12, "start", MUTED))
    write("testigos-3n5.svg", svg(W, H, "\n".join(b),
          "3n+5 contra 4n y 8n: 4n la tapa desde n = 5 y 8n desde n = 1"))


def n2_vs_cn():
    W, H = 800, 420
    L, R, T, B = 70, 600, 40, 360
    XM, YM = 14, 160
    sx = lambda t: L + (R - L) * t / XM
    sy = lambda v: B - (B - T) * v / YM
    b = []
    _ejes(b, L, R, T, B, sx, sy, range(0, XM + 1, 2), range(0, YM + 1, 40), "pasos")
    tonos = [(2, "#93a8f4"), (5, "#4f6fe8"), (10, COB)]
    for c, col in tonos:
        b.append(f'<polyline points="{_curva(lambda t, c=c: c * t, 0, XM, sx, sy)}" fill="none" stroke="{col}" stroke-width="2.2"/>')
    b.append(f'<polyline points="{_curva(lambda t: t * t, 0, math.sqrt(YM), sx, sy)}" fill="none" stroke="{ACC}" stroke-width="3"/>')
    for c, col in tonos:
        b.append(f'<line x1="{sx(c):.1f}" y1="{sy(c * c):.1f}" x2="{sx(c):.1f}" y2="{B}" stroke="{col}" stroke-width="1" stroke-dasharray="4 4"/>')
        b.append(f'<circle cx="{sx(c):.1f}" cy="{sy(c * c):.1f}" r="5" fill="#ffffff" stroke="{col}" stroke-width="2.2"/>')
        b.append(text(sx(c), B + 40, f"n = {c}", 12, "middle", col, 700))
    lx, ly = R + 22, T + 30
    items = [("n²", ACC, 3)] + [(f"{c}n", col, 2.2) for c, col in reversed(tonos)]
    for i, (t, col, sw) in enumerate(items):
        y = ly + 26 * i
        b.append(f'<line x1="{lx}" y1="{y}" x2="{lx + 24}" y2="{y}" stroke="{col}" stroke-width="{sw}"/>')
        b.append(text(lx + 30, y + 4, t, 13, "start", col, 700))
    for j, t in enumerate(["n² cruza a c·n", "justo en n = c,", "y después queda", "arriba para siempre.", "",
                           "Subir c solo mueve", "el cruce a la derecha."]):
        b.append(text(lx, ly + 126 + 16 * j, t, 12, "start", MUTED))
    write("n2-vs-cn.svg", svg(W, H, "\n".join(b),
          "n² contra 2n, 5n y 10n: la parábola cruza cada recta en n = c y queda arriba"))

if __name__ == "__main__":
    ciclo_curso(); memoria(); mapa_l01(); big_o(); crecimiento(); costos_push(); contraccion(); seno(); grafos(); testigos_3n5(); n2_vs_cn()
    mapa_l02(); corrimiento()
    mapa_l03(); lista_simple(); ownership_listas()
    mapa_l04(); buffer_circular()
    mapa_l05(); arbol_mergesort()
    mapa_l06(); heap_arreglo()
    mapa_l07(); rotacion(); avl_casos()
    mapa_l08(); select_size()
    mapa_l09(); bfs_dfs()
    mapa_l10(); relajacion()
    mapa_l11(); corte()
    mapa_l12(); tabla_lcs()
    mapa_l13(); trie_diagrama(); fenwick_diagrama()
