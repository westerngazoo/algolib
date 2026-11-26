#!/usr/bin/env python3
"""Genera el sitio HTML del curso a partir de las notas Markdown.

Uso:   python3 notas/herramientas/build_html.py
Salida: notas/html/{index,L00,L01,L01-guion}.html  (autocontenidas: SVG incrustados)

Requiere pandoc y beautifulsoup4. Las fórmulas se renderizan en el navegador con
MathJax (SVG, sin fuentes externas), cargado desde cdnjs.
"""
import html
import pathlib
import re
import subprocess

from bs4 import BeautifulSoup, NavigableString

NOTAS = pathlib.Path(__file__).resolve().parent.parent
OUT = NOTAS / "html"
OUT.mkdir(exist_ok=True)

CURSO = "Estructuras de Datos y Algoritmos"

PAGES = [  # (archivo md, salida, etiqueta corta, descripción)
    ("L00-prerrequisitos.md", "L00.html", "L00 · Prerrequisitos",
     "Repaso de lógica, inducción, logaritmos, sumas, recurrencias, probabilidad, memoria, C, C++ y Rust."),
    ("L01-modelo-asintotica-pilas.md", "L01.html", "L01 · Notas",
     "Modelo de cómputo, notación asintótica con taller de demostraciones, TAD Pila y análisis amortizado."),
    ("L01-guion-profesor.md", "L01-guion.html", "L01 · Guion del profesor",
     "Lo que se dice, se escribe en el pizarrón y se pregunta en 3 clases y una recitación. Incluye soluciones."),
]

# Las páginas cuya fuente .md no existe (p. ej. el guion del profesor, que no va en el
# repo público) se omiten, y la navegación no las enlaza.
PAGES = [p for p in PAGES if (NOTAS / p[0]).exists()]

CALLOUTS = {  # emoji -> (clase, etiqueta)
    "🗣️": ("dice", "Dices"),
    "❓": ("pregunta", "Pregunta a la clase"),
    "💡": ("nota", "Nota para el profesor"),
    "⚠️": ("atencion", "Atención"),
}

