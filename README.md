# algolib — Estructuras de Datos y Algoritmos en C, C++20 y Rust

Curso de nivel licenciatura con un enfoque riguroso, al estilo de MIT 6.006/6.046. El alumno construye **su propia librería** de estructuras de datos, lab por lab, en **tres lenguajes** (C, C++ y Rust), y compara en cada paso cómo cada paradigma garantiza (o no) la correctitud.

## Cómo se trabaja

1. Lee las notas de la lección: abre `notas/html/index.html` en el navegador (o los `.md` de `notas/`). Empieza por L00.
2. Abre el lab: la API ya está declarada, y cada función pendiente llama a `ALGOLIB_TODO()` (C y C++) o `todo!()` (Rust).
3. Implementa hasta que los tests pasen:

```bash
make lab01          # C, C++ y Rust, uno tras otro
make c-lab01        # solo C    (compila con AddressSanitizer + UBSan)
make c-fugas-lab01  # fugas de memoria en C (LeakSanitizer en Linux, `leaks` en macOS)
make cpp-lab01      # solo C++  (compila con AddressSanitizer + UBSan)
make rust-lab01     # solo Rust
make all-labs       # todo el curso
make extraer-L00    # los programas de las notas de L00, cada uno en su carpeta de ejemplos/L00/
```

Salida típica al empezar:

```
== lab01 (C++) ==
  [ OK ] stack_lifo
  [FAIL] stack_alias_push_top
         tests/lab01/test_lab01.cpp:74  CHECK_EQ(s.top(), std::string(40, 'x'))  →  … != …
  [TODO] minstack_basico  (no implementado: push)
-- 1 ok, 1 fallidos, 1 pendientes
```

`[TODO]` significa que aún no lo implementas; `[FAIL]` significa que está implementado pero es incorrecto.

**Dependencias entre labs:** el lab08 parte de tu AVL del lab07; Dijkstra (lab10) y Prim (lab11) usan tu `BinaryHeap` del lab06. En Rust, los labs con `unsafe` (02 y 03) se revisan además con Miri: `cd rust && cargo +nightly miri test --test lab02`.

**Requisitos:** CMake ≥ 3.20, GCC ≥ 11 o Clang ≥ 14 (C17 y C++20) y Rust ≥ 1.75 (`rustup`). Para regenerar el sitio HTML: `pandoc` y `beautifulsoup4`.

## Empezar

Haz un **fork** de este repo en GitHub (botón *Fork*) y clona **tu** fork:

```bash
git clone https://github.com/<tu-usuario>/algolib.git
cd algolib
git remote add curso https://github.com/westerngazoo/algolib.git
make lab01
```

Trabaja en tu fork y haz un commit cada vez que avances (`git add -A && git commit -m "lab01: push con duplicación"`).

## Recibir actualizaciones del curso sin perder tus labs

```bash
git pull curso main
```

Git mezcla los cambios del curso (notas, tests, labs nuevos) con tu trabajo. El curso **no modifica** los archivos donde implementas, salvo para corregir un esqueleto que todavía no hayas tocado. Si alguna vez hay un conflicto en un archivo tuyo, git te lo marca y decides qué conservar.

<details><summary>¿Sin git? Actualizar desde un zip</summary>

Si trabajas con un zip en lugar de git, **no lo descomprimas encima** de tu carpeta. Desde tu carpeta de trabajo corre:

```bash
make actualizar ZIP=~/Downloads/algolib.zip
```

Tu trabajo (`c/src/`, `cpp/include/algolib/labNN/`, `rust/src/`) nunca se sobrescribe: si el curso cambió un archivo que ya editaste, la versión nueva queda al lado como `archivo.nuevo`. Los esqueletos que no has tocado sí se actualizan, y antes de empezar se guarda un respaldo en `../algolib-respaldo-<fecha>.tar.gz`.
</details>

## Estructura del repositorio