CSS = r"""
/* VOICE: apuntes de curso universitario, sobrios y precisos; el guion habla en voz de profesor.
   IDEA: cuaderno de clase técnico con pizarrón: hoja limpia, recuadros de pizarra verde para lo que se escribe.
   PICTURE: diagramas SVG propios de cada lección (cotas, arreglos, costos), sobre placa blanca.
   COLOUR and TYPE: tira cobalto #1240e0, hoja blanca, zonas #eef2ff, acento naranja quemado; Space Grotesk + Inter + JetBrains Mono.
   LAYOUT: tira de título a todo el ancho, fila de secciones, columna de lectura de 780px; figuras y tablas hasta 900px. */
:root{color-scheme:light dark;
 --bg:light-dark(#ffffff,#0b0e17);--ink:light-dark(#111827,#e6e9f2);--muted:light-dark(#5b6270,#9aa3b5);
 --cob:#1240e0;--cob-ink:light-dark(#1240e0,#8fa8ff);--pale:light-dark(#eef2ff,#141b33);--line:light-dark(#dfe4f2,#252c44);
 --acc:light-dark(#c2410c,#fb923c);--acc-pale:light-dark(#fff1e6,#2a1a10);--ok:light-dark(#15803d,#4ade80);
 --chalk-bg:#1f3a2e;--chalk:#eef3e8;--chalk-dim:#a9c2b2;--wood:#7a5a33}
:root[data-theme=light]{color-scheme:light}:root[data-theme=dark]{color-scheme:dark}
*{box-sizing:border-box;min-width:0}
html{scroll-padding-top:72px}
body{margin:0;background:var(--bg);color:var(--ink);font:17px/1.65 Inter,system-ui,sans-serif;overflow-x:clip}
a{color:var(--cob-ink)}
.strip{background:var(--cob);color:#fff;padding:clamp(22px,3.5vw,44px) max(5vw,16px) clamp(20px,3vw,34px)}
.strip .curso{font:600 14px/1.3 'Space Grotesk',sans-serif;opacity:.85;margin:0 0 10px}
.strip h1{font:700 clamp(30px,4.6vw,48px)/1.06 'Space Grotesk',sans-serif;margin:0;text-wrap:balance;max-width:24ch}
.strip .desc{margin:14px 0 0;max-width:62ch;opacity:.92;font-size:16px}
.pages{display:flex;flex-wrap:wrap;gap:8px;margin-top:18px}
.pages a{color:#fff;text-decoration:none;border:1px solid rgba(255,255,255,.45);border-radius:999px;padding:6px 14px;font-size:14px;line-height:1.3}
.pages a[aria-current]{background:#fff;color:var(--cob)}
.pages a:hover{background:rgba(255,255,255,.16)}
nav.toc{border-bottom:1px solid var(--line);background:var(--bg);z-index:5}
nav.toc .scroll{display:flex;gap:4px;padding:8px max(5vw,16px);white-space:nowrap}
nav.toc a{flex:none;text-decoration:none;color:var(--muted);font-size:14px;padding:6px 10px;border-radius:8px}
nav.toc a:hover{background:var(--pale);color:var(--ink)}
@media(min-width:900px){nav.toc{position:sticky;top:0}}
main{max-width:820px;margin:0 auto;padding:28px max(4vw,16px) 80px}
main h1{font:700 32px/1.1 'Space Grotesk',sans-serif;margin:56px 0 12px;padding-top:18px;border-top:4px solid var(--cob);color:var(--cob-ink)}
main h2{font:700 26px/1.2 'Space Grotesk',sans-serif;margin:48px 0 10px;text-wrap:balance}
main h3{font:600 21px/1.25 'Space Grotesk',sans-serif;margin:36px 0 8px}
main h4{font:600 18px/1.3 'Space Grotesk',sans-serif;margin:28px 0 6px}
.tiempo{display:inline-block;vertical-align:middle;margin-left:8px;font:500 13px/1 Inter,sans-serif;color:var(--muted);border:1px solid var(--line);border-radius:999px;padding:4px 9px}
p,li{max-width:70ch}
hr{border:0;border-top:1px solid var(--line);margin:40px 0}
blockquote{margin:16px 0;padding:12px 18px;border-left:4px solid var(--cob);background:var(--pale);border-radius:0 10px 10px 0}
blockquote p{margin:6px 0}
code{font-family:'JetBrains Mono',ui-monospace,monospace;font-size:.88em;background:var(--pale);padding:1px 5px;border-radius:5px;overflow-wrap:anywhere}
pre{position:relative;overflow-x:auto;background:light-dark(#f6f7fb,#11162a);border:1px solid var(--line);border-radius:10px;padding:14px 16px;font:14px/1.55 'JetBrains Mono',ui-monospace,monospace}
pre code{background:none;padding:0;font-size:inherit;overflow-wrap:normal}
.copy{position:absolute;top:8px;right:8px;font:600 12px Inter,sans-serif;border:1px solid var(--line);background:var(--bg);color:var(--muted);border-radius:7px;padding:6px 10px;min-height:32px;cursor:pointer}
.copy:focus-visible,a:focus-visible,summary:focus-visible{outline:3px solid var(--acc);outline-offset:2px}
.scroll{overflow-x:auto}
table{border-collapse:collapse;margin:16px 0;font-size:15px;width:100%}
th,td{border-bottom:1px solid var(--line);padding:8px 10px;text-align:left;vertical-align:top}
th{font-weight:600;background:var(--pale)}
figure{margin:26px 0;padding:0}
figure svg{display:block;width:100%;height:auto;border:1px solid var(--line);border-radius:12px}
figcaption{color:var(--muted);font-size:14px;margin-top:8px}
details{margin:12px 0;border:1px solid var(--line);border-radius:10px;padding:10px 14px;background:var(--bg)}
summary{cursor:pointer;font-weight:600;color:var(--cob-ink);min-height:28px}
.callout{margin:16px 0;padding:12px 16px 12px 16px;border-radius:10px;border:1px solid var(--line)}
.callout>.lbl{display:block;font:700 12px/1.2 Inter,sans-serif;letter-spacing:.02em;margin-bottom:4px}
.dice{background:light-dark(#f8fafc,#121726);border-left:5px solid var(--ink)}
.dice>.lbl{color:var(--ink)}
.pregunta{background:var(--pale);border-left:5px solid var(--cob)}
.pregunta>.lbl{color:var(--cob-ink)}
.pregunta+blockquote{margin-top:-10px;border-left-color:var(--ok);background:light-dark(#f0fdf4,#0f1f16)}
.pregunta+blockquote::before{content:"Respuesta esperada";display:block;font:700 12px Inter,sans-serif;color:var(--ok);margin-bottom:2px}
.nota{background:light-dark(#fffbea,#211d0c);border-left:5px solid #ca8a04}
.nota>.lbl{color:light-dark(#a16207,#facc15)}
.atencion{background:var(--acc-pale);border-left:5px solid var(--acc)}
.atencion>.lbl{color:var(--acc)}
.ejercicio{display:inline-block;font:700 12px Inter,sans-serif;color:#fff;background:var(--acc);border-radius:6px;padding:2px 8px;margin-right:6px;vertical-align:2px}
.moneda{display:inline-block;width:.9em;height:.9em;border-radius:50%;background:radial-gradient(circle at 35% 35%,#fde68a,#d97706);vertical-align:-1px;margin-right:4px}
.pizarra-label{display:table!important;margin:22px 0 0;font:600 13px/1.3 Inter,sans-serif;color:var(--chalk-dim);background:var(--chalk-bg);display:inline-block;padding:6px 12px;border-radius:8px 8px 0 0;border:6px solid var(--wood);border-bottom:0}
.pizarra-label b{color:#fff}
pre.pizarra,div.pizarra{margin-top:0;background:var(--chalk-bg);color:var(--chalk);border:6px solid var(--wood);border-radius:0 10px 10px 10px;box-shadow:inset 0 0 40px rgba(0,0,0,.35);font:15px/1.6 'JetBrains Mono',ui-monospace,monospace;padding:16px 18px;overflow-x:auto;white-space:pre}
table.pizarra{border:6px solid var(--wood);background:var(--chalk-bg);color:var(--chalk);margin-top:0}
table.pizarra th{background:#2a4a3c;color:#fff}table.pizarra td,table.pizarra th{border-color:#3d5e4f}
mjx-container[display=true]{overflow-x:auto;overflow-y:hidden;max-width:100%}
mjx-container:not([display]){display:inline-block;max-width:100%;overflow-x:auto;overflow-y:hidden;vertical-align:middle}
.lessons{display:grid;grid-template-columns:repeat(auto-fit,minmax(min(100%,240px),1fr));gap:16px;margin:24px 0}
.lessons a{display:block;text-decoration:none;color:var(--ink);background:var(--pale);border:1px solid var(--line);border-radius:12px;padding:18px}
.lessons a:hover{border-color:var(--cob)}
.lessons b{display:block;font:700 19px/1.2 'Space Grotesk',sans-serif;color:var(--cob-ink);margin-bottom:6px}
.lessons span{color:var(--muted);font-size:15px}
footer{color:var(--muted);font-size:14px;border-top:1px solid var(--line);padding:24px max(5vw,16px)}
@media(max-width:600px){body{font-size:16px}table{font-size:13px}pre.pizarra,div.pizarra{font-size:13px}main h1{font-size:27px}main h2{font-size:23px}}

/* pestañas de código */
.tabs{margin:20px 0;border:1px solid var(--line);border-radius:12px;overflow:hidden;background:light-dark(#f6f7fb,#11162a)}
.tablist{display:flex;gap:2px;padding:6px 6px 0;background:var(--pale);border-bottom:1px solid var(--line);overflow-x:auto}
.tablist button{font:600 14px Inter,sans-serif;color:var(--muted);background:none;border:0;border-radius:8px 8px 0 0;padding:10px 16px;min-height:44px;cursor:pointer;white-space:nowrap}
.tablist button[aria-selected=true]{background:light-dark(#f6f7fb,#11162a);color:var(--cob-ink);box-shadow:inset 0 3px 0 var(--cob)}
.tablist button:focus-visible{outline:3px solid var(--acc);outline-offset:-3px}
.tablist .badge{font:600 11px Inter,sans-serif;color:var(--acc);margin-left:6px}
.tabs .panel pre{margin:0;border:0;border-radius:0}
.tabs .panel[hidden]{display:none}
pre.salida{background:light-dark(#0f172a,#05070d);color:#d1fae5;border-color:#0f172a;margin-top:-8px;padding-top:30px}
pre.salida::before{content:"Salida";position:absolute;top:8px;left:16px;font:700 11px Inter,sans-serif;color:#6ee7b7;letter-spacing:.03em}
pre.salida .copy{display:none}
/* resaltado de sintaxis (clases de pandoc) */
code span.kw,code span.cf{color:light-dark(#1d4ed8,#93b4ff);font-weight:600}
code span.dt{color:light-dark(#7c3aed,#c4b5fd)}
code span.dv,code span.bn,code span.fl,code span.cn{color:light-dark(#b45309,#fbbf24)}
code span.st,code span.ch,code span.ss,code span.sc{color:light-dark(#047857,#6ee7b7)}
code span.co,code span.do,code span.cv{color:light-dark(#6b7280,#8b95a7);font-style:italic}
code span.pp,code span.im,code span.at{color:light-dark(#be185d,#f9a8d4)}
code span.fu,code span.bu{color:light-dark(#0e7490,#67e8f9)}
code span.op{color:light-dark(#374151,#cbd5e1)}
code span.er,code span.al{color:var(--acc)}
pre.sourceCode a{display:none}
@media print{nav.toc,.copy,.tablist{display:none}.tabs .panel[hidden]{display:block}.strip{background:none;color:#000}}
"""

HEAD = """<!DOCTYPE html>
<html lang="es"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{title}</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&family=JetBrains+Mono:wght@400;700&family=Space+Grotesk:wght@600;700&display=swap" rel="stylesheet">
<style>{css}</style>
<script>window.MathJax={{tex:{{inlineMath:[['\\\\(','\\\\)']],displayMath:[['\\\\[','\\\\]']]}},svg:{{fontCache:'global'}},options:{{enableMenu:false}}}};</script>
<script defer src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/3.2.2/es5/tex-svg.js"></script>
</head><body>
"""

SCRIPT = """<script>{

const KEY='algolib-lang';let pref='cpp';try{pref=localStorage.getItem(KEY)||'cpp'}catch(e){}
function pick(lang,save){
  for(const g of document.querySelectorAll('.tabs')){
    const btns=[...g.querySelectorAll('[role=tab]')];if(!btns.length)continue;
    const b=btns.find(x=>x.dataset.lang===lang)||btns[0];
    for(const x of btns){const on=x===b;x.setAttribute('aria-selected',on);x.tabIndex=on?0:-1;
      document.getElementById(x.getAttribute('aria-controls')).hidden=!on;}
  }
  if(save){try{localStorage.setItem(KEY,lang)}catch(e){}}
}
for(const g of document.querySelectorAll('.tablist')){
  g.addEventListener('click',e=>{const b=e.target.closest('[role=tab]');if(b){const y=b.getBoundingClientRect().top;pick(b.dataset.lang,true);window.scrollBy(0,b.getBoundingClientRect().top-y);}});
  g.addEventListener('keydown',e=>{const bs=[...g.querySelectorAll('[role=tab]')];const i=bs.indexOf(document.activeElement);
    if(i<0)return;let j=i;if(e.key==='ArrowRight')j=(i+1)%bs.length;else if(e.key==='ArrowLeft')j=(i-1+bs.length)%bs.length;else return;
    e.preventDefault();pick(bs[j].dataset.lang,true);bs[j].focus();});
}
pick(pref,false);
for(const p of document.querySelectorAll('main pre:not(.pizarra):not(.salida)')){
  const b=document.createElement('button');b.className='copy';b.type='button';b.textContent='Copiar';
  b.addEventListener('click',async()=>{try{await navigator.clipboard.writeText(p.innerText.replace(/Copiar$/,''));b.textContent='Copiado';}catch(e){b.textContent='No se pudo';}setTimeout(()=>b.textContent='Copiar',1600);});
  p.appendChild(b);}
}</script>"""