```
algolib/
├── Makefile                      make labNN / c-labNN / cpp-labNN / rust-labNN / html / ejemplos
├── notas/                        teoría por lección (.md) y el sitio del curso (notas/html/)
├── c/
│   ├── CMakeLists.txt            biblioteca algolib_c + un ejecutable labNN_tests por lab
│   ├── include/algolib/labNN/*.h API (tipos opacos, genéricos void* + tamaño + comparador)
│   ├── src/labNN/*.c             ← implementas aquí (tú defines cada struct)
│   └── tests/                    runner algotest en C (setjmp/longjmp) + tests por lab
├── cpp/
│   ├── CMakeLists.txt            un ejecutable labNN_tests por lab
│   ├── include/algolib/labNN/*.hpp ← implementas aquí
│   └── tests/                    micro-framework algotest (sin dependencias) + tests por lab
├── rust/
│   ├── Cargo.toml                un test de integración por lab
│   ├── src/labNN/                ← implementas aquí
│   └── tests/                    runner con [ OK ]/[FAIL]/[TODO] + tests por lab
└── herramientas/                 actualizar.sh (actualización por zip) + manifiesto de esqueletos
```

Los tests de los tres lenguajes usan el **mismo generador pseudoaleatorio** (`Lcg`, con la misma semilla), así que los casos aleatorios son comparables. Siempre que es posible, comparan contra un **oráculo independiente** (la biblioteca estándar, o un algoritmo ingenuo escrito en el propio test) en lugar de valores fijos. C y C++ compilan con AddressSanitizer y UBSan; para la memoria sin liberar en C, usa `make c-fugas-labNN`: en Linux corre LeakSanitizer y en macOS (donde LeakSanitizer no existe) la herramienta `leaks` del sistema, que señala la línea del `malloc` que nunca se liberó.

## Programa

| Lab | Lección | Estructuras / algoritmos | Contraste de paradigma principal | Tests validados* |
|---|---|---|---|---|
| — | L00 Prerrequisitos | — | modelo de memoria; manual vs. RAII vs. ownership | — |
| 01 | Modelo, asintótica, amortizado | `ArrayStack`, `MinStack` | `bool`/`NULL` vs. excepción vs. `Option`; `void*` vs. plantillas vs. genéricos; aliasing | ✅ |
| 02 | Memoria cruda | `Vector` | `malloc` vs. `allocator`+`construct_at` vs. `MaybeUninit`+`unsafe` | ✅ |
| 03 | Listas ligadas, ownership | `ForwardList`/`LinkedStack`, `DList` | punteros crudos vs. `unique_ptr` vs. `Box`/`NonNull` | ✅ |
| 04 | Colas y hashing | `RingQueue`, `HashMap` | puntero a función vs. `std::hash` vs. `Hash`+`BuildHasher` | ✅ |
| 05 | Ordenamiento | insertion, merge, quick, heap, counting, radix | firma de `qsort` vs. `std::span` vs. `&mut [T]` | ✅ |
| 06 | Heaps | `BinaryHeap` | comparador como función vs. plantilla vs. `Reverse<T>` | ✅ |
| 07 | BST y AVL | `AvlMap` | rotaciones con punteros vs. `unique_ptr` vs. `Option<Box>` | ✅ |
| 08 | Árboles aumentados | `OrderStatSet` (select/rank) | — | ✅ |
| 09 | Grafos: recorridos | BFS, DFS, topológico, SCC | recursión profunda en los tres; DFS iterativo | ✅ |
| 10 | Caminos mínimos | Dijkstra, Bellman-Ford, DAG, Floyd-Warshall | `INT64_MAX` vs. `std::optional` vs. `Option` como ∞ | ✅ |
| 11 | Union-Find y MST | `DisjointSet`, Kruskal, Prim | `find` mutante | ✅ |
| 12 | Programación dinámica | Fibonacci, LCS, edición, mochila, LIS, monedas | `char*` vs. `std::string` vs. UTF-8 `&str` | ✅ |
| 13 | Tries y árboles de rango | `Trie`, `FenwickTree`, `SegmentTree` | — | ✅ |

\* ✅ = los tests pasan contra la solución de referencia del instructor en los tres lenguajes. ⏳ = los tests compilan y salen como `[TODO]`; se validan al llegar a cada lección.

Las preguntas tipo entrevista se agregarán en `entrevistas/` al cerrar cada unidad.