def pandoc(md_text):
    return subprocess.run(
        ["pandoc", "-f", "markdown+tex_math_dollars+lists_without_preceding_blankline-raw_tex", "-t", "html5", "--mathjax", "--wrap=none"],
        input=md_text, capture_output=True, text=True, check=True).stdout


def strip_emoji_prefix(tag, emoji):
    """Quita el emoji (y espacios) al inicio del primer nodo de texto del párrafo."""
    for node in tag.descendants:
        if isinstance(node, NavigableString) and node.strip():
            i = node.find(emoji)
            if i != -1:
                node.replace_with(NavigableString(node[i + len(emoji):].lstrip()))
            return


def nav_pages(current, soup_factory=None):
    links = ['<a href="index.html"{}>Inicio</a>'.format(' aria-current="page"' if current == "index.html" else "")]
    for _, out, label, _ in PAGES:
        cur = ' aria-current="page"' if out == current else ""
        links.append(f'<a href="{out}"{cur}>{html.escape(label)}</a>')
    return '<div class="pages">' + "".join(links) + "</div>"


def build_page(md_file, out_file, label, desc):
    md = (NOTAS / md_file).read_text(encoding="utf-8")
    # Título: el primer encabezado de nivel 1
    m = re.search(r"^# (.+)$", md, re.M)
    title = m.group(1).strip()
    md = md[:m.start()] + md[m.end():]
    body = pandoc(md)
    soup = BeautifulSoup(body, "html.parser")

    # 1. Figuras: incrustar SVG
    for img in soup.find_all("img"):
        src = img.get("src", "")
        if src.startswith("diagramas/") and src.endswith(".svg"):
            svg = BeautifulSoup((NOTAS / src).read_text(encoding="utf-8"), "html.parser").find("svg")
            svg.attrs.pop("width", None)
            svg.attrs.pop("height", None)
            img.replace_with(svg)

    # 2. Tiempos ⏱️ en encabezados
    for h in soup.find_all(["h1", "h2", "h3", "h4"]):
        s = str(h)
        if "⏱️" in s:
            new = re.sub(r"\(⏱️\s*([^)]*)\)", lambda mm: f'<span class="tiempo">{mm.group(1).strip()}</span>', s)
            h.replace_with(BeautifulSoup(new, "html.parser"))

    # 3. Callouts y pizarrón
    for p in list(soup.find_all("p")):
        txt = p.get_text().lstrip()
        if txt.startswith("✍️"):
            strip_emoji_prefix(p, "✍️")
            nxt = p.find_next_sibling()
            rest = p.get_text().strip()
            only_code = p.find("code") is not None and p.get_text().strip() == p.find("code").get_text().strip()
            if only_code:
                div = soup.new_tag("div", attrs={"class": "pizarra"})
                div.string = p.find("code").get_text()
                lab = soup.new_tag("p", attrs={"class": "pizarra-label"})
                lab.append(BeautifulSoup("<b>Pizarrón</b>", "html.parser"))
                p.replace_with(lab)
                lab.insert_after(div)
                continue
            p["class"] = "pizarra-label"
            inner = p.decode_contents()
            p.clear()
            p.append(BeautifulSoup("<b>Pizarrón</b>" + (" · " + inner if rest else ""), "html.parser"))
            if nxt is not None and nxt.name in ("pre", "table"):
                nxt["class"] = (nxt.get("class") or []) + ["pizarra"]
            elif nxt is not None and nxt.name == "div" and nxt.find("table"):
                nxt.find("table")["class"] = ["pizarra"]
            continue
        for emo, (cls, lbl) in CALLOUTS.items():
            if txt.startswith(emo):
                strip_emoji_prefix(p, emo)
                p.name = "div"
                p["class"] = f"callout {cls}"
                inner = p.decode_contents()
                p.clear()
                p.append(BeautifulSoup(f'<span class="lbl">{lbl}</span>' + inner, "html.parser"))
                break
        else:
            pass

    # 4. Monedas del banquero
    for node in list(soup.find_all(string=re.compile("🪙"))):
        node.replace_with(BeautifulSoup(html.escape(str(node)).replace("🪙", '<span class="moneda" aria-hidden="true"></span>'), "html.parser"))

    # 4b. Pestañas de código (::: {.tabs} con bloques c / cpp / rust)
    LANG = {"c": "C", "cpp": "C++", "rust": "Rust"}
    for gi, div in enumerate(soup.find_all("div", class_="tabs")):
        blocks = [b for b in div.find_all(["div", "pre"], recursive=False)]
        tl = soup.new_tag("div", attrs={"class": "tablist", "role": "tablist", "aria-label": "Lenguaje"})
        panels = []
        for bi, b in enumerate(blocks):
            pre = b if b.name == "pre" else b.find("pre")
            cls = pre.get("class") or []
            lang = next((c for c in cls if c in LANG), "texto")
            badge = ""
            if "no-compila" in cls:
                badge = "no compila"
            elif "no-ejecutar" in cls:
                badge = "comportamiento indefinido"
            pid = f"t{gi}-{bi}"
            btn = soup.new_tag("button", attrs={"type": "button", "role": "tab", "id": f"{pid}-b",
                                                "aria-controls": pid, "data-lang": lang,
                                                "aria-selected": "false", "tabindex": "-1"})
            btn.string = LANG.get(lang, lang)
            if badge:
                sp = soup.new_tag("span", attrs={"class": "badge"})
                sp.string = badge
                btn.append(sp)
            tl.append(btn)
            panel = soup.new_tag("div", attrs={"class": "panel", "role": "tabpanel", "id": pid,
                                               "aria-labelledby": f"{pid}-b", "data-lang": lang})
            b.extract()
            panel.append(b)
            panels.append(panel)
        div.clear()
        div.append(tl)
        for pnl in panels:
            div.append(pnl)

    # 5. Tablas con scroll horizontal
    for t in soup.find_all("table"):
        if t.parent.name != "div" or "scroll" not in (t.parent.get("class") or []):
            w = soup.new_tag("div", attrs={"class": "scroll"})
            t.wrap(w)

    # 6. Índice de secciones: h1 si existen (guion), si no h2
    heads = soup.find_all("h1") or soup.find_all("h2")
    toc = []
    for h in heads:
        hid = h.get("id")
        if not hid:
            continue
        t = h.get_text(" ", strip=True)
        t = re.sub(r"\s*\(.*$", "", t)
        t = re.sub(r"\s*~?\d+\s*(–\s*\d+)?\s*min.*$", "", t)
        t = re.sub(r"^§", "§", t)
        if len(t) > 34:
            t = t[:32].rstrip() + "…"
        toc.append(f'<a href="#{hid}">{html.escape(t)}</a>')

    page = HEAD.format(title=html.escape(f"{label} · {CURSO}"), css=CSS)
    page += f'<header class="strip"><p class="curso">algolib · {CURSO}</p><h1>{html.escape(title)}</h1>'
    page += f'<p class="desc">{html.escape(desc)}</p>{nav_pages(out_file)}</header>'
    page += f'<nav class="toc" aria-label="Secciones"><div class="scroll">{"".join(toc)}</div></nav>'
    main_html = str(soup).replace("✏️", '<span class="ejercicio">Resuélvelo</span>')
    page += f"<main>{main_html}</main>"
    page += f'<footer>algolib · {CURSO}. Fuente: <code>notas/{md_file}</code>. Regenerar con <code>make html</code>.</footer>'
    page += SCRIPT + "</body></html>\n"
    (OUT / out_file).write_text(page, encoding="utf-8")
    print("escrito", OUT / out_file)


def build_index():
    cards = "".join(
        f'<a href="{out}"><b>{html.escape(label)}</b><span>{html.escape(desc)}</span></a>'
        for _, out, label, desc in PAGES)
    ciclo = BeautifulSoup((NOTAS / "diagramas/ciclo-curso.svg").read_text(encoding="utf-8"), "html.parser").find("svg")
    ciclo.attrs.pop("width", None)
    ciclo.attrs.pop("height", None)
    body = f"""
<h2 id="lecciones">Lecciones publicadas</h2>
<div class="lessons">{cards}</div>
<h2 id="como">Cómo funciona cada lección</h2>
<figure>{ciclo}<figcaption>Notas → lab en C, C++ y Rust (<code>make labNN</code>) → problem set → preguntas de entrevista al cerrar la unidad.</figcaption></figure>
<p>La librería <code>algolib</code> trae la API y los tests de los 13 labs desde el primer día. Al principio todo aparece como <code>[TODO]</code>; el trabajo del alumno es dejarlo en <code>[ OK ]</code>.</p>
<pre><code>make lab01        # C, C++ y Rust
make c-lab01      # solo C
make cpp-lab01    # solo C++ (AddressSanitizer + UBSan)
make rust-lab01   # solo Rust</code></pre>
<h2 id="programa">Programa</h2>
<div class="scroll"><table>
<thead><tr><th>Lab</th><th>Tema</th><th>Contraste de paradigma</th></tr></thead><tbody>
<tr><td>01</td><td>Modelo, asintótica, amortizado · <code>ArrayStack</code>, <code>MinStack</code></td><td>excepción vs <code>Option</code>; regla de los 5 vs movimiento implícito</td></tr>
<tr><td>02</td><td>Memoria cruda · <code>Vector</code></td><td><code>allocator</code> vs <code>MaybeUninit</code> + <code>unsafe</code></td></tr>
<tr><td>03</td><td>Listas ligadas y ownership</td><td><code>unique_ptr</code> vs <code>Box</code>; ciclos de punteros</td></tr>
<tr><td>04</td><td>Colas y hashing</td><td><code>std::hash</code> vs <code>Hash</code> + <code>BuildHasher</code></td></tr>
<tr><td>05</td><td>Ordenamiento</td><td><code>std::span</code> vs <code>&amp;mut [T]</code></td></tr>
<tr><td>06</td><td>Heaps</td><td>comparador vs <code>Reverse&lt;T&gt;</code></td></tr>
<tr><td>07–08</td><td>AVL y árboles aumentados</td><td>rotaciones con <code>unique_ptr</code> vs <code>Option&lt;Box&gt;</code></td></tr>
<tr><td>09–11</td><td>Grafos, caminos mínimos, Union-Find, MST</td><td>recursión profunda; <code>find</code> mutante</td></tr>
<tr><td>12–13</td><td>Programación dinámica, tries, árboles de rango</td><td>UTF-8 <code>&amp;str</code> vs <code>std::string</code></td></tr>
</tbody></table></div>
"""
    page = HEAD.format(title=f"Inicio · {CURSO}", css=CSS)
    page += (f'<header class="strip"><p class="curso">algolib · curso de licenciatura en C++20 y Rust</p>'
             f'<h1>{CURSO}</h1><p class="desc">Teoría rigurosa con demostraciones completas, y una librería propia que el alumno '
             f'construye lab por lab en C, C++ y Rust para comparar sus paradigmas.</p>{nav_pages("index.html")}</header>')
    page += ('<nav class="toc" aria-label="Secciones"><div class="scroll"><a href="#lecciones">Lecciones</a>'
             '<a href="#como">Cómo funciona</a><a href="#programa">Programa</a></div></nav>')
    page += f"<main>{body}</main><footer>algolib · {CURSO}. Regenerar con <code>make html</code>.</footer>"
    page += SCRIPT + "</body></html>\n"
    (OUT / "index.html").write_text(page, encoding="utf-8")
    print("escrito", OUT / "index.html")


if __name__ == "__main__":
    for args in PAGES:
        build_page(*args)
    build_index()
