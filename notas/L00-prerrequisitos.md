# Lección 0 — Prerrequisitos: las herramientas que usaremos todo el curso

> **Qué es esta lección.** Antes de estudiar algoritmos necesitamos tres cosas: un **idioma** para escribir afirmaciones precisas (lógica), unas **herramientas** para demostrarlas (inducción, invariantes) y para contar (logaritmos, sumas, recurrencias, probabilidad), y un **modelo mental** de cómo la computadora guarda los datos (memoria, C, C++ y Rust).
>
> **Cómo leerla.** Cada tema sigue el mismo orden: *para qué sirve en el curso* → *la idea con un ejemplo cotidiano* → *la definición precisa* → *un ejemplo resuelto paso a paso* → *un programa que puedes compilar y correr* → *errores típicos* → *ejercicios con solución*.
>
> **Los programas** vienen en tres pestañas: **C**, **C++** y **Rust**. Los tres hacen exactamente lo mismo e imprimen exactamente la misma salida; elige el lenguaje que conozcas mejor y compara con los otros dos. Todos los programas de esta lección se compilan y ejecutan automáticamente (`make ejemplos`), así que puedes copiarlos con confianza.

---

## 0. Cómo funciona el curso

![Ciclo de cada lección: notas, lab, problem set y entrevistas](diagramas/ciclo-curso.svg)

Cada lección tiene cuatro partes:

1. **Notas** (como esta): teoría con demostraciones completas.
2. **Lab**: implementas la estructura de datos de la lección en **C, C++ y Rust** dentro de nuestra librería `algolib`. Los tests ya están escritos; al principio salen `[TODO]` y tu trabajo es dejarlos en `[ OK ]`.
3. **Problem set**: ejercicios de demostración y de extensión del lab.
4. **Preguntas de entrevista**, al cerrar cada unidad.

### 0.1 Preparar tu computadora

| Sistema | Compiladores de C y C++, CMake | Rust |
|---|---|---|
| Ubuntu / Debian / WSL | `sudo apt install build-essential cmake` | `curl https://sh.rustup.rs -sSf \| sh` |
| macOS | `xcode-select --install` y `brew install cmake` | igual que arriba |
| Windows | usa WSL (Ubuntu) y sigue la primera fila | igual que arriba |

### 0.2 Tu primer programa en los tres lenguajes

Guarda cada versión en un archivo, compílala y ejecútala. Fíjate en cómo cambia la forma de compilar:

::: {.tabs}
```c
// hola.c        compilar:  gcc -std=c17 hola.c -o hola     ejecutar:  ./hola
#include <stdio.h>

int main(void) {
    printf("Hola, algolib\n");
    return 0;
}
```

```cpp
// hola.cpp      compilar:  g++ -std=c++20 hola.cpp -o hola  ejecutar:  ./hola
#include <iostream>

int main() {
    std::cout << "Hola, algolib\n";
}
```

```rust
// hola.rs       compilar:  rustc hola.rs                   ejecutar:  ./hola
fn main() {
    println!("Hola, algolib");
}
```
:::

```{.text .salida}
Hola, algolib
```

Con la librería del curso todo se hace con `make`:

```bash
make lab01        # compila y corre los tests del lab 1 en C, C++ y Rust
make extraer-L00  # copia cada programa de esta lección a su propia carpeta en ejemplos/L00/
```

**Para correr y modificar los programas de las notas** no hace falta copiarlos a mano. `make extraer-L00` crea una carpeta por programa (por ejemplo `ejemplos/L00/08-4-3-ejemplo-importante-busqueda-binaria/`) con `programa.c`, `programa.cpp`, `programa.rs`, la salida esperada y un `Makefile`. Dentro de la carpeta, `make` compila y corre los tres y te dice si la salida coincide con la de las notas; `make c`, `make cpp` o `make rust` corren uno solo. Cambia lo que quieras: es tu copia, git la ignora y volver a extraer no la sobrescribe (`make extraer-L00 FORZAR=1` la regenera). Lo mismo sirve para cualquier lección: `make extraer-L05`.

---

## 1. Lógica: el idioma de las definiciones

**¿Para qué la necesitamos?** Para leer, y sobre todo para **traducir**, la definición central del curso: la de la notación $O$.

**Qué es la notación $O$ ("O grande", *big O notation*).** Cuando decimos que un algoritmo "es $O(n^2)$", queremos decir que su costo **no crece más rápido que $n^2$**, sin importar las constantes ni lo que pase con entradas pequeñas. Formalmente, $O(g)$ no es un número ni una función: es una **familia** (un conjunto) de funciones, **todas las que crecen a lo más como $g$**. Por ejemplo, $O(n^2)$ contiene a $n^2$, a $3n^2+7$, a $5n$ y a $\lg n$, pero no a $n^3$. Por eso se escribe $f\in O(g)$: "$f$ **pertenece** a la familia $O(g)$". La estudiaremos a fondo en la L01; aquí solo aprendemos a leerla.

La definición es una frase con tres cuantificadores seguidos:
$$f\in O(g)\iff \exists c>0\ \ \exists n_0\ \ \forall n\ge n_0:\ f(n)\le c\,g(n).$$

**Tradúcela pedazo por pedazo** antes de seguir:

| Símbolo | Se lee | Qué significa aquí |
|---|---|---|
| $f\in O(g)$ | "$f$ está en $O$ de $g$" | $f$ pertenece a la familia de funciones que crecen a lo más como $g$ |
| $\iff$ | "si y solo si" | lo que sigue es la definición: las dos frases dicen exactamente lo mismo |
| $\exists c>0$ | "existe una constante $c$ positiva" | puedo escoger un factor de holgura, tan grande como quiera, pero **fijo** |
| $\exists n_0$ | "existe un punto de arranque $n_0$" | puedo ignorar las entradas pequeñas, las que están antes de $n_0$ |
| $\forall n\ge n_0$ | "para toda $n$ desde $n_0$ en adelante" | a partir de ahí, **sin excepciones** |
| $f(n)\le c\,g(n)$ | "$f$ de $n$ es a lo más $c$ veces $g$ de $n$" | $f$ queda por debajo de $g$ multiplicada por esa constante |

**En una frase:** *"$f$ es $O(g)$ si puedo encontrar una constante $c$ y un punto de arranque $n_0$ tales que, de ahí en adelante, $f$ **nunca** supera a $c$ veces $g$."* Dicho aún más simple: **ignorando constantes y entradas chicas, $f$ no crece más rápido que $g$**.

**Con números: ¿es $3n+5\in O(n)$?** Hay que encontrar un **testigo**: una pareja concreta $c$, $n_0$ que haga verdadera la frase. Nada de esto es magia; vamos por partes.

**Qué papel juega $c$.** Con $g(n)=n$, la condición $f(n)\le c\cdot n$ dice: *"$f$ cabe debajo de $c$ copias de $n$"*. Así que $c$ responde a una pregunta muy concreta: **¿cuántas copias de $n$ necesito para tapar a $3n+5$?**

**Estrategia: convertir cada término de $f$ en copias de $n$.** Se mira $3n+5$ término por término:

- El término $3n$ **ya son** 3 copias de $n$. No hay nada que hacer.
- El término $5$ es una constante, no un múltiplo de $n$. Para "taparlo" necesito escribir algo de la forma $5\le(\text{algunas copias})\cdot n$, y eso **solo es cierto a partir de cierta $n$**. Ahí aparece $n_0$.

Hay dos maneras naturales de tapar el $5$, y cada una da un testigo distinto:

| | Opción A: tapar el 5 con **una** copia de $n$ | Opción B: tapar el 5 con **cinco** copias de $n$ |
|---|---|---|
| Desigualdad que necesito | $5\le n$ | $5\le 5n$ |
| ¿Desde cuándo es cierta? | desde $n=5$ (antes, $n$ es menor que 5) | desde $n=1$: multiplica $1\le n$ por 5 |
| Por eso $n_0$ = | $5$ | $1$ |
| Sustituyo el 5 en $3n+5$ | $3n+5\ \le\ 3n+n$ | $3n+5\ \le\ 3n+5n$ |
| Cuento las copias de $n$ | $3+1=4$ | $3+5=8$ |
| Por eso $c$ = | $4$ | $8$ |

Fíjate de dónde salen las $n$: en la opción A, la $n$ extra **es la que puse en lugar del 5**; en la opción B, las $5n$ **son las que puse en lugar del 5**. Y $c$ no se adivina: **es la suma de las copias** que quedaron ($3$ que ya había más las que usé para tapar el $5$).

**Compruébalo con números** antes de creerlo:

| $n$ | $3n+5$ | $4n$ (opción A) | ¿$3n+5\le4n$? | $8n$ (opción B) | ¿$3n+5\le8n$? |
|---|---|---|---|---|---|
| 1 | 8 | 4 | no (todavía no llegamos a $n_0=5$) | 8 | sí |
| 3 | 14 | 12 | no | 24 | sí |
| 5 | 20 | 20 | sí, justo | 40 | sí |
| 10 | 35 | 40 | sí | 80 | sí |
| 100 | 305 | 400 | sí | 800 | sí |

Y en una gráfica: cada recta es un número de copias de $n$, y la línea punteada marca $n_0=5$ para $c=4$.

![3n+5 contra 4n y 8n: 4n la tapa desde n = 5 y 8n desde n = 1](diagramas/testigos-3n5.svg)

La tabla y la gráfica muestran el **intercambio** entre $c$ y $n_0$: con pocas copias ($c=4$) tengo que esperar a que $n$ crezca ($n_0=5$); con muchas copias ($c=8$) funciona desde el principio ($n_0=1$). Los dos testigos son válidos, y basta con **uno** para demostrar que $3n+5\in O(n)$.

> 💡 **La receta, en general.** Para demostrar que una suma de términos está en $O(g)$: acota cada término por un múltiplo de $g$, anotando a partir de qué $n$ vale cada acotación. Entonces $c$ es la suma de los múltiplos y $n_0$ es el **mayor** de los "a partir de qué $n$". En la L01 (§3.3) usaremos exactamente esta receta con funciones más complicadas.

En cambio, $n^2\notin O(n)$. Supón que alguien propone una constante $c$. En cuanto $n>c$, multiplicamos ambos lados de $n>c$ por $n$ (que es positivo, así que la desigualdad se conserva) y obtenemos $n\cdot n>c\cdot n$, es decir, $n^2>c\cdot n$: la cota falla. Como esto pasa para **cualquier** $c$, ninguna constante sirve.

![n² contra 2n, 5n y 10n: la parábola cruza cada recta en n = c y queda arriba](diagramas/n2-vs-cn.svg)

En la gráfica: la parábola $n^2$ cruza a cada recta $c\cdot n$ justo en $n=c$, y de ahí en adelante queda arriba para siempre. Subir $c$ no resuelve nada: solo mueve el cruce más a la derecha. Y no importa qué $n_0$ propongan: basta tomar una $n$ que sea mayor que $c$ **y** mayor que $n_0$.

**Sus dos hermanas, $\Omega$ y $\Theta$.** $O$ es una cota **por arriba**. La cota **por abajo** es $\Omega$: $f\in\Omega(g)$ si existen $c>0$ y $n_0$ tales que $f(n)\ge c\,g(n)$ para todo $n\ge n_0$ ("de ahí en adelante, $f$ nunca baja de $c$ veces $g$"). Y $f\in\Theta(g)$ si está en las dos: en $O(g)$ y en $\Omega(g)$, o sea que crece **igual** que $g$, ignorando constantes. También escribiremos $f\prec g$ para decir que $f$ crece **estrictamente** más lento que $g$. *Con números:* $3n+5$ es $\Omega(n)$ con $c=3$, $n_0=1$ (porque $3n+5\ge3n$), y junto con lo de arriba, $3n+5\in\Theta(n)$. En la L01 (§3) se estudian a fondo.

> 💭 **Para reflexionar.** ¿Por qué la definición deja escoger $c$ y $n_0$, en lugar de exigir simplemente $f(n)\le g(n)$ para toda $n$? Piensa en dos algoritmos: uno hace $100n$ pasos y otro $n^2$. ¿Cuál es mejor para $n=10$? ¿Y para $n=10^6$? La notación $O$ está diseñada para responder la segunda pregunta.

> **Hábito del curso:** cada vez que veas una fórmula con cuantificadores, **tradúcela a palabras** antes de usarla, y vuelve de las palabras a la fórmula para comprobar que no cambiaste el significado. Si no puedes decirla en español, todavía no la entiendes. En esta sección cada fórmula viene con su traducción.

Si no sabes leer, usar y **negar** frases así, no podrás demostrar ni refutar una cota. Esta sección te deja listo para eso.

### 1.1 Proposiciones y conectivos

Una **proposición** es una afirmación que es verdadera o falsa: "$7$ es primo" (verdadera), "$2+2=5$" (falsa). Se combinan con **conectivos**:

| Símbolo | Se lee | Es verdadera cuando… | En C, C++ y Rust |
|---|---|---|---|
| $\neg P$ | "no $P$" | $P$ es falsa | `!p` |
| $P\land Q$ | "$P$ y $Q$" | las dos son verdaderas | `p && q` |
| $P\lor Q$ | "$P$ o $Q$" | al menos una es verdadera | `p \|\| q` |
| $P\Rightarrow Q$ | "si $P$, entonces $Q$" | **todos los casos excepto** $P$ verdadera y $Q$ falsa | `!p \|\| q` |
| $P\iff Q$ | "$P$ si y solo si $Q$" | las dos tienen el mismo valor | `p == q` |

**La implicación es la que confunde.** Piensa en una promesa: *"si llueve, llevo paraguas"*. ¿Cuándo **rompí** la promesa? Solo si llovió **y** no llevé paraguas. Si no llovió, no rompí nada, lleve o no paraguas. Por eso $P\Rightarrow Q$ es falsa **solo** cuando $P$ es verdadera y $Q$ es falsa:

| $P$ | $Q$ | $P\Rightarrow Q$ | ¿Rompí la promesa? |
|---|---|---|---|
| V | V | V | no: llovió y llevé paraguas |
| V | F | **F** | **sí**: llovió y no llevé |
| F | V | V | no: no llovió (llevar paraguas no rompe nada) |
| F | F | V | no: no llovió |

### 1.2 Cuantificadores: "para todo" y "existe"

- $\forall x\in A:\ P(x)$ — "**para todo** $x$ de $A$ se cumple $P(x)$".
- $\exists x\in A:\ P(x)$ — "**existe** (al menos un) $x$ de $A$ con $P(x)$".

La mejor forma de entenderlos es como **ciclos**:

- Para comprobar un $\forall$, recorres **todos** los elementos y te detienes en cuanto uno **falla** (un contraejemplo basta para que sea falso).
- Para comprobar un $\exists$, recorres hasta encontrar **uno que cumpla** (un testigo basta para que sea verdadero).

::: {.tabs}
```c
#include <stdbool.h>
#include <stdio.h>

// ∀ i: a[i] > 0      → busca un contraejemplo
bool todos_positivos(const int *a, int n) {
    for (int i = 0; i < n; i++)
        if (!(a[i] > 0)) return false;   // encontré uno que falla
    return true;                          // ninguno falló
}

// ∃ i: a[i] < 0      → busca un testigo
bool existe_negativo(const int *a, int n) {
    for (int i = 0; i < n; i++)
        if (a[i] < 0) return true;        // encontré uno que cumple
    return false;                         // ninguno cumplió
}

int main(void) {
    int a[] = {3, 1, -4, 1, 5};
    int n = 5;
    printf("todos positivos: %s\n", todos_positivos(a, n) ? "sí" : "no");
    printf("existe negativo: %s\n", existe_negativo(a, n) ? "sí" : "no");
    return 0;
}
```

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

// ∀ i: a[i] > 0      → busca un contraejemplo
bool todos_positivos(const std::vector<int>& a) {
    for (int x : a)
        if (!(x > 0)) return false;   // encontré uno que falla
    return true;                       // ninguno falló
}

// ∃ i: a[i] < 0      → busca un testigo
bool existe_negativo(const std::vector<int>& a) {
    for (int x : a)
        if (x < 0) return true;        // encontré uno que cumple
    return false;                      // ninguno cumplió
}

int main() {
    std::vector<int> a{3, 1, -4, 1, 5};
    std::cout << "todos positivos: " << (todos_positivos(a) ? "sí" : "no") << "\n";
    std::cout << "existe negativo: " << (existe_negativo(a) ? "sí" : "no") << "\n";
    // La biblioteca estándar ya trae ambos cuantificadores:
    //   std::all_of(a.begin(), a.end(), pred)   es ∀
    //   std::any_of(a.begin(), a.end(), pred)   es ∃
    bool t = std::all_of(a.begin(), a.end(), [](int x) { return x > 0; });
    (void)t;
}
```

```rust
// ∀ i: a[i] > 0      → busca un contraejemplo
fn todos_positivos(a: &[i32]) -> bool {
    for &x in a {
        if !(x > 0) {
            return false; // encontré uno que falla
        }
    }
    true // ninguno falló
}

// ∃ i: a[i] < 0      → busca un testigo
fn existe_negativo(a: &[i32]) -> bool {
    for &x in a {
        if x < 0 {
            return true; // encontré uno que cumple
        }
    }
    false // ninguno cumplió
}

fn main() {
    let a = [3, 1, -4, 1, 5];
    println!("todos positivos: {}", if todos_positivos(&a) { "sí" } else { "no" });
    println!("existe negativo: {}", if existe_negativo(&a) { "sí" } else { "no" });
    // Los iteradores ya traen ambos cuantificadores:
    //   a.iter().all(|&x| x > 0)   es ∀
    //   a.iter().any(|&x| x < 0)   es ∃
    assert_eq!(a.iter().all(|&x| x > 0), todos_positivos(&a));
}
```
:::

```{.text .salida}
todos positivos: no
existe negativo: sí
```

### 1.3 Negar una frase con cuantificadores

Observa el código de arriba: "**no** todos son positivos" es lo mismo que "**existe** uno que **no** es positivo". En general:

$$
\begin{aligned}
\neg\big(\forall x:\ P(x)\big)&\iff\exists x:\ \neg P(x)\\
\neg\big(\exists x:\ P(x)\big)&\iff\forall x:\ \neg P(x)
\end{aligned}
$$

**Regla mecánica:** para negar, recorre la frase de izquierda a derecha, **cambia cada $\forall$ por $\exists$ y cada $\exists$ por $\forall$**, y al final **niega la condición**.

**Ejemplo resuelto: negar la definición de $O$.** Queremos escribir qué significa $f\notin O(g)$.

| Paso | Frase | Qué hicimos |
|---|---|---|
| 0 | $\neg\big(\exists c>0\ \exists n_0\ \forall n\ge n_0:\ f(n)\le c\,g(n)\big)$ | la negación de la definición |
| 1 | $\forall c>0\ \neg\big(\exists n_0\ \forall n\ge n_0:\ f(n)\le c\,g(n)\big)$ | primer $\exists$ → $\forall$, la negación avanza |
| 2 | $\forall c>0\ \forall n_0\ \neg\big(\forall n\ge n_0:\ f(n)\le c\,g(n)\big)$ | segundo $\exists$ → $\forall$ |
| 3 | $\forall c>0\ \forall n_0\ \exists n\ge n_0:\ \neg\big(f(n)\le c\,g(n)\big)$ | $\forall$ → $\exists$ |
| 4 | $\forall c>0\ \forall n_0\ \exists n\ge n_0:\ f(n)> c\,g(n)$ | negar "$\le$" da "$>$" |

En palabras: *"para cualquier constante $c$ y cualquier umbral $n_0$ que me den, puedo encontrar una $n$ más allá del umbral donde la cota falla"*. Lo usaremos en la L01 §3.3.8.

> ⚠️ Observa que $c>0$ **no** se niega: es la descripción de *qué valores recorre* $c$, no parte de la condición. La negación de "$\exists c>0$" es "$\forall c>0$", no "$\forall c\le 0$".

### 1.4 El orden de los cuantificadores importa

Compara estas dos frases sobre los alumnos y los asientos de un salón:

- $\forall$ alumno $\exists$ asiento: el alumno se sienta en el asiento. — *"Cada alumno tiene **algún** asiento"* (pueden ser asientos distintos).
- $\exists$ asiento $\forall$ alumno: el alumno se sienta en el asiento. — *"Hay **un** asiento donde se sientan **todos**"* (absurdo).

Con números: $\forall x\ \exists y:\ y>x$ es verdadera (dado $x$, toma $y=x+1$). Pero $\exists y\ \forall x:\ y>x$ es falsa (no hay un número mayor que todos). **Cambiar el orden cambia el significado.** En la definición de $O$, $c$ y $n_0$ van **antes** del $\forall n$: deben funcionar **para todas** las $n$ a la vez, no una $c$ distinta para cada $n$.

✏️ **Ejercicio 0.1.** Niega formalmente: $\forall\varepsilon>0\ \exists\delta>0\ \forall x:\ |x|<\delta\Rightarrow|f(x)|<\varepsilon$.
<details><summary>Solución</summary>

Aplico la regla mecánica de §1.3: cambio cada $\forall$ por $\exists$ y cada $\exists$ por $\forall$, y niego la condición:
$$\exists\varepsilon>0\ \forall\delta>0\ \exists x:\ \neg\big(|x|<\delta\Rightarrow|f(x)|<\varepsilon\big)$$
Falta negar la implicación. La negación de $P\Rightarrow Q$ es $P\land\neg Q$ (la promesa se rompe cuando $P$ ocurre y $Q$ no). Resultado:
$$\exists\varepsilon>0\ \forall\delta>0\ \exists x:\ |x|<\delta\ \land\ |f(x)|\ge\varepsilon.$$

**En palabras.** La frase original dice: *"por pequeña que sea la tolerancia $\varepsilon$, hay una ventana alrededor de 0 (de radio $\delta$) donde $|f(x)|$ queda por debajo de esa tolerancia"*; es decir, $f(x)$ se acerca a 0 cuando $x$ se acerca a 0. La negación dice: *"hay una tolerancia $\varepsilon$ tal que, por pequeña que hagas la ventana, siempre hay una $x$ dentro de ella donde $|f(x)|$ no baja de $\varepsilon$"*; es decir, $f(x)$ **no** se acerca a 0.
</details>

---

## 2. Cómo se escribe una demostración

**¿Para qué?** En este curso no basta con que un algoritmo "funcione en los ejemplos": hay que **demostrar** que es correcto y cuánto cuesta. Hay cuatro técnicas básicas; aquí va cada una con un ejemplo corto.

### 2.1 Demostración directa

Partes de la hipótesis y avanzas con pasos válidos hasta la conclusión.

**Afirmación:** si $n$ es par, entonces $n^2$ es par.

"Par" significa "el doble de un entero". Así que tengo que llegar a escribir $n^2$ como $2\cdot(\text{algún entero})$.

**Paso 1. Traduzco la hipótesis.** Que $n$ sea par quiere decir que existe un entero $k$ con $n=2k$. ($k$ es la mitad de $n$: si $n=6$, $k=3$.) La encierro en una caja:
$$\boxed{\,n=2k\,}$$

**Paso 2. Uso la caja dentro de $n^2$.** Cambio $n$ por $2k$:
$$n^2=(2k)^2$$

**Paso 3. Escribo el cuadrado como producto.** Por definición, $x^2=x\cdot x$:
$$n^2=(2k)\cdot(2k)$$

**Paso 4. Multiplico.** $2\cdot2=4$ y $k\cdot k=k^2$; el orden de los factores no cambia un producto:
$$n^2=4k^2$$

**Paso 5. Saco un 2.** Como $4=2\cdot2$, la expresión ya tiene la forma "2 por algo":
$$n^2=2\cdot(2k^2)$$

**Paso 6. Compruebo con un número.** Con $n=6$ y $k=3$: el paso 2 da $6^2=36$; el paso 4 da $4\cdot9=36$; el paso 5 da $2\cdot18=36$ ✓.

**Conclusión.** $2k^2$ es entero, porque es producto de enteros. Entonces $n^2$ es 2 por un entero: es par. $\square$

### 2.2 Contrapositiva

$P\Rightarrow Q$ es **equivalente** a $\neg Q\Rightarrow\neg P$ (si la promesa "si llueve, llevo paraguas" se cumple, entonces "si no llevo paraguas, es que no llovió"). A veces la segunda es más fácil de probar.

**Afirmación:** si $n^2$ es par, entonces $n$ es par.

Directamente es incómodo. Probamos la contrapositiva: **si $n$ es impar, entonces $n^2$ es impar.** "Impar" significa "el doble de un entero, más uno", así que tengo que llegar a $n^2=2\cdot(\text{entero})+1$.

**Paso 1. Traduzco la hipótesis.** Que $n$ sea impar quiere decir que existe un entero $k$ con:
$$\boxed{\,n=2k+1\,}$$

**Paso 2. Uso la caja dentro de $n^2$.** Cambio $n$ por $2k+1$:
$$n^2=(2k+1)^2$$

**Paso 3. Desarrollo el binomio.** Uso $(a+b)^2=a^2+2ab+b^2$ con $a=2k$ y $b=1$. (Sale de multiplicar $(a+b)(a+b)=a^2+ab+ba+b^2$.)
$$n^2=(2k)^2+2\cdot(2k)\cdot1+1^2$$

**Paso 4. Simplifico cada término.** $(2k)^2=4k^2$, $\ 2\cdot2k\cdot1=4k$ y $1^2=1$:
$$n^2=4k^2+4k+1$$

**Paso 5. Saco un 2 de los dos primeros términos.** Por la distributiva al revés, $4k^2+4k=2\,(2k^2+2k)$. El $+1$ se queda aparte: es justo el "más uno" que busco.
$$n^2=2\cdot(2k^2+2k)+1$$

**Paso 6. Compruebo con un número.** Con $n=7$ y $k=3$: el paso 2 da $7^2=49$; el paso 4 da $36+12+1=49$; el paso 5 da $2\cdot24+1=49$ ✓.

**Conclusión.** $2k^2+2k$ es entero, así que $n^2$ es impar. $\square$

**Por qué esto demuestra la afirmación original.** Probé "$n$ impar $\Rightarrow$ $n^2$ impar", que es $\neg Q\Rightarrow\neg P$ con $P$="$n^2$ es par" y $Q$="$n$ es par". Como es equivalente a $P\Rightarrow Q$, también quedó probado "$n^2$ par $\Rightarrow$ $n$ par".

### 2.3 Por contradicción

Supones que la conclusión es **falsa** y llegas a algo imposible. Entonces la conclusión tenía que ser verdadera.

**Afirmación (Euclides):** hay infinitos números primos.

**Paso 1. Supongo lo contrario.** Hay una cantidad finita de primos, y esta lista los tiene **todos**:
$$\boxed{\,p_1,\ p_2,\ \dots,\ p_k\,}$$

**Paso 2. Construyo un número nuevo.** Multiplico todos los primos de la lista y le sumo 1:
$$N=p_1\cdot p_2\cdots p_k+1$$

**Paso 3. Ningún primo de la lista divide a $N$.** Tomo cualquier $p_i$ de la lista. $N$ es un múltiplo de $p_i$ **más uno**, así que al dividir $N$ entre $p_i$ el residuo es $1$, no $0$. *Con números:* si la lista fuera $2,3,5$, $N=31=2\cdot15+1=3\cdot10+1=5\cdot6+1$.
$$N=p_i\cdot(\text{el producto de los demás primos})+1$$

**Paso 4. Algún primo sí divide a $N$.** Cada primo es $\ge2$, así que $N\ge2+1=3$. Todo entero $\ge2$ tiene algún divisor primo (§3.6 demuestra que es un producto de primos, y cualquiera de ellos lo divide). Llamo $q$ a ese primo:
$$N=q\cdot(\text{algún entero})$$

**Paso 5. Llego a algo imposible.** Por el paso 3, $q$ **no** está en la lista. Pero la caja del paso 1 decía que la lista tenía **todos** los primos. La suposición del paso 1 es falsa, y los primos son infinitos. $\square$

### 2.4 Contraejemplo: para refutar un "para todo"

Para demostrar que una afirmación "**para todo** $n$…" es **falsa**, basta **un** caso donde falle (§1.3).

**Afirmación (falsa):** "$n^2+n+41$ es primo para todo $n\ge0$". Para $n=0,1,2,\dots,39$ da primos ($41, 43, 47, 53,\dots$). Parece cierta… hasta que una computadora busca el contraejemplo:

::: {.tabs}
```c
#include <stdbool.h>
#include <stdio.h>

// Devuelve el divisor más pequeño de x (>= 2); si x es primo, devuelve x.
long menor_divisor(long x) {
    for (long d = 2; d * d <= x; d++)
        if (x % d == 0) return d;
    return x;
}

int main(void) {
    for (long n = 0;; n++) {
        long v = n * n + n + 41;
        long d = menor_divisor(v);
        if (d != v) {   // no es primo: ¡contraejemplo!
            printf("primer contraejemplo: n = %ld, valor = %ld = %ld x %ld\n", n, v, d, v / d);
            return 0;
        }
    }
}
```

```cpp
#include <iostream>

// Devuelve el divisor más pequeño de x (>= 2); si x es primo, devuelve x.
long menor_divisor(long x) {
    for (long d = 2; d * d <= x; ++d)
        if (x % d == 0) return d;
    return x;
}

int main() {
    for (long n = 0;; ++n) {
        long v = n * n + n + 41;
        long d = menor_divisor(v);
        if (d != v) {   // no es primo: ¡contraejemplo!
            std::cout << "primer contraejemplo: n = " << n << ", valor = " << v
                      << " = " << d << " x " << v / d << "\n";
            return 0;
        }
    }
}
```

```rust
/// Devuelve el divisor más pequeño de x (>= 2); si x es primo, devuelve x.
fn menor_divisor(x: i64) -> i64 {
    let mut d = 2;
    while d * d <= x {
        if x % d == 0 {
            return d;
        }
        d += 1;
    }
    x
}

fn main() {
    for n in 0i64.. {
        let v = n * n + n + 41;
        let d = menor_divisor(v);
        if d != v {
            // no es primo: ¡contraejemplo!
            println!("primer contraejemplo: n = {n}, valor = {v} = {d} x {}", v / d);
            return;
        }
    }
}
```
:::

```{.text .salida}
primer contraejemplo: n = 40, valor = 1681 = 41 x 41
```

> **Moraleja (muy importante para los labs):** 40 casos que funcionan **no demuestran** nada; un solo caso que falla **refuta**. Los tests pueden encontrar errores, pero nunca demuestran que no los hay. Por eso en este curso los tests y las demostraciones van juntos.

---

## 3. Inducción matemática

**¿Para qué?** Casi todas las demostraciones del curso son inducciones disfrazadas: la correctitud de un algoritmo recursivo, el costo de una secuencia de operaciones, la altura de un árbol balanceado.

### 3.1 La idea: una fila de dominós

Imagina una fila infinita de dominós numerados $0,1,2,\dots$ Para estar seguro de que **todos** caen, basta saber dos cosas:

1. **Base:** el primer dominó cae.
2. **Paso:** si cualquier dominó cae, tumba al siguiente.

Con eso, cae el 0; como cae el 0, cae el 1; como cae el 1, cae el 2; y así **para siempre**. No hace falta revisar los dominós uno por uno: el paso es una sola demostración que vale para **todos** a la vez.

**Plantilla de una demostración por inducción** (cópiala tal cual en tus tareas):

> **Afirmación $P(n)$:** … (escríbela explícitamente, con $n$)
>
> **Base:** demuestro $P(n_0)$ (normalmente $n_0=0$ o $1$) sustituyendo y calculando.
>
> **Hipótesis de inducción (HI):** supongo que $P(k)$ es verdadera para un $k\ge n_0$ **cualquiera**, y la escribo en una caja.
>
> **Paso:** usando la HI, demuestro $P(k+1)$. *(Encierra en otra caja el pedazo donde usas la HI.)*
>
> **Conclusión:** por inducción, $P(n)$ vale para todo $n\ge n_0$. $\square$

**Qué es cada letra.**

- **$P(n)$ es una frase con un hueco.** Cada número que pongas en el hueco $n$ da una frase verdadera o falsa. En §3.2, $P(n)$ es "$1+2+\dots+n=\frac{n(n+1)}{2}$", y $P(3)$ es "$1+2+3=\frac{3\cdot4}{2}$", que es verdadera ($6=6$).
- **$k$ es un número fijo pero cualquiera.** Es **un** dominó concreto, del que solo sé que $k\ge n_0$ y que cae. No elijo cuál; por eso lo que demuestre vale para todos a la vez. No escribo "supongo $P(n)$": eso sería suponer justo lo que quiero demostrar.
- **$P(k+1)$ se obtiene sustituyendo:** cada $n$ se reemplaza por $(k+1)$, **con paréntesis**. En §3.2, $\frac{n(n+1)}{2}$ se vuelve $\frac{(k+1)\big((k+1)+1\big)}{2}=\frac{(k+1)(k+2)}{2}$.

**Lado izquierdo y lado derecho** son siempre los de la fórmula que quiero demostrar, $P(k+1)$. La HI no es un lado: es una caja que uso como herramienta. Trabajo con un solo lado hasta que se vea igual al otro, y en algún paso uso la caja. El modelo completo, paso por paso, es §3.2.

**Trucos de álgebra para el paso.** Cada uno reescribe una expresión **sin cambiar su valor**. Cuando uses uno, dilo por su nombre. El ejemplo de §3.3 usa varios.

- **Separar el último:** $1+\dots+k+(k+1)=\big(1+\dots+k\big)+(k+1)$, y $5^{k+1}=5\cdot5^k$.
- **Sumar cero:** restar y sumar lo mismo, $5\cdot5^k-1=5\cdot5^k-5+5-1$.
- **Multiplicar por uno:** $(k+1)=\frac{2(k+1)}{2}$, porque $\frac22=1$.
- **Factor común:** $k(k+1)+2(k+1)=(k+1)(k+2)$, igual que $3x+2x=5x$.
- **Traducir la definición:** "múltiplo de 4" significa "$=4m$ con $m$ entero".
- **Restar y ver el signo**, para comparar: $(2k+2)-(k+2)=k\ge0$, así que $2k+2\ge k+2$.

**Señales de alarma:**

- **Nunca usaste la HI.** O no hacía falta inducción, o el álgebra está mal.
- **Usaste la meta para justificar un paso.** Es circular: la meta solo es el destino.
- **El paso no funciona con el $k$ más chico** ($k=n_0$). O te falta una base (como en las monedas de §15), o la afirmación es falsa (como en los caballos de §3.7).

### 3.2 Ejemplo resuelto: la suma $1+2+\dots+n$

Lo que queremos demostrar:
$$1+2+\dots+n=\frac{n(n+1)}{2}\qquad\text{para todo } n\ge1.$$

Vamos paso a paso. Cada paso hace **una sola cosa**.

**Paso 1. Pruebo con números chicos.** Calculo los dos lados para $n=1,2,3$:
$$
\begin{aligned}
n=1:&\quad 1=1 &&\text{y}\quad \tfrac{1\cdot2}{2}=1\ ✓\\
n=2:&\quad 1+2=3 &&\text{y}\quad \tfrac{2\cdot3}{2}=3\ ✓\\
n=3:&\quad 1+2+3=6 &&\text{y}\quad \tfrac{3\cdot4}{2}=6\ ✓
\end{aligned}
$$
Funciona. Pero probar casos no es demostrar (§2.4): faltan todos los demás $n$.

**Paso 2. La base.** El primer dominó es $n=1$, y en el paso 1 ya vimos que cae ✓.

**Paso 3. Supongo que la fórmula vale para un $n=k$ cualquiera.** Escribo la fórmula con $k$ en lugar de $n$ y la encierro en una caja. Esta caja es la **hipótesis de inducción (HI)**: es lo único que puedo usar.
$$\boxed{\,1+2+\dots+k=\frac{k(k+1)}{2}\,}\qquad\text{(HI)}$$

**Paso 4. Escribo lo que quiero demostrar: la fórmula con $n=k+1$.** Cambio cada $n$ por $(k+1)$. A la izquierda, la suma ahora llega hasta $k+1$; escribo ese último sumando a la vista:
$$1+2+\dots+k+(k+1)\ \overset{?}{=}\ \frac{(k+1)(k+2)}{2}$$
El $\overset{?}{=}$ dice que **todavía no sé** si es cierto. Ahora trabajo solo con el lado izquierdo, hasta que se vea igual al derecho.

**Paso 5. Busco la caja de la HI dentro del lado izquierdo.** Los primeros $k$ sumandos son **exactamente** el lado izquierdo de la caja:
$$\boxed{\,1+2+\dots+k\,}+(k+1)$$

**Paso 6. Uso la HI.** Cambio lo que está en la caja por lo que la HI dice que vale:
$$\frac{k(k+1)}{2}+(k+1)$$
Ya no hay puntos suspensivos. Lo que falta es álgebra.

**Paso 7. Pongo el segundo término sobre 2.** Para sumar fracciones necesito el mismo denominador. Multiplico $(k+1)$ por $\frac22$, que vale 1 y no cambia nada:
$$\frac{k(k+1)}{2}+\frac{2(k+1)}{2}$$

**Paso 8. Sumo las fracciones.** Con el mismo denominador, se suman los numeradores:
$$\frac{k(k+1)+2(k+1)}{2}$$

**Paso 9. Saco $(k+1)$ de factor común.** Arriba hay $k$ veces $(k+1)$ más $2$ veces $(k+1)$: en total, $(k+2)$ veces $(k+1)$. Es lo mismo que $3x+2x=5x$, con $x=k+1$:
$$\frac{(k+1)(k+2)}{2}$$
Es exactamente el lado derecho del paso 4 ✓.

**Paso 10. Compruebo con un número.** Con $k=3$: el lado izquierdo del paso 4 es $1+2+3+4=10$; el paso 6 da $\frac{3\cdot4}{2}+4=10$; el paso 9 da $\frac{4\cdot5}{2}=10$ ✓. Todos los pasos dan lo mismo, así que ninguno cambió el valor.

**Conclusión.** El paso 2 tiró el primer dominó ($n=1$). Los pasos 3 a 9 demostraron que **cualquier** dominó $k$ tumba al siguiente, $k+1$. Entonces caen todos: la fórmula vale para todo $n\ge1$. $\square$

> **¿Por qué no es trampa suponer la HI?** No supuse lo que quería demostrar (la fórmula con $k+1$); supuse la fórmula con $k$ y de ahí **llegué** a la de $k+1$. Fíjate en que la meta del paso 4 nunca se usó para justificar nada: solo fue el destino.

Un programa puede **comprobar** la fórmula para muchos valores (y es buena práctica hacerlo antes de intentar demostrarla), pero recuerda §2.4: comprobar no es demostrar.

::: {.tabs}
```c
#include <stdio.h>

int main(void) {
    printf(" n  suma  n(n+1)/2\n");
    for (int n = 1; n <= 6; n++) {
        int suma = 0;
        for (int i = 1; i <= n; i++) suma += i;
        printf("%2d  %4d  %8d\n", n, suma, n * (n + 1) / 2);
    }
    return 0;
}
```

```cpp
#include <cstdio>

int main() {
    std::printf(" n  suma  n(n+1)/2\n");
    for (int n = 1; n <= 6; ++n) {
        int suma = 0;
        for (int i = 1; i <= n; ++i) suma += i;
        std::printf("%2d  %4d  %8d\n", n, suma, n * (n + 1) / 2);
    }
}
```

```rust
fn main() {
    println!(" n  suma  n(n+1)/2");
    for n in 1..=6 {
        let suma: i32 = (1..=n).sum();
        println!("{:2}  {:4}  {:8}", n, suma, n * (n + 1) / 2);
    }
}
```
:::

```{.text .salida}
 n  suma  n(n+1)/2
 1     1         1
 2     3         3
 3     6         6
 4    10        10
 5    15        15
 6    21        21
```

### 3.3 Segundo ejemplo: $5^n-1$ es múltiplo de 4

Lo que queremos demostrar: $5^n-1$ es múltiplo de 4 para todo $n\ge0$. Aquí no hay un lado derecho con fórmula: la meta es **exhibir** el entero que multiplica al 4.

**Paso 1. Pruebo con números chicos.**
$$
\begin{aligned}
n=0:&\quad 5^0-1=0=4\cdot0\ ✓\\
n=1:&\quad 5^1-1=4=4\cdot1\ ✓\\
n=2:&\quad 5^2-1=24=4\cdot6\ ✓
\end{aligned}
$$
Funciona, pero probar casos no es demostrar.

**Paso 2. La base.** El primer dominó es $n=0$, y en el paso 1 ya vimos que cae: $5^0-1=0=4\cdot0$ ✓.

**Paso 3. Supongo que vale para un $n=k\ge0$ cualquiera, y traduzco la definición.** "Múltiplo de 4" significa "4 por un entero". A ese entero le llamo $m$:
$$\boxed{\,5^k-1=4m\,}\qquad\text{(HI, con } m \text{ entero)}$$

**Paso 4. Escribo lo que quiero.** Cambio $n$ por $(k+1)$:
$$5^{k+1}-1\ \overset{?}{=}\ 4\cdot(\text{algún entero})$$
Trabajo con $5^{k+1}-1$ hasta que aparezca un 4 multiplicando a todo.

**Paso 5. Separo el último factor.** Por la ley de exponentes, $5^{k+1}=5\cdot5^k$:
$$5\cdot5^k-1$$

**Paso 6. Sumo cero.** Resto 5 y sumo 5, que no cambia nada. ¿Por qué 5? Quiero que aparezca $5\cdot(5^k-1)$, que es $5\cdot5^k-5$, y me falta ese $-5$:
$$5\cdot5^k-5+5-1$$

**Paso 7. Saco el factor común 5 y hago la resta que sobra.** Por la distributiva al revés, $5\cdot5^k-5\cdot1=5\,(5^k-1)$; y $5-1=4$:
$$5\,(5^k-1)+4$$

**Paso 8. Busco la caja de la HI.** El paréntesis es **exactamente** el lado izquierdo de la caja:
$$5\,\boxed{\,5^k-1\,}+4$$

**Paso 9. Uso la HI.** Cambio la caja por lo que la HI dice que vale, $4m$:
$$5\cdot4m+4$$

**Paso 10. Saco el factor común 4.** $5\cdot4m$ es 4 por $5m$ (el orden de los factores no cambia el producto), y $4=4\cdot1$; por la distributiva al revés:
$$4\,(5m+1)$$
Como $m$ es entero, $5m+1$ también lo es. Es la meta del paso 4 ✓.

**Paso 11. Compruebo con un número.** Con $k=2$ la HI dice $5^2-1=24=4\cdot6$, así que $m=6$. El paso 4 da $5^3-1=124$; el paso 7, $5\cdot24+4=124$; el paso 10, $4\cdot31=124$ ✓.

**Conclusión.** El paso 2 tiró el primer dominó ($n=0$). Los pasos 3 a 10 demostraron que **cualquier** dominó $k$ tumba al siguiente, $k+1$. Por inducción, $5^n-1$ es múltiplo de 4 para todo $n\ge0$. $\square$

### 3.4 Tercer ejemplo: una desigualdad

Lo que queremos demostrar:
$$2^n\ge n+1\qquad\text{para todo } n\ge0.$$

**Paso 1. Pruebo con números chicos.** Calculo los dos lados para $n=0,1,2,3$:
$$
\begin{aligned}
n=0:&\quad 2^0=1 &&\text{y}\quad 0+1=1;\quad 1\ge1\ ✓\\
n=1:&\quad 2^1=2 &&\text{y}\quad 1+1=2;\quad 2\ge2\ ✓\\
n=2:&\quad 2^2=4 &&\text{y}\quad 2+1=3;\quad 4\ge3\ ✓\\
n=3:&\quad 2^3=8 &&\text{y}\quad 3+1=4;\quad 8\ge4\ ✓
\end{aligned}
$$
Se cumple, y cada vez con más holgura.

**Paso 2. La base.** El primer dominó es $n=0$: lado izquierdo $2^0=1$, lado derecho $0+1=1$, y $1\ge1$ ✓.

**Paso 3. Supongo que vale para un $n=k\ge0$ cualquiera.**
$$\boxed{\,2^k\ge k+1\,}\qquad\text{(HI)}$$

**Paso 4. Escribo lo que quiero.** Cambio cada $n$ por $(k+1)$; a la derecha queda $(k+1)+1=k+2$:
$$2^{k+1}\ \overset{?}{\ge}\ k+2$$
Trabajo con el lado izquierdo. Cada paso que sigue empieza con $2^{k+1}$ y dice que es igual, o mayor o igual, a otra cosa. Todos los signos van en la **misma** dirección, así que al final $2^{k+1}$ es $\ge$ que lo último (transitividad: si $a\ge b$ y $b\ge c$, entonces $a\ge c$).

**Paso 5. Separo el último factor y busco la caja.** Por la ley de exponentes, $a^{m+1}=a\cdot a^m$. Es una igualdad: no gano ni pierdo nada. Ahora aparece $2^k$, que es de lo que habla la HI:
$$2^{k+1}=2\cdot\boxed{\,2^k\,}$$

**Paso 6. Uso la HI.** La HI dice $2^k\ge k+1$. Multiplico los dos lados por 2; como 2 es **positivo**, la desigualdad no se voltea (con un negativo sí se voltearía):
$$2^{k+1}\ge2\,(k+1)$$

**Paso 7. Abro el paréntesis.** Por la distributiva, el 2 multiplica a cada sumando:
$$2^{k+1}\ge2k+2$$

**Paso 8. Cierro la distancia restando.** Quiero $k+2$ y tengo $2k+2$. Resto: $(2k+2)-(k+2)=k$, y $k\ge0$; entonces $2k+2\ge k+2$. Por transitividad con el paso 7:
$$2^{k+1}\ge k+2$$
Es la meta del paso 4 ✓.

**Paso 9. Compruebo con un número.** Con $k=3$, $2^{k+1}=16$. El paso 5 da $16=2\cdot8$; el paso 7, $16\ge6+2=8$; el paso 8, $16\ge5$ ✓. La HI me da más de lo que necesito ($16$ contra $5$): el paso 8 solo tira la parte que sobra.

**Conclusión.** El paso 2 tiró el primer dominó ($n=0$). Los pasos 3 a 8 demostraron que cualquier dominó $k$ tumba al $k+1$. Por inducción, $2^n\ge n+1$ para todo $n\ge0$. $\square$

> **Truco para desigualdades:** reescribe el lado izquierdo de la meta hasta que **aparezca** la caja de la HI (aquí $2^k$, con $2^{k+1}=2\cdot 2^k$), usa la HI y luego cierra la distancia **restando** lo que obtuviste menos lo que querías: si la diferencia es $\ge 0$, terminaste.

### 3.5 Inducción y recursión son lo mismo

Una función recursiva es una inducción escrita en código: el **caso base** de la función es la **base** de la inducción, y la **llamada recursiva** confía en que la función ya es correcta para un valor más chico, igual que la **HI**.

::: {.tabs}
```c
#include <stdio.h>

// Afirmación: suma(n) devuelve 1 + 2 + ... + n.
long suma(long n) {
    if (n == 0) return 0;      // base: la suma vacía es 0
    return n + suma(n - 1);    // HI: suma(n-1) ya es correcta
}

int main(void) {
    printf("suma(100) = %ld\n", suma(100));
    return 0;
}
```

```cpp
#include <iostream>

// Afirmación: suma(n) devuelve 1 + 2 + ... + n.
long suma(long n) {
    if (n == 0) return 0;      // base: la suma vacía es 0
    return n + suma(n - 1);    // HI: suma(n-1) ya es correcta
}

int main() {
    std::cout << "suma(100) = " << suma(100) << "\n";
}
```

```rust
/// Afirmación: suma(n) devuelve 1 + 2 + ... + n.
fn suma(n: u64) -> u64 {
    if n == 0 {
        return 0; // base: la suma vacía es 0
    }
    n + suma(n - 1) // HI: suma(n-1) ya es correcta
}

fn main() {
    println!("suma(100) = {}", suma(100));
}
```
:::

```{.text .salida}
suma(100) = 5050
```

**Demostración de que `suma` es correcta**, por inducción sobre $n$. Lo que queremos demostrar: `suma(n)` devuelve $1+\dots+n$ (y $0$ cuando $n=0$, la suma de cero términos).

**Paso 1. Pruebo con números chicos.** Sigo el código a mano:
$$
\begin{aligned}
\text{suma}(1)&=1+\text{suma}(0)=1+0=1\ ✓\\
\text{suma}(2)&=2+\text{suma}(1)=2+1=3=1+2\ ✓\\
\text{suma}(3)&=3+\text{suma}(2)=3+3=6=1+2+3\ ✓
\end{aligned}
$$

**Paso 2. La base.** Con $n=0$ el código entra al `if` y devuelve 0, que es la suma de cero términos ✓.

**Paso 3. Supongo que vale para un $n=k\ge0$ cualquiera.**
$$\boxed{\,\text{suma}(k)=1+\dots+k\,}\qquad\text{(HI)}$$

**Paso 4. Escribo lo que quiero.** Cambio $n$ por $(k+1)$:
$$\text{suma}(k+1)\ \overset{?}{=}\ 1+\dots+k+(k+1)$$

**Paso 5. Sigo el código y busco la caja.** Como $k+1\ge1$, no entra al `if` y ejecuta `return n + suma(n - 1)` con $n=k+1$. La llamada recursiva es la caja de la HI:
$$\text{suma}(k+1)=(k+1)+\boxed{\,\text{suma}(k)\,}$$

**Paso 6. Uso la HI.** Cambio la caja por lo que la HI dice que devuelve:
$$\text{suma}(k+1)=(k+1)+(1+\dots+k)$$

**Paso 7. Reordeno los sumandos.** Sumar en otro orden no cambia el resultado:
$$\text{suma}(k+1)=1+\dots+k+(k+1)$$
Es la meta del paso 4 ✓.

**Paso 8. Compruebo con un número.** Con $k=3$: el paso 5 da $4+\text{suma}(3)$; el paso 6, $4+6=10$; el paso 7, $1+2+3+4=10$ ✓.

**Conclusión.** El paso 2 tiró el primer dominó ($n=0$), y los pasos 3 a 7 mostraron que cada dominó tumba al siguiente. Por inducción, `suma(n)` devuelve $1+\dots+n$ para todo $n\ge0$. $\square$

> Esta forma de razonar ("supongo que la llamada recursiva funciona") es exactamente cómo se diseñan los algoritmos recursivos del curso: merge sort, quicksort, los recorridos de árboles.

### 3.6 Inducción fuerte

A veces, para probar $P(k+1)$, no basta con $P(k)$: necesitas $P$ de un valor **cualquiera** más chico. La **inducción fuerte** te deja suponer **todos** los anteriores:

> **HI fuerte:** supongo $P(j)$ para **todo** $j$ con $n_0\le j\le k$. **Paso:** demuestro $P(k+1)$.

**Lo que queremos demostrar:** todo entero $n\ge2$ es producto de primos.

**Paso 1. Pruebo con números chicos.**
$$
\begin{aligned}
n=2:&\quad 2\ \text{es primo}\ ✓\\
n=3:&\quad 3\ \text{es primo}\ ✓\\
n=4:&\quad 4=2\cdot2\ ✓
\end{aligned}
$$

**Paso 2. La base.** $n=2$ es primo: es un producto de un solo primo ✓.

**Paso 3. Supongo la HI fuerte.** Para un $k\ge2$ cualquiera, supongo que **todos** los anteriores cumplen:
$$\boxed{\,\text{todo } j \text{ con } 2\le j\le k \text{ es producto de primos}\,}\qquad\text{(HI fuerte)}$$

**Paso 4. Escribo lo que quiero.** $k+1$ es producto de primos. Hay dos casos.

*Caso 1: $k+1$ es primo.*

**Paso 5. Ya terminé.** Es un producto de un solo primo ✓.

*Caso 2: $k+1$ no es primo.*

**Paso 6. Lo parto en dos.** Como $k+1$ no es primo, tiene un divisor $a$ que no es 1 ni $k+1$. Llamo $b$ al cociente:
$$k+1=a\cdot b$$

**Paso 7. Reviso que $a$ cabe en la caja.** $a\ne1$, así que $a\ge2$. Un divisor no pasa del número y $a\ne k+1$, así que $a<k+1$; como es entero, $a\le k$:
$$2\le a\le k$$

**Paso 8. Reviso que $b$ cabe en la caja.** $b$ también divide a $k+1$. Si $b$ fuera 1, $a$ sería $k+1$; si $b$ fuera $k+1$, $a$ sería 1. Entonces $b$ cumple lo mismo que $a$:
$$2\le b\le k$$

**Paso 9. Uso la HI fuerte.** $a$ y $b$ están en la caja, así que los dos son productos de primos. Juntando las dos listas de primos:
$$k+1=a\cdot b=(\text{primos de } a)\cdot(\text{primos de } b)\ ✓$$

**Paso 10. Compruebo con un número.** Con $k=11$: $k+1=12$ no es primo, y $12=3\cdot4$. Rangos: $2\le3\le11$ y $2\le4\le11$ ✓. La HI da $3=3$ y $4=2\cdot2$; juntando, $12=3\cdot2\cdot2$ ✓.

**Conclusión.** El paso 2 tiró el primer dominó ($n=2$). Los pasos 3 a 9 mostraron que, si caen todos los dominós de 2 a $k$, cae el $k+1$. Por inducción fuerte, todo $n\ge2$ es producto de primos. $\square$

Fíjate en que $a$ y $b$ **no** son $k$: pueden ser cualquier valor más chico. Por eso hace falta la versión fuerte. Es lo que pasa en divide y vencerás: los subproblemas son "más chicos", pero no "uno menos". El programa sigue exactamente la demostración:

::: {.tabs}
```c
#include <stdio.h>

// Imprime la factorización de n >= 2 en primos (en orden no decreciente).
void factorizar(long n) {
    for (long d = 2; d * d <= n; d++) {
        if (n % d == 0) {          // n = d * (n/d), ambos más chicos que n
            factorizar(d);         // HI fuerte sobre d
            printf(" x");
            factorizar(n / d);     // HI fuerte sobre n/d
            return;
        }
    }
    printf(" %ld", n);             // n es primo: caso base
}

int main(void) {
    printf("360 =");
    factorizar(360);
    printf("\n");
    return 0;
}
```

```cpp
#include <iostream>

// Imprime la factorización de n >= 2 en primos (en orden no decreciente).
void factorizar(long n) {
    for (long d = 2; d * d <= n; ++d) {
        if (n % d == 0) {          // n = d * (n/d), ambos más chicos que n
            factorizar(d);         // HI fuerte sobre d
            std::cout << " x";
            factorizar(n / d);     // HI fuerte sobre n/d
            return;
        }
    }
    std::cout << " " << n;         // n es primo: caso base
}

int main() {
    std::cout << "360 =";
    factorizar(360);
    std::cout << "\n";
}
```

```rust
/// Imprime la factorización de n >= 2 en primos (en orden no decreciente).
fn factorizar(n: u64) {
    let mut d = 2;
    while d * d <= n {
        if n % d == 0 {
            // n = d * (n/d), ambos más chicos que n
            factorizar(d); // HI fuerte sobre d
            print!(" x");
            factorizar(n / d); // HI fuerte sobre n/d
            return;
        }
        d += 1;
    }
    print!(" {n}"); // n es primo: caso base
}

fn main() {
    print!("360 =");
    factorizar(360);
    println!();
}
```
:::

```{.text .salida}
360 = 2 x 2 x 2 x 3 x 3 x 5
```

### 3.7 Errores típicos con la inducción

1. **Olvidar la base.** El paso solo no basta: "si $k$ dominós caen, el siguiente cae" no sirve si ningún dominó cae nunca. (Prueba a "demostrar" $n=n+1$: el paso funciona —si $k=k+1$, sumando 1 queda $k+1=k+2$— pero la base es falsa.)
2. **Suponer lo que se quiere probar.** La HI es para $k$, no para $k+1$.
3. **Un paso que no funciona para el primer $k$.** El ejemplo clásico de "todos los caballos son del mismo color" falla al pasar de $k=1$ a $k=2$. Verifica siempre que tu paso funciona para el $k$ más chico.

✏️ **Ejercicio 0.2.** Demuestra por inducción que $\sum_{i=0}^{k}2^i=2^{k+1}-1$. (Lo usaremos en la L01 para el arreglo dinámico.)
<details><summary>Solución</summary>

Aquí la variable de la afirmación ya se llama $k$; para no confundir, la renombro $n$. Quiero demostrar $\sum_{i=0}^{n}2^i=2^{n+1}-1$ para todo $n\ge0$, y uso $k$ en la HI, como en la plantilla.

**Paso 1. Pruebo con números chicos.**
$$
\begin{aligned}
n=0:&\quad 1 &&\text{y}\quad 2^1-1=1\ ✓\\
n=1:&\quad 1+2=3 &&\text{y}\quad 2^2-1=3\ ✓\\
n=2:&\quad 1+2+4=7 &&\text{y}\quad 2^3-1=7\ ✓
\end{aligned}
$$

**Paso 2. La base.** Con $n=0$ la suma tiene un solo término, $2^0=1$, y el lado derecho es $2^{0+1}-1=1$ ✓.

**Paso 3. Supongo que vale para un $n=k\ge0$ cualquiera.**
$$\boxed{\,\sum_{i=0}^{k}2^i=2^{k+1}-1\,}\qquad\text{(HI)}$$

**Paso 4. Escribo lo que quiero.** Cambio cada $n$ por $(k+1)$; a la derecha, $2^{(k+1)+1}=2^{k+2}$:
$$\sum_{i=0}^{k+1}2^i\ \overset{?}{=}\ 2^{k+2}-1$$

**Paso 5. Separo el último término y busco la caja.** El término $i=k+1$ es $2^{k+1}$; lo que queda antes es **exactamente** el lado izquierdo de la caja:
$$\boxed{\,\sum_{i=0}^{k}2^i\,}+2^{k+1}$$

**Paso 6. Uso la HI.** Cambio la caja por lo que vale:
$$(2^{k+1}-1)+2^{k+1}$$

**Paso 7. Junto los dos $2^{k+1}$.** Algo más sí mismo es el doble, $x+x=2x$:
$$2\cdot2^{k+1}-1$$

**Paso 8. Uso la ley de exponentes.** $2\cdot2^{k+1}=2^{1+(k+1)}=2^{k+2}$:
$$2^{k+2}-1$$
Es el lado derecho del paso 4 ✓.

**Paso 9. Compruebo con un número.** Con $k=2$: el lado izquierdo del paso 4 es $1+2+4+8=15$; el paso 6 da $(8-1)+8=15$; el paso 8, $16-1=15$ ✓.

**Conclusión.** El paso 2 tiró el primer dominó y los pasos 3 a 8 mostraron que cada uno tumba al siguiente. Por inducción, la fórmula vale para todo $n\ge0$. $\square$
</details>

---

## 4. Invariantes de ciclo: inducción para ciclos `for` y `while`

**¿Para qué?** Para demostrar que un ciclo calcula lo que dice. Es la herramienta con la que justificaremos cada algoritmo iterativo del curso: búsqueda binaria, particionado de quicksort, Dijkstra.

### 4.1 La idea

Un **invariante** es una afirmación sobre las variables que es verdadera **al inicio de cada vuelta** del ciclo. Se demuestra igual que una inducción, sobre el número de vueltas:

| Parte | Qué se demuestra | Analogía con la inducción |
|---|---|---|
| **Inicialización** | el invariante vale antes de la primera vuelta | base |
| **Mantenimiento** | si vale al inicio de una vuelta, vale al inicio de la siguiente | paso |
| **Terminación** | el ciclo termina, y al terminar, *invariante + condición de salida* dan el resultado | conclusión |

### 4.2 Primer ejemplo: sumar un arreglo

El ciclo empieza con `s = 0` y, para cada `i` de `0` a `n-1`, hace `s += a[i]` (el programa completo está más abajo).

**Invariante:** al inicio de la vuelta con índice $i$, $s$ es la suma de los primeros $i$ elementos. Lo escribo en una caja, igual que una HI:
$$\boxed{\,s=a[0]+a[1]+\dots+a[i-1]\,}$$

Seguimos el ciclo a mano con `a = [4, 2, 7, 1]`:

| Inicio de la vuelta | $i$ | $s$ | ¿$s$ = suma de los primeros $i$? |
|---|---|---|---|
| 1ª | 0 | 0 | suma vacía = 0 ✓ |
| 2ª | 1 | 4 | $4$ ✓ |
| 3ª | 2 | 6 | $4+2$ ✓ |
| 4ª | 3 | 13 | $4+2+7$ ✓ |
| salida | 4 | 14 | $4+2+7+1$ ✓ |

**Paso 1. Inicialización.** Antes de la primera vuelta, $i=0$ y $s=0$. La suma de cero elementos es 0, así que la caja se cumple ✓:
$$i=0,\qquad s=0$$

**Paso 2. Mantenimiento: escribo lo que quiero.** Supongo la caja al inicio de la vuelta $i$. Quiero la caja al inicio de la vuelta siguiente: cambio $i$ por $(i+1)$, y $a[(i+1)-1]=a[i]$:
$$s_{\text{nueva}}\ \overset{?}{=}\ a[0]+\dots+a[i]$$

**Paso 3. Ejecuto la vuelta y busco la caja.** El código hace `s += a[i]`. El $s$ de antes es el lado izquierdo de la caja:
$$s_{\text{nueva}}=\boxed{\,s\,}+a[i]$$

**Paso 4. Uso el invariante.** Cambio la caja por lo que vale:
$$s_{\text{nueva}}=\big(a[0]+\dots+a[i-1]\big)+a[i]$$

**Paso 5. Quito los paréntesis.** Sumar en otro agrupamiento no cambia el resultado:
$$s_{\text{nueva}}=a[0]+\dots+a[i]$$
Es la meta del paso 2 ✓.

**Paso 6. Compruebo con un número.** En la vuelta $i=2$ de la traza: el paso 3 da $6+7=13$; el paso 4, $(4+2)+7=13$; el paso 5, $4+2+7=13$ ✓.

**Paso 7. Terminación.** $i$ sube de 1 en 1 y el ciclo para en $i=n$. La caja con $i=n$ dice lo que queríamos ✓:
$$s=a[0]+\dots+a[n-1]$$

Un invariante también se puede **comprobar en el código** con `assert`. Es una gran herramienta de depuración: si el invariante se rompe, el programa se detiene justo ahí.

::: {.tabs}
```c
#include <assert.h>
#include <stdio.h>

// suma de a[0..i)  (solo para verificar el invariante)
static int suma_prefijo(const int *a, int i) {
    int s = 0;
    for (int j = 0; j < i; j++) s += a[j];
    return s;
}

int main(void) {
    int a[] = {4, 2, 7, 1};
    int n = 4, s = 0;
    for (int i = 0; i < n; i++) {
        assert(s == suma_prefijo(a, i));   // INVARIANTE al inicio de la vuelta
        s += a[i];
    }
    assert(s == suma_prefijo(a, n));       // terminación: i == n
    printf("suma = %d\n", s);
    return 0;
}
```

```cpp
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> a{4, 2, 7, 1};
    int s = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        // INVARIANTE al inicio de la vuelta: s == a[0] + ... + a[i-1]
        assert(s == std::accumulate(a.begin(), a.begin() + i, 0));
        s += a[i];
    }
    assert(s == std::accumulate(a.begin(), a.end(), 0));  // terminación
    std::cout << "suma = " << s << "\n";
}
```

```rust
fn main() {
    let a = [4, 2, 7, 1];
    let mut s = 0;
    for i in 0..a.len() {
        // INVARIANTE al inicio de la vuelta: s == a[0] + ... + a[i-1]
        assert_eq!(s, a[..i].iter().sum::<i32>());
        s += a[i];
    }
    assert_eq!(s, a.iter().sum::<i32>()); // terminación
    println!("suma = {s}");
}
```
:::

```{.text .salida}
suma = 14
```

### 4.3 Ejemplo importante: búsqueda binaria

**Problema:** dado un arreglo **ordenado** `a[0..n)` y un valor `x`, encontrar el **primer índice** `i` con `a[i] >= x` (o `n` si no hay ninguno). Es el `std::lower_bound` de C++ y el `partition_point` de Rust.

**Idea:** mantener un rango `[lo, hi)` donde está la respuesta, y partirlo a la mitad en cada vuelta.

**Invariante:** todo lo que está a la izquierda de `lo` es $<x$, y todo lo que está desde `hi` en adelante es $\ge x$:

```
 índice:   0 ... lo-1 | lo ... hi-1 | hi ... n-1
 valor:    todos < x  | sin decidir | todos >= x
```

::: {.tabs}
```c
#include <stddef.h>
#include <stdio.h>

// Primer índice i con a[i] >= x, o n si no existe. Requiere a ordenado.
size_t lower_bound(const int *a, size_t n, int x) {
    size_t lo = 0, hi = n;
    // INVARIANTE: a[0..lo) < x  y  a[hi..n) >= x
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;   // lo <= mid < hi
        if (a[mid] < x) lo = mid + 1;      // a[0..mid] < x
        else            hi = mid;          // a[mid..n) >= x
    }
    return lo;                             // lo == hi: la frontera
}

int main(void) {
    int a[] = {1, 3, 3, 5, 8, 13};
    int xs[] = {0, 3, 5, 6, 13, 20};
    for (int k = 0; k < 6; k++)
        printf("lower_bound(%d) = %zu\n", xs[k], lower_bound(a, 6, xs[k]));
    return 0;
}
```

```cpp
#include <cstddef>
#include <iostream>
#include <vector>

// Primer índice i con a[i] >= x, o a.size() si no existe. Requiere a ordenado.
std::size_t lower_bound(const std::vector<int>& a, int x) {
    std::size_t lo = 0, hi = a.size();
    // INVARIANTE: a[0..lo) < x  y  a[hi..n) >= x
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;   // lo <= mid < hi
        if (a[mid] < x) lo = mid + 1;           // a[0..mid] < x
        else            hi = mid;               // a[mid..n) >= x
    }
    return lo;                                  // lo == hi: la frontera
}

int main() {
    std::vector<int> a{1, 3, 3, 5, 8, 13};
    for (int x : {0, 3, 5, 6, 13, 20})
        std::cout << "lower_bound(" << x << ") = " << lower_bound(a, x) << "\n";
    // Equivale a: std::lower_bound(a.begin(), a.end(), x) - a.begin()
}
```

```rust
/// Primer índice i con a[i] >= x, o a.len() si no existe. Requiere a ordenado.
fn lower_bound(a: &[i32], x: i32) -> usize {
    let (mut lo, mut hi) = (0, a.len());
    // INVARIANTE: a[0..lo) < x  y  a[hi..n) >= x
    while lo < hi {
        let mid = lo + (hi - lo) / 2; // lo <= mid < hi
        if a[mid] < x {
            lo = mid + 1; // a[0..=mid] < x
        } else {
            hi = mid; // a[mid..n) >= x
        }
    }
    lo // lo == hi: la frontera
}

fn main() {
    let a = [1, 3, 3, 5, 8, 13];
    for x in [0, 3, 5, 6, 13, 20] {
        println!("lower_bound({x}) = {}", lower_bound(&a, x));
        assert_eq!(lower_bound(&a, x), a.partition_point(|&v| v < x));
    }
}
```
:::

```{.text .salida}
lower_bound(0) = 0
lower_bound(3) = 1
lower_bound(5) = 3
lower_bound(6) = 4
lower_bound(13) = 5
lower_bound(20) = 6
```

**Traza a mano** con `a = [1, 3, 3, 5, 8, 13]` y `x = 5`:

| Vuelta | `lo` | `hi` | `mid` | `a[mid]` | ¿`a[mid] < 5`? | Acción |
|---|---|---|---|---|---|---|
| 1 | 0 | 6 | 3 | 5 | no | `hi = 3` |
| 2 | 0 | 3 | 1 | 3 | sí | `lo = 2` |
| 3 | 2 | 3 | 2 | 3 | sí | `lo = 3` |
| fin | 3 | 3 | — | — | — | devuelve 3 |

**Demostración de correctitud.** El invariante, en una caja:
$$\boxed{\,a[0..\mathrm{lo})<x\quad\text{y}\quad a[\mathrm{hi}..n)\ge x\,}$$

**Paso 1. Inicialización.** El código empieza con `lo = 0` y `hi = n`. Entonces $a[0..0)$ y $a[n..n)$ están vacíos, y la caja se cumple sin nada que revisar ✓:
$$\mathrm{lo}=0,\qquad\mathrm{hi}=n$$

*Mantenimiento.* Supongo la caja al inicio de una vuelta. Dentro del ciclo, `lo < hi`.

**Paso 2. Reviso que `mid` cae dentro del rango.** El código calcula `mid = lo + (hi - lo) / 2`. Como `hi - lo` $\ge1$, la división entera `(hi - lo) / 2` es $\ge0$ y es menor que `hi - lo` (la mitad de un número $\ge1$, redondeada hacia abajo, es menor que el número). Sumando `lo`:
$$\mathrm{lo}\le\mathrm{mid}<\mathrm{hi}$$
*Con números* (la vuelta 1 de la traza): `lo = 0`, `hi = 6`; `(6 - 0) / 2 = 3`, así que `mid = 3`, y $0\le3<6$ ✓.

*Caso 1: `a[mid] < x`.*

**Paso 3. Uso que el arreglo está ordenado.** Todo `a[j]` con `lo <= j <= mid` cumple `a[j] <= a[mid] < x`:
$$a[\mathrm{lo}..\mathrm{mid}]<x$$

**Paso 4. Uso la caja.** La caja dice $a[0..\mathrm{lo})<x$. Junto ese pedazo con el del paso 3:
$$a[0..\mathrm{mid}+1)<x$$

**Paso 5. Ejecuto la vuelta.** El código hace `lo = mid + 1`. Con el nuevo `lo`, la primera mitad de la caja dice exactamente lo del paso 4. `hi` no cambió, así que la segunda mitad sigue igual ✓.
$$\mathrm{lo}_{\text{nuevo}}=\mathrm{mid}+1$$

*Caso 2: `a[mid] >= x`.*

**Paso 6. Uso que el arreglo está ordenado.** Todo `a[j]` con `j >= mid` cumple `a[j] >= a[mid] >= x`:
$$a[\mathrm{mid}..n)\ge x$$

**Paso 7. Ejecuto la vuelta.** El código hace `hi = mid`. Con el nuevo `hi`, la segunda mitad de la caja dice exactamente lo del paso 6. `lo` no cambió ✓.
$$\mathrm{hi}_{\text{nuevo}}=\mathrm{mid}$$

**Paso 8. Terminación: el ciclo para.** En cada vuelta `hi - lo` baja **estrictamente**: o `lo` sube a `mid + 1 > lo`, o `hi` baja a `mid < hi` (paso 2). Un entero que no es negativo y baja al menos 1 en cada vuelta no puede bajar para siempre. En la traza:
$$\mathrm{hi}-\mathrm{lo}:\quad 6\ \to\ 3\ \to\ 1\ \to\ 0$$

**Paso 9. Terminación: la respuesta.** Al salir, `lo == hi`. Cambio `hi` por `lo` en la caja:
$$a[0..\mathrm{lo})<x\quad\text{y}\quad a[\mathrm{lo}..n)\ge x$$
Todo lo anterior a `lo` es $<x$ y todo desde `lo` es $\ge x$: `lo` es la frontera que buscábamos ✓. $\square$

**Costo:** el rango se reduce a la mitad (o menos) en cada vuelta, así que hay a lo más $\lfloor\lg n\rfloor+1$ vueltas, es decir $O(\lg n)$ (§5).

### 4.4 Un detalle que causa errores reales: el desbordamiento

¿Por qué `mid = lo + (hi - lo) / 2` y no `(lo + hi) / 2`, que es más corto? Porque `lo + hi` puede **pasarse del máximo** que cabe en el tipo entero. Este error estuvo escondido nueve años en la búsqueda binaria de la biblioteca estándar de Java (Bloch, 2006).

::: {.tabs}
```c
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    uint32_t lo = 3000000000u, hi = 4000000000u;   // el máximo de uint32_t es 4294967295
    uint32_t malo  = (lo + hi) / 2;      // lo + hi se pasa de 2^32 y "da la vuelta"
    uint32_t bueno = lo + (hi - lo) / 2; // hi - lo nunca se pasa
    printf("(lo + hi) / 2      = %" PRIu32 "  <- incorrecto\n", malo);
    printf("lo + (hi - lo) / 2 = %" PRIu32 "\n", bueno);
    return 0;
}
```

```cpp
#include <cstdint>
#include <iostream>

int main() {
    std::uint32_t lo = 3000000000u, hi = 4000000000u;  // el máximo de uint32_t es 4294967295
    std::uint32_t malo  = (lo + hi) / 2;      // lo + hi se pasa de 2^32 y "da la vuelta"
    std::uint32_t bueno = lo + (hi - lo) / 2; // hi - lo nunca se pasa
    std::cout << "(lo + hi) / 2      = " << malo << "  <- incorrecto\n";
    std::cout << "lo + (hi - lo) / 2 = " << bueno << "\n";
}
```

```rust
fn main() {
    let (lo, hi): (u32, u32) = (3_000_000_000, 4_000_000_000); // u32::MAX = 4294967295
    // En Rust, `lo + hi` haría PANIC en modo debug ("attempt to add with overflow").
    // Para ver el mismo valor que en C/C++ hay que pedir la vuelta explícitamente:
    let malo = lo.wrapping_add(hi) / 2;
    let bueno = lo + (hi - lo) / 2;
    println!("(lo + hi) / 2      = {malo}  <- incorrecto");
    println!("lo + (hi - lo) / 2 = {bueno}");
}
```
:::

```{.text .salida}
(lo + hi) / 2      = 1352516352  <- incorrecto
lo + (hi - lo) / 2 = 3500000000
```

**Qué pasa en cada lenguaje:**
- **C y C++, enteros sin signo:** "dar la vuelta" está definido (se calcula módulo $2^{32}$), así que el programa **sigue** con un `mid` absurdo, menor que `lo`.
- **C y C++, enteros con signo (`int`):** el desbordamiento es **comportamiento indefinido**. El compilador puede asumir que nunca ocurre, y el resultado es impredecible.
- **Rust:** en modo debug el desbordamiento **detiene el programa** con un *panic*; en modo release da la vuelta, salvo que se use `checked_add` u `overflow-checks`.

✏️ **Ejercicio 0.3.** En la búsqueda binaria, ¿qué pasaría si escribieras `lo = mid` en lugar de `lo = mid + 1`? Busca un arreglo de 2 elementos donde el ciclo no termine.
<details><summary>Solución</summary>

Con `a = [1, 2]` y `x = 5`: `lo = 0`, `hi = 2`, `mid = 1`, `a[1] = 2 < 5`, así que `lo = 1`. Luego `mid = 1 + (2-1)/2 = 1`, `a[1] < 5`, `lo = 1`… **no avanza nunca**. La demostración de terminación usaba que `hi - lo` **decrece estrictamente**; con `lo = mid` eso falla cuando `mid == lo`.
</details>

---

## 5. Potencias y logaritmos

**¿Para qué?** El logaritmo aparece cada vez que un algoritmo **divide el problema a la mitad**: búsqueda binaria, altura de un árbol balanceado, profundidad de merge sort. Es la razón por la que esos algoritmos son tan rápidos.

### 5.1 Potencias de 2 que conviene saber de memoria

| $n$ | $2^n$ | Para ubicarte |
|---|---|---|
| 0 | 1 | |
| 1 | 2 | |
| 8 | 256 | valores de un byte |
| 10 | 1 024 | ≈ mil ($10^3$) |
| 16 | 65 536 | |
| 20 | 1 048 576 | ≈ un millón ($10^6$) |
| 30 | ≈ $1.07\times10^9$ | ≈ mil millones |
| 32 | 4 294 967 296 | valores de `uint32_t` |
| 64 | ≈ $1.8\times10^{19}$ | valores de `uint64_t` |

Regla práctica: **$2^{10}\approx10^3$**. Por eso $2^{20}\approx10^6$ y $2^{30}\approx10^9$.

### 5.2 Qué es un logaritmo

$\lg n$ (logaritmo **base 2**) es **el exponente al que hay que elevar 2 para obtener $n$**:
$$\lg n=x\iff 2^x=n.$$
Ejemplos: $\lg 8=3$ (porque $2^3=8$), $\lg 1024=10$, $\lg 1=0$.

**La interpretación que usaremos siempre:** $\lfloor\lg n\rfloor$ es **cuántas veces puedes dividir $n$ entre 2** (división entera) **antes de llegar a 1**.

::: {.tabs}
```c
#include <stdio.h>

// ¿Cuántas veces puedo dividir n entre 2 antes de llegar a 1?  Es ⌊lg n⌋.
int mitades(unsigned long n) {
    int veces = 0;
    while (n > 1) {
        n /= 2;
        veces++;
    }
    return veces;
}

int main(void) {
    unsigned long ns[] = {1, 2, 3, 8, 1000, 1000000};
    for (int i = 0; i < 6; i++)
        printf("n = %7lu  ->  %2d mitades\n", ns[i], mitades(ns[i]));
    return 0;
}
```

```cpp
#include <bit>
#include <cassert>
#include <cstdio>
#include <initializer_list>

// ¿Cuántas veces puedo dividir n entre 2 antes de llegar a 1?  Es ⌊lg n⌋.
int mitades(unsigned long n) {
    int veces = 0;
    while (n > 1) {
        n /= 2;
        ++veces;
    }
    return veces;
}

int main() {
    for (unsigned long n : {1ul, 2ul, 3ul, 8ul, 1000ul, 1000000ul}) {
        std::printf("n = %7lu  ->  %2d mitades\n", n, mitades(n));
        // C++20 lo trae hecho: bit_width(n) es el número de bits de n = ⌊lg n⌋ + 1
        assert(mitades(n) == static_cast<int>(std::bit_width(n)) - 1);
    }
}
```

```rust
/// ¿Cuántas veces puedo dividir n entre 2 antes de llegar a 1?  Es ⌊lg n⌋.
fn mitades(mut n: u64) -> u32 {
    let mut veces = 0;
    while n > 1 {
        n /= 2;
        veces += 1;
    }
    veces
}

fn main() {
    for n in [1u64, 2, 3, 8, 1000, 1_000_000] {
        println!("n = {:7}  ->  {:2} mitades", n, mitades(n));
        // Rust lo trae hecho: n.ilog2() es ⌊lg n⌋
        assert_eq!(mitades(n), n.ilog2());
    }
}
```
:::

```{.text .salida}
n =       1  ->   0 mitades
n =       2  ->   1 mitades
n =       3  ->   1 mitades
n =       8  ->   3 mitades
n =    1000  ->   9 mitades
n = 1000000  ->  19 mitades
```

**Lee la última línea con calma:** un millón de elementos se reducen a uno en solo **19** mitades. Por eso la búsqueda binaria encuentra cualquier dato entre un millón con unas 20 comparaciones, mientras que revisarlos uno por uno puede costar un millón.

### 5.3 Las reglas de los logaritmos, con números

| Regla | Ejemplo numérico | Por qué es cierta |
|---|---|---|
| $\lg(xy)=\lg x+\lg y$ | $\lg(8\cdot4)=\lg32=5=3+2$ | $2^a\cdot2^b=2^{a+b}$: multiplicar suma exponentes |
| $\lg(x/y)=\lg x-\lg y$ | $\lg(32/4)=\lg 8=3=5-2$ | $2^a/2^b=2^{a-b}$ |
| $\lg(x^k)=k\lg x$ | $\lg(8^2)=\lg64=6=2\cdot3$ | $x^k$ es multiplicar $x$ por sí mismo $k$ veces; por la primera regla, el $\lg$ de ese producto es $\lg x+\dots+\lg x$ ($k$ veces) $=k\lg x$ |
| $\log_b x=\dfrac{\lg x}{\lg b}$ (cambio de base) | $\log_4 64=\dfrac{\lg64}{\lg4}=\dfrac62=3$ ✓ ($4^3=64$) | si $b^y=x$, aplicar $\lg$: $y\lg b=\lg x$ |
| $a^{\lg n}=n^{\lg a}$ | $4^{\lg n}=n^{\lg4}=n^2$ | aplico $\lg$ a cada lado con la regla $\lg(x^k)=k\lg x$: $\lg(a^{\lg n})=\lg n\cdot\lg a$ y $\lg(n^{\lg a})=\lg a\cdot\lg n$, que son el mismo producto; dos números con el mismo $\lg$ son iguales |

**Consecuencia importante del cambio de base:** $\log_{10}n$, $\ln n$ y $\lg n$ difieren solo en una **constante multiplicativa** ($\log_b n=\lg n/\lg b$). Como $O(\cdot)$ ignora constantes, se escribe $O(\log n)$ **sin base**.

**Notación:** $\ln n=\log_e n$ (logaritmo natural). $\lg^2 n$ significa $(\lg n)^2$, que **no** es lo mismo que $\lg\lg n=\lg(\lg n)$.

### 5.4 Qué tan rápido crece cada función

Esta tabla es la razón de existir del curso. Supón una computadora que hace $10^9$ operaciones por segundo:

| $n$ | $\lg n$ | $\sqrt n$ | $n\lg n$ | $n^2$ | $2^n$ |
|---|---|---|---|---|---|
| 10 | 3.3 | 3.2 | 33 | 100 | 1 024 |
| 1 000 | 10 | 32 | ≈ $10^4$ | $10^6$ | ≈ $10^{301}$ |
| $10^6$ | 20 | 1 000 | ≈ $2\times10^7$ | $10^{12}$ | — |
| **Tiempo con $n=10^6$** | 20 ns | 1 µs | **0.02 s** | **17 minutos** | más que la edad del universo |

Pasar de un algoritmo $O(n^2)$ a uno $O(n\lg n)$ convierte 17 minutos en 0.02 segundos. Ninguna computadora más rápida compensa eso.

### 5.5 Un resultado que usaremos: $\lg(n!)=\Theta(n\lg n)$

($n!=1\cdot2\cdots n$. Este resultado da la cota inferior de ordenamiento en la L05.)

**Qué hay que demostrar.** $\Theta$ pide dos cotas (§1): una superior, $\lg(n!)\le c_2\,n\lg n$, y una inferior, $\lg(n!)\ge c_1\,n\lg n$, ambas a partir de algún $n_0$. Las busco por separado. Para comprobar uso $n=8$: $8!=40\,320$ y $\lg 40\,320\approx15.3$.

*Cota superior: agrando cada factor hasta $n$.*

**Paso 1. Agrando cada factor hasta $n$.** Cada uno de los $n$ factores de $n!=1\cdot2\cdots n$ es $\le n$, y en un producto de positivos agrandar un factor no achica el resultado. $n$ factores iguales a $n$ dan $n^n$:
$$n!\le n^n$$

**Paso 2. Aplico $\lg$ a los dos lados.** $\lg$ es creciente (si $u\le v$, entonces $\lg u\le\lg v$), así que el signo no se voltea:
$$\lg(n!)\le\lg(n^n)$$

**Paso 3. Bajo el exponente.** Por la regla $\lg(x^k)=k\lg x$ (§5.3):
$$\lg(n!)\le n\lg n$$
Testigo $c_2=1$ ✓.

**Paso 4. Compruebo con $n=8$.** El paso 1 da $40\,320\le8^8=16\,777\,216$; el paso 3, $15.3\le8\cdot3=24$ ✓.

*Cota inferior: me quedo con la mitad de arriba de los factores.*

**Paso 5. Tiro los factores de abajo.** Tiro los menores que $\frac n2$. Cada uno es $\ge1$, así que tirarlos no aumenta el producto:
$$n!\ge\big\lceil\tfrac n2\big\rceil\cdots n$$

**Paso 6. Achico cada factor que quedó a $\frac n2$.** Quedaron $\lfloor\frac n2\rfloor+1$ factores (de $a$ a $b$ hay $b-a+1$ enteros, y $n-\lceil\frac n2\rceil=\lfloor\frac n2\rfloor$ por §6.1), cada uno $\ge\frac n2$:
$$n!\ge\big(\tfrac n2\big)^{\lfloor n/2\rfloor+1}$$

**Paso 7. Bajo el exponente a $\frac n2$.** Si $n\ge2$, la base $\frac n2$ es $\ge1$, y $\lfloor\frac n2\rfloor+1\ge\frac n2$. Con base $\ge1$, bajar el exponente no agranda la potencia (como $4^5\ge4^4$):
$$n!\ge\big(\tfrac n2\big)^{n/2}$$

**Paso 8. Aplico $\lg$ y bajo el exponente**, igual que en los pasos 2 y 3:
$$\lg(n!)\ge\tfrac n2\,\lg\tfrac n2$$

**Paso 9. Separo la división.** $\lg\frac n2=\lg n-\lg2=\lg n-1$ (§5.3):
$$\lg(n!)\ge\tfrac n2\,(\lg n-1)$$

**Paso 10. Reviso cuándo $\lg n-1\ge\frac12\lg n$.** Resto y veo el signo: $(\lg n-1)-\frac12\lg n=\frac12\lg n-1$. Eso es $\ge0$ cuando $\lg n\ge2$, es decir, cuando $n\ge4$ ($\lg$ es creciente y $\lg4=2$):
$$\lg n-1\ \ge\ \tfrac12\lg n\qquad\text{si } n\ge4$$

**Paso 11. Achico y multiplico.** Con $n\ge4$ cambio $\lg n-1$ por $\frac12\lg n$ (achico un factor de $\frac n2>0$: el $\ge$ se conserva), y $\frac n2\cdot\frac12=\frac n4$:
$$\lg(n!)\ge\tfrac n4\lg n$$
Testigo $c_1=\frac14$, a partir de $n_0=4$ ✓.

**Paso 12. Compruebo con $n=8$.** Los factores de arriba son $4,5,6,7,8$: cinco, y $\lfloor\frac82\rfloor+1=5$ ✓.
$$
\begin{aligned}
\text{paso 5:}&\quad 40\,320\ge4\cdot5\cdot6\cdot7\cdot8=6720\\
\text{paso 7:}&\quad 6720\ge4^4=256\\
\text{paso 8:}&\quad 15.3\ge4\cdot\lg4=8\\
\text{paso 11:}&\quad 15.3\ge\tfrac84\cdot3=6\ ✓
\end{aligned}
$$

**Conclusión.** Con las dos cotas, $\frac14\,n\lg n\le\lg(n!)\le 1\cdot n\lg n$ para todo $n\ge4$: testigos $c_1=\frac14$, $c_2=1$, $n_0=4$. Por lo tanto $\lg(n!)=\Theta(n\lg n)$. $\square$

✏️ **Ejercicio 0.4.** Ordena de menor a mayor crecimiento: $n^{1/2}$, $\lg^3 n$, $2^{\lg n}$, $n\lg n$, $1.01^n$, $\lg\lg n$, $n!$, $4^{\lg n}$.
<details><summary>Solución</summary>

Primero simplifica con las reglas de §5.3: $2^{\lg n}=n$ y $4^{\lg n}=n^2$. Usa además dos hechos que se demuestran en la L01, §3.3.10: cualquier potencia de $\lg n$ crece más lento que cualquier $n^\varepsilon$ con $\varepsilon>0$, y todo polinomio crece más lento que $r^n$ con $r>1$.

**Orden:** $\lg\lg n\ \prec\ \lg^3 n\ \prec\ n^{1/2}\ \prec\ 2^{\lg n}=n\ \prec\ n\lg n\ \prec\ 4^{\lg n}=n^2\ \prec\ 1.01^n\ \prec\ n!$.
</details>

---

## 6. Pisos, techos y la división entera de los lenguajes

**¿Para qué?** Cuando un algoritmo parte un arreglo de $n$ elementos "a la mitad", las mitades miden $\lfloor n/2\rfloor$ y $\lceil n/2\rceil$. Y la división entera de C, C++ y Rust **no** siempre es el piso: es una fuente clásica de errores.

### 6.1 Definiciones

- **Piso** $\lfloor x\rfloor$: el mayor entero $\le x$. $\ \lfloor3.7\rfloor=3$, $\lfloor5\rfloor=5$, $\lfloor-3.7\rfloor=-4$.
- **Techo** $\lceil x\rceil$: el menor entero $\ge x$. $\ \lceil3.2\rceil=4$, $\lceil5\rceil=5$, $\lceil-3.2\rceil=-3$.

Propiedades útiles: $x-1<\lfloor x\rfloor\le x\le\lceil x\rceil<x+1$; para enteros $n$, $\lfloor n/2\rfloor+\lceil n/2\rceil=n$; un entero $n\ge1$ se escribe con $\lfloor\lg n\rfloor+1$ bits.

### 6.2 Cuidado: `/` en C, C++ y Rust trunca hacia cero

Para números **positivos**, `a / b` es $\lfloor a/b\rfloor$. Para **negativos**, no: los tres lenguajes **truncan hacia cero**, y por eso `-7 / 2` da `-3`, no $\lfloor-3.5\rfloor=-4$. Y el residuo `%` toma el signo del dividendo.

::: {.tabs}
```c
#include <stdio.h>

// ⌊a / b⌋ para b > 0, también con a negativo
long piso_div(long a, long b) {
    long q = a / b;
    if ((a % b != 0) && (a < 0)) q--;   // si truncó hacia arriba, corrige
    return q;
}

// ⌈a / b⌉ para a >= 0, b > 0: el truco clásico
long techo_div(long a, long b) { return (a + b - 1) / b; }

int main(void) {
    printf("7 / 2 = %ld\n", 7L / 2);
    printf("-7 / 2 = %ld\n", -7L / 2);
    printf("-7 %% 2 = %ld\n", -7L % 2);
    printf("piso(-7 / 2) = %ld\n", piso_div(-7, 2));
    printf("techo(7 / 2) = %ld\n", techo_div(7, 2));
    return 0;
}
```

```cpp
#include <iostream>

// ⌊a / b⌋ para b > 0, también con a negativo
long piso_div(long a, long b) {
    long q = a / b;
    if ((a % b != 0) && (a < 0)) --q;   // si truncó hacia arriba, corrige
    return q;
}

// ⌈a / b⌉ para a >= 0, b > 0: el truco clásico
long techo_div(long a, long b) { return (a + b - 1) / b; }

int main() {
    std::cout << "7 / 2 = " << 7L / 2 << "\n";
    std::cout << "-7 / 2 = " << -7L / 2 << "\n";
    std::cout << "-7 % 2 = " << -7L % 2 << "\n";
    std::cout << "piso(-7 / 2) = " << piso_div(-7, 2) << "\n";
    std::cout << "techo(7 / 2) = " << techo_div(7, 2) << "\n";
}
```

```rust
fn main() {
    println!("7 / 2 = {}", 7i64 / 2);
    println!("-7 / 2 = {}", -7i64 / 2);
    println!("-7 % 2 = {}", -7i64 % 2);
    // Rust trae la división "euclidiana": para b > 0 coincide con el piso
    println!("piso(-7 / 2) = {}", (-7i64).div_euclid(2));
    // y para enteros sin signo, el techo:
    println!("techo(7 / 2) = {}", 7u64.div_ceil(2));
}
```
:::

```{.text .salida}
7 / 2 = 3
-7 / 2 = -3
-7 % 2 = -1
piso(-7 / 2) = -4
techo(7 / 2) = 4
```

**Por qué funciona `(a + b - 1) / b`** (con $a\ge0$ y $b>0$). Para positivos, `/` es el piso, así que hay que ver que $\big\lfloor\frac{a+b-1}{b}\big\rfloor=\big\lceil\frac ab\big\rceil$.

**Paso 1. Escribo la división con residuo.** Cociente $q$ y residuo $r$, con $0\le r<b$:
$$a=q\,b+r$$

*Caso 1: $r=0$ ($b$ divide a $a$).*

**Paso 2. Sumo $b-1$ a los dos lados.**
$$a+b-1=q\,b+(b-1)$$

**Paso 3. Divido.** $b-1$ es un residuo válido ($0\le b-1<b$), así que la división entera da $q$. Como $\frac ab=q$ exacto, su techo también es $q$:
$$\Big\lfloor\frac{a+b-1}{b}\Big\rfloor=q=\Big\lceil\frac ab\Big\rceil$$

*Caso 2: $r\ge1$.*

**Paso 4. Sumo $b-1$ a los dos lados y reacomodo.** El orden de los sumandos no cambia la suma: $r+b-1=b+(r-1)$.
$$a+b-1=q\,b+b+(r-1)$$

**Paso 5. Saco $b$ de factor común.** $q\,b+b=(q+1)\,b$, por la distributiva al revés:
$$a+b-1=(q+1)\,b+(r-1)$$

**Paso 6. Divido.** $r-1$ es un residuo válido ($0\le r-1<b$), así que la división entera da $q+1$. Como $\frac ab=q+\frac rb$ está estrictamente entre $q$ y $q+1$, su techo es $q+1$:
$$\Big\lfloor\frac{a+b-1}{b}\Big\rfloor=q+1=\Big\lceil\frac ab\Big\rceil$$

**Paso 7. Compruebo con números.** $a=8$, $b=2$ (caso 1): $9/2=4=\lceil4\rceil$. $a=7$, $b=2$ (caso 2): $8/2=4=\lceil3.5\rceil$ ✓.

---

## 7. Sumatorias

**¿Para qué?** El costo de un algoritmo es la **suma** de los costos de sus pasos. Casi todo análisis termina en una sumatoria que hay que simplificar.

### 7.1 La notación $\Sigma$ es un ciclo `for`

$$\sum_{i=1}^{n} f(i)\;=\;f(1)+f(2)+\dots+f(n)$$
se lee exactamente como el código: `s = 0; for i in 1..=n: s += f(i)`. La $i$ es la variable del ciclo, los números de abajo y arriba son el inicio y el fin (**incluido**), y $f(i)$ es lo que se suma en cada vuelta.

::: {.tabs}
```c
#include <stdio.h>

int main(void) {
    int n = 10;
    long s = 0;
    for (int i = 1; i <= n; i++) s += (long)i * i;     // Σ_{i=1}^{n} i²
    printf("ciclo:   %ld\n", s);
    printf("formula: %ld\n", (long)n * (n + 1) * (2 * n + 1) / 6);
    return 0;
}
```

```cpp
#include <iostream>

int main() {
    int n = 10;
    long s = 0;
    for (int i = 1; i <= n; ++i) s += static_cast<long>(i) * i;   // Σ_{i=1}^{n} i²
    std::cout << "ciclo:   " << s << "\n";
    std::cout << "formula: " << static_cast<long>(n) * (n + 1) * (2 * n + 1) / 6 << "\n";
}
```

```rust
fn main() {
    let n: i64 = 10;
    let s: i64 = (1..=n).map(|i| i * i).sum(); // Σ_{i=1}^{n} i²
    println!("ciclo:   {s}");
    println!("formula: {}", n * (n + 1) * (2 * n + 1) / 6);
}
```
:::

```{.text .salida}
ciclo:   385
formula: 385
```

La forma cerrada que usa el programa es $\sum_{i=1}^{n}i^2=\frac{n(n+1)(2n+1)}{6}$ (con $n=10$: $\frac{10\cdot11\cdot21}{6}=385$, como en la salida). La gracia de una **forma cerrada** es que cuesta $O(1)$ evaluarla, en lugar de $O(n)$, y sobre todo que nos dice **cómo crece**: $\sum i^2=\Theta(n^3)$.

### 7.2 Suma aritmética: el truco de Gauss

$S=1+2+\dots+n$. Escríbela dos veces, una al derecho y otra al revés, y suma columna por columna:
$$
\begin{array}{rcccccc}
S &=& 1 &+& 2 &+\cdots+& n\\
S &=& n &+& (n-1) &+\cdots+& 1\\\hline
2S &=& (n+1) &+& (n+1) &+\cdots+& (n+1)
\end{array}
$$

**Paso 1. Cada columna suma $n+1$.** En la columna $i$, arriba está $i$ y abajo $n+1-i$ (la de abajo empieza en $n$ y baja 1 cada vez que la de arriba sube 1):
$$i+(n+1-i)=n+1$$

**Paso 2. Hay $n$ columnas.** Cada una suma $n+1$:
$$2S=n(n+1)$$

**Paso 3. Divido entre 2.**
$$S=\frac{n(n+1)}{2}=\Theta(n^2)$$
*Con números* ($n=4$): $S=1+2+3+4=10$; las columnas son $1+4$, $2+3$, $3+2$, $4+1$, todas $5$; $2S=4\cdot5=20$, y $S=10$ ✓.

**Dónde aparece:** un ciclo doble `for i < n: for j < i:` hace $0+1+\dots+(n-1)=\frac{n(n-1)}2$ vueltas en total. Es el costo de insertion sort en el peor caso.

### 7.3 Suma geométrica: el truco de multiplicar y restar

$S=1+r+r^2+\dots+r^k$, con $r\ne1$. Multiplica por $r$ y resta:
$$
\begin{array}{rcl}
rS &=& r+r^2+\dots+r^k+r^{k+1}\\
S &=& 1+r+r^2+\dots+r^k\\\hline
rS-S &=& r^{k+1}-1\qquad(\text{todo lo de en medio se cancela})
\end{array}
$$
*Por qué se cancela lo de en medio:* los términos $r, r^2,\dots,r^k$ aparecen en **los dos** renglones, así que al restar se van. Solo sobreviven $r^{k+1}$ (que solo está arriba) y $1$ (que solo está abajo, y por eso entra con signo menos).

Ahora despejo $S$.

**Paso 1. Parto de la resta.**
$$rS-S=r^{k+1}-1$$

**Paso 2. Saco $S$ de factor común.** Por la distributiva al revés, $rS-S=S\,(r-1)$:
$$S\,(r-1)=r^{k+1}-1$$

**Paso 3. Divido entre $r-1$.** Se puede porque $r\ne1$, así que $r-1\ne0$ (para eso estaba esa condición):
$$S=\frac{r^{k+1}-1}{r-1}$$

**Paso 4. Compruebo con números.** Con $r=2$, $k=3$, $S=1+2+4+8=15$. El paso 1 da $30-15=15$; el paso 2, $15\cdot1=15$; el paso 3, $\frac{16-1}{1}=15$ ✓.

**Los dos casos que importan:**

- **$r=2$:** $1+2+4+\dots+2^k=2^{k+1}-1$. En binario se ve: $1+2+4+8=1111_2$, y sumarle 1 da $10000_2=16$. **La suma es menor que el doble del último término.**
- **$r=\frac12$:** $1+\frac12+\frac14+\dots<2$, por muchos términos que sumes. Es como caminar hacia una pared avanzando cada vez la mitad de lo que falta: nunca la alcanzas. **La suma es menor que el doble del primer término.**

*La cuenta con $r=\frac12$:*

**Paso 1. Sustituyo $r=\frac12$ en la fórmula.**
$$S=\frac{(1/2)^{k+1}-1}{\frac12-1}$$

**Paso 2. Cambio el signo arriba y abajo.** Abajo, $\frac12-1=-\frac12$. Multiplico arriba y abajo por $-1$, que no cambia la fracción (es multiplicar por $\frac{-1}{-1}=1$):
$$S=\frac{1-(1/2)^{k+1}}{1/2}$$

**Paso 3. Divido entre $\frac12$.** Dividir entre $\frac12$ es multiplicar por 2; reparto el 2 con la distributiva:
$$S=2-2\cdot\big(\tfrac12\big)^{k+1}$$

**Paso 4. Simplifico.** Un factor $\frac12$ se cancela con el 2:
$$S=2-\big(\tfrac12\big)^{k}$$
Es 2 menos algo positivo: menor que 2 ✓.

**Paso 5. Compruebo con $k=2$**, donde $S=1+\frac12+\frac14=1.75$. El paso 1 da $\frac{0.125-1}{-0.5}=1.75$; el paso 2, $\frac{0.875}{0.5}=1.75$; el paso 4, $2-0.25=1.75$ ✓.

> **Regla de oro:** en una suma geométrica, **el término más grande domina**. Si la razón es $>1$, la suma es $\Theta(\text{último término})$; si es $<1$, es $\Theta(\text{primer término})$. Es *la* idea detrás del análisis del arreglo dinámico (L01).

### 7.4 Una suma que usaremos para los heaps: $\sum k/2^k=2$

En la L06 demostraremos que construir un heap cuesta $O(n)$ gracias a esta suma:
$$\sum_{k=1}^{\infty}\frac{k}{2^k}=\frac12+\frac24+\frac38+\frac4{16}+\dots=2.$$

**Demostración (el mismo truco de §7.3).** Sea $S=\sum_{k\ge1}k\,x^k$ con $0<x<1$. Entonces:
$$
\begin{array}{rcl}
S &=& 1x+2x^2+3x^3+4x^4+\cdots\\
xS &=& \phantom{1x+{}}1x^2+2x^3+3x^4+\cdots\\\hline
S-xS &=& 1x+1x^2+1x^3+1x^4+\cdots
\end{array}
$$
**Paso 1. Resto columna por columna.** Cada potencia $x^j$ aparece arriba con coeficiente $j$ y abajo con coeficiente $j-1$ (el renglón de abajo es el de arriba corrido un lugar). Restando, queda un 1 delante de cada potencia:
$$j\,x^j-(j-1)\,x^j=x^j$$

**Paso 2. Saco $x$ de factor común** en lo que quedó:
$$S-xS=x\,(1+x+x^2+\cdots)$$

**Paso 3. Sumo la geométrica.** Con $0<x<1$, §7.3 da $1+x+\dots+x^N=\frac{1-x^{N+1}}{1-x}$ (llamo $N$ al último exponente, para no confundirlo con el $k$ de la suma). $x^{N+1}$ se hace tan chico como quieras al crecer $N$, así que la suma infinita vale $\frac{1}{1-x}$:
$$S-xS=\frac{x}{1-x}$$

**Paso 4. Saco $S$ de factor común.** $S-xS=S\,(1-x)$:
$$S\,(1-x)=\frac{x}{1-x}$$

**Paso 5. Divido entre $1-x$**, que no es 0 porque $x<1$:
$$S=\frac{x}{(1-x)^2}$$

**Paso 6. Sustituyo $x=\frac12$.** $(1-x)^2=\frac14$, y $\frac{1/2}{1/4}=\frac12\cdot4=2$:
$$S=2\qquad\square$$

**Paso 7. Compruebo con $x=\frac12$**, usando $S=2$ solo para revisar que cada paso es coherente. El paso 3 da $2-1=\frac{1/2}{1/2}=1$; el paso 4, $2\cdot\frac12=1$; el paso 5, $\frac{1/2}{1/4}=2$ ✓.

(Restar sumas infinitas término a término se vale aquí porque, con $0<x<1$, las dos sumas convergen; el programa de abajo lo confirma con números.)

Veamos cómo se acercan las sumas parciales:

::: {.tabs}
```c
#include <stdio.h>

int main(void) {
    double s = 0, pot = 1;            // pot = 2^k
    for (int k = 1; k <= 30; k++) {
        pot *= 2;
        s += k / pot;
        if (k == 5 || k == 10 || k == 20 || k == 30)
            printf("hasta k = %2d: %.6f\n", k, s);
    }
    return 0;
}
```

```cpp
#include <cstdio>

int main() {
    double s = 0, pot = 1;            // pot = 2^k
    for (int k = 1; k <= 30; ++k) {
        pot *= 2;
        s += k / pot;
        if (k == 5 || k == 10 || k == 20 || k == 30)
            std::printf("hasta k = %2d: %.6f\n", k, s);
    }
}
```

```rust
fn main() {
    let (mut s, mut pot) = (0.0f64, 1.0f64); // pot = 2^k
    for k in 1..=30 {
        pot *= 2.0;
        s += k as f64 / pot;
        if [5, 10, 20, 30].contains(&k) {
            println!("hasta k = {:2}: {:.6}", k, s);
        }
    }
}
```
:::

```{.text .salida}
hasta k =  5: 1.781250
hasta k = 10: 1.988281
hasta k = 20: 1.999979
hasta k = 30: 2.000000
```

### 7.5 La suma armónica crece como el logaritmo

$H_n=1+\frac12+\frac13+\dots+\frac1n$. Aparece en el costo esperado de quicksort (L05) y en el hashing (L04).

*Cota superior: bloques que empiezan en potencias de 2.*

**Paso 1. Agrupo los términos en bloques.** El bloque $j$ va de $\frac1{2^j}$ a $\frac1{2^{j+1}-1}$ y tiene $2^j$ términos. Si el último bloque queda incompleto, lo completo con términos de más; son positivos, así que la suma solo crece:
$$
\begin{aligned}
H_n\le{}&\underbrace{1}_{j=0}+\underbrace{\tfrac12+\tfrac13}_{j=1}\\
&+\underbrace{\tfrac14+\dots+\tfrac17}_{j=2}+\underbrace{\tfrac18+\dots+\tfrac1{15}}_{j=3}+\cdots
\end{aligned}
$$

**Paso 2. Acoto cada bloque por 1.** Cada término del bloque $j$ es $\le\frac1{2^j}$ (el primero es el más grande), y hay $2^j$ términos:
$$\tfrac1{2^j}+\dots+\tfrac1{2^{j+1}-1}\ \le\ 2^j\cdot\tfrac1{2^j}=1$$

**Paso 3. Cuento los bloques.** Hay uno por cada $j$ con $2^j\le n$ (si $2^j>n$, el bloque ya no tiene términos de $H_n$): $j=0,1,\dots,\lfloor\lg n\rfloor$, que son $\lfloor\lg n\rfloor+1$ bloques (§5.2). Cada uno aporta a lo más 1:
$$H_n\le\lfloor\lg n\rfloor+1\le\lg n+1$$

*Cota inferior: bloques que terminan en potencias de 2.*

**Paso 4. Agrupo distinto.** Aparto el 1. El bloque $j\ge1$ va de $\frac1{2^{j-1}+1}$ a $\frac1{2^j}$ y tiene $2^{j-1}$ términos. Me quedo solo con los bloques completos; lo que sobra es positivo, así que tirarlo no aumenta la suma:
$$
\begin{aligned}
H_n\ge{}&1+\underbrace{\tfrac12}_{j=1}\\
&+\underbrace{\tfrac13+\tfrac14}_{j=2}+\underbrace{\tfrac15+\dots+\tfrac18}_{j=3}+\cdots
\end{aligned}
$$

**Paso 5. Acoto cada bloque por $\frac12$.** Cada término del bloque $j$ es $\ge\frac1{2^j}$ (el último es el más chico), y hay $2^{j-1}$ términos:
$$\tfrac1{2^{j-1}+1}+\dots+\tfrac1{2^j}\ \ge\ 2^{j-1}\cdot\tfrac1{2^j}=\tfrac12$$

**Paso 6. Cuento los bloques completos.** El bloque $j$ está completo dentro de $H_n$ si $2^j\le n$, o sea para $j=1,\dots,\lfloor\lg n\rfloor$. Cada uno aporta al menos $\frac12$, más el 1 que aparté:
$$H_n\ge1+\tfrac12\lfloor\lg n\rfloor$$

**Paso 7. Compruebo con $n=8$**, donde $\lg 8=3$: $H_8\approx2.72$; la cota superior da $3+1=4\ge2.72$ y la inferior $1+\frac12\cdot3=2.5\le2.72$ ✓.

Por lo tanto $H_n=\Theta(\lg n)$. (Con cálculo se obtiene algo más fino: $H_n\approx\ln n+0.5772$.)

::: {.tabs}
```c
#include <math.h>
#include <stdio.h>

int main(void) {
    long ns[] = {10, 100, 1000, 1000000};
    for (int t = 0; t < 4; t++) {
        double h = 0;
        for (long i = 1; i <= ns[t]; i++) h += 1.0 / i;
        printf("n = %7ld   H_n = %.4f   ln n = %.4f\n", ns[t], h, log((double)ns[t]));
    }
    return 0;
}
```

```cpp
#include <cmath>
#include <cstdio>
#include <initializer_list>

int main() {
    for (long n : {10L, 100L, 1000L, 1000000L}) {
        double h = 0;
        for (long i = 1; i <= n; ++i) h += 1.0 / i;
        std::printf("n = %7ld   H_n = %.4f   ln n = %.4f\n", n, h, std::log(static_cast<double>(n)));
    }
}
```

```rust
fn main() {
    for n in [10i64, 100, 1000, 1_000_000] {
        let mut h = 0.0f64;
        for i in 1..=n {
            h += 1.0 / i as f64;
        }
        println!("n = {:7}   H_n = {:.4}   ln n = {:.4}", n, h, (n as f64).ln());
    }
}
```
:::

```{.text .salida}
n =      10   H_n = 2.9290   ln n = 2.3026
n =     100   H_n = 5.1874   ln n = 4.6052
n =    1000   H_n = 7.4855   ln n = 6.9078
n = 1000000   H_n = 14.3927   ln n = 13.8155
```

La diferencia $H_n-\ln n$ se acerca a $0.5772\ldots$ (la constante de Euler–Mascheroni).

### 7.6 Sumas telescópicas

Si cada término es una **diferencia** de dos cosas consecutivas, casi todo se cancela:
$$\sum_{i=0}^{n-1}(a_{i+1}-a_i)=(a_1-a_0)+(a_2-a_1)+\dots+(a_n-a_{n-1})=a_n-a_0.$$
**Ejemplo:** $\frac1{i(i+1)}=\frac1i-\frac1{i+1}$. (Compruébalo restando con común denominador: $\frac1i-\frac1{i+1}=\frac{(i+1)-i}{i(i+1)}=\frac1{i(i+1)}$.) Entonces cada término es una diferencia de dos cosas consecutivas y se cancelan en cadena:
$$
\begin{aligned}
\sum_{i=1}^{n}\frac1{i(i+1)}&=\Big(1-\tfrac12\Big)+\Big(\tfrac12-\tfrac13\Big)+\dots+\Big(\tfrac1n-\tfrac1{n+1}\Big)\\
&=1-\frac1{n+1}.
\end{aligned}
$$
*Con números* ($n=3$): $\frac12+\frac16+\frac1{12}=\frac{6+2+1}{12}=\frac9{12}=\frac34$, y $1-\frac14=\frac34$ ✓.
Esta es la herramienta del **método del potencial** en la L01.

✏️ **Ejercicio 0.5.** Demuestra que $\sum_{i=1}^{n}\lg i=\Theta(n\lg n)$.
<details><summary>Solución</summary>

La regla $\lg(xy)=\lg x+\lg y$ de §5.3, leída de derecha a izquierda, convierte una suma de logaritmos en el logaritmo de un producto. Aplicada una vez por cada término: $\lg1+\lg2+\dots+\lg n=\lg(1\cdot2\cdots n)=\lg(n!)$, que es $\Theta(n\lg n)$ por §5.5. *Con números* ($n=4$): $\lg1+\lg2+\lg3+\lg4\approx0+1+1.585+2=4.585$, y $\lg(4!)=\lg24\approx4.585$ ✓.
</details>

---

## 8. Recurrencias: el costo de un algoritmo recursivo

**¿Para qué?** Para saber cuánto cuesta un algoritmo de divide y vencerás (merge sort, búsqueda binaria, recorridos de árboles) hay que resolver una **recurrencia**: una fórmula que define el costo en términos del costo de problemas más chicos.

### 8.1 Del código a la recurrencia

Este programa suma un arreglo **partiéndolo a la mitad** recursivamente. Además cuenta dos cosas: cuántas **llamadas** hace y cuánto **trabajo** total hace, si cada llamada "toca" todos los elementos de su rango (como hace merge sort al mezclar).

::: {.tabs}
```c
#include <stdio.h>

static long llamadas, trabajo;

// Suma a[lo..hi) partiendo el rango a la mitad. Requiere hi - lo >= 1.
long suma_mitades(const int *a, int lo, int hi) {
    llamadas++;
    trabajo += hi - lo;                      // esta llamada "toca" hi - lo elementos
    if (hi - lo == 1) return a[lo];          // caso base
    int mid = lo + (hi - lo) / 2;
    return suma_mitades(a, lo, mid) + suma_mitades(a, mid, hi);
}

int lg(int n) { int k = 0; while (n > 1) { n /= 2; k++; } return k; }

int main(void) {
    static int a[1024];
    for (int i = 0; i < 1024; i++) a[i] = 1;
    int ns[] = {1, 2, 8, 1024};
    for (int t = 0; t < 4; t++) {
        int n = ns[t];
        llamadas = trabajo = 0;
        suma_mitades(a, 0, n);
        printf("n = %4d   llamadas = %4ld (2n-1 = %4d)   trabajo = %5ld (n(lg n + 1) = %5d)\n",
               n, llamadas, 2 * n - 1, trabajo, n * (lg(n) + 1));
    }
    return 0;
}
```

```cpp
#include <cstdio>
#include <vector>

static long llamadas, trabajo;

// Suma a[lo..hi) partiendo el rango a la mitad. Requiere hi - lo >= 1.
long suma_mitades(const std::vector<int>& a, int lo, int hi) {
    ++llamadas;
    trabajo += hi - lo;                      // esta llamada "toca" hi - lo elementos
    if (hi - lo == 1) return a[lo];          // caso base
    int mid = lo + (hi - lo) / 2;
    return suma_mitades(a, lo, mid) + suma_mitades(a, mid, hi);
}

int lg(int n) { int k = 0; while (n > 1) { n /= 2; ++k; } return k; }

int main() {
    std::vector<int> a(1024, 1);
    for (int n : std::vector<int>{1, 2, 8, 1024}) {
        llamadas = trabajo = 0;
        suma_mitades(a, 0, n);
        std::printf("n = %4d   llamadas = %4ld (2n-1 = %4d)   trabajo = %5ld (n(lg n + 1) = %5d)\n",
                    n, llamadas, 2 * n - 1, trabajo, n * (lg(n) + 1));
    }
}
```

```rust
/// Suma a[lo..hi) partiendo el rango a la mitad. Requiere hi - lo >= 1.
/// `cuenta` = (llamadas, trabajo)
fn suma_mitades(a: &[i32], lo: usize, hi: usize, cuenta: &mut (u64, u64)) -> i64 {
    cuenta.0 += 1;
    cuenta.1 += (hi - lo) as u64; // esta llamada "toca" hi - lo elementos
    if hi - lo == 1 {
        return a[lo] as i64; // caso base
    }
    let mid = lo + (hi - lo) / 2;
    suma_mitades(a, lo, mid, cuenta) + suma_mitades(a, mid, hi, cuenta)
}

fn main() {
    let a = vec![1; 1024];
    for n in [1usize, 2, 8, 1024] {
        let mut cuenta = (0, 0);
        suma_mitades(&a, 0, n, &mut cuenta);
        let lg = n.ilog2() as usize;
        println!(
            "n = {:4}   llamadas = {:4} (2n-1 = {:4})   trabajo = {:5} (n(lg n + 1) = {:5})",
            n, cuenta.0, 2 * n - 1, cuenta.1, n * (lg + 1)
        );
    }
}
```
:::

```{.text .salida}
n =    1   llamadas =    1 (2n-1 =    1)   trabajo =     1 (n(lg n + 1) =     1)
n =    2   llamadas =    3 (2n-1 =    3)   trabajo =     4 (n(lg n + 1) =     4)
n =    8   llamadas =   15 (2n-1 =   15)   trabajo =    32 (n(lg n + 1) =    32)
n = 1024   llamadas = 2047 (2n-1 = 2047)   trabajo = 11264 (n(lg n + 1) = 11264)
```

Si $T(n)$ es el trabajo para un rango de tamaño $n$, el código dice **literalmente**:
$$
\begin{aligned}
T(n)&=\underbrace{2\,T(n/2)}_{\text{dos llamadas de la mitad}}+\underbrace{n}_{\text{trabajo propio}}\\
T(1)&=1
\end{aligned}
$$
Ahora veamos cómo **resolverla** sin correr el programa.

### 8.2 Método del árbol de recursión: dibujar y sumar por niveles

Dibuja cada llamada como un nodo con el trabajo **propio** que hace (para $n=8$):

```
nivel 0:                 [8]                         → 1 nodo  × 8 = 8
nivel 1:          [4]            [4]                 → 2 nodos × 4 = 8
nivel 2:      [2]     [2]    [2]     [2]             → 4 nodos × 2 = 8
nivel 3:     [1] [1] [1] [1] [1] [1] [1] [1]         → 8 nodos × 1 = 8
                                                        total = 4 × 8 = 32 ✓
```

En general, para $n$ potencia de 2:

| Nivel $i$ | Nodos | Tamaño de cada uno | Trabajo del nivel |
|---|---|---|---|
| $0$ | $1$ | $n$ | $n$ |
| $1$ | $2$ | $n/2$ | $n$ |
| $i$ | $2^i$ | $n/2^i$ | $2^i\cdot\frac{n}{2^i}=n$ |
| $\lg n$ (hojas) | $n$ | $1$ | $n$ |

Hay $\lg n+1$ niveles (§5.2: las mitades), cada uno con trabajo $n$:
$$T(n)=n(\lg n+1)=\Theta(n\lg n).$$
Es exactamente lo que imprimió el programa. Y el número de **llamadas** es el número de nodos: $1+2+4+\dots+n=2n-1$ (suma geométrica, §7.3).

### 8.3 Método de sustitución: demostrar la fórmula adivinada

El árbol sirve para **adivinar**; para **demostrar** se usa inducción (§3). Quiero demostrar $T(n)=n(\lg n+1)$ para $n$ potencia de 2.

Esta inducción avanza sobre las potencias de 2, $1,2,4,8,\dots$ El "dominó anterior" de $n$ no es $n-1$ sino $n/2$, porque la recurrencia relaciona $T(n)$ con $T(n/2)$.

**Paso 1. Pruebo con números chicos.** Calculo $T$ con la recurrencia y lo comparo con la fórmula:
$$
\begin{aligned}
n=1:&\quad T(1)=1 &&\text{y}\quad 1\cdot(0+1)=1\ ✓\\
n=2:&\quad T(2)=2\cdot1+2=4 &&\text{y}\quad 2\cdot(1+1)=4\ ✓\\
n=4:&\quad T(4)=2\cdot4+4=12 &&\text{y}\quad 4\cdot(2+1)=12\ ✓
\end{aligned}
$$

**Paso 2. La base.** $n=1$: $T(1)=1$ y $1\cdot(\lg1+1)=1\cdot(0+1)=1$ ✓.

**Paso 3. Supongo que vale para la mitad.** Para una potencia de 2 $n\ge2$ cualquiera, supongo la fórmula con $n/2$ en lugar de $n$:
$$\boxed{\,T(n/2)=\tfrac n2\big(\lg\tfrac n2+1\big)\,}\qquad\text{(HI)}$$

**Paso 4. Escribo lo que quiero.**
$$T(n)\ \overset{?}{=}\ n(\lg n+1)$$

**Paso 5. Escribo la recurrencia y busco la caja.** La recurrencia vale para $n\ge2$, y adentro está $T(n/2)$, el lado izquierdo de la caja:
$$T(n)=2\,\boxed{\,T(n/2)\,}+n$$

**Paso 6. Uso la HI.** Cambio la caja por lo que la HI dice que vale:
$$T(n)=2\cdot\tfrac n2\big(\lg\tfrac n2+1\big)+n$$

**Paso 7. Multiplico** $2\cdot\frac n2=n$:
$$T(n)=n\big(\lg\tfrac n2+1\big)+n$$

**Paso 8. Separo la división y cancelo.** $\lg\frac n2=\lg n-1$ (§5.3), así que $\lg\frac n2+1=\lg n$:
$$T(n)=n\lg n+n$$

**Paso 9. Saco $n$ de factor común.**
$$T(n)=n\,(\lg n+1)$$
Es la meta del paso 4 ✓.

**Paso 10. Compruebo con $n=8$**, donde la HI da $T(4)=4\cdot(2+1)=12$. El paso 5 da $2\cdot12+8=32$; el paso 8, $24+8=32$; el paso 9, $8\cdot4=32$ ✓.

**Conclusión.** El paso 2 tiró el primer dominó ($n=1$), y los pasos 3 a 9 mostraron que cada potencia de 2 hereda la fórmula de su mitad. Por inducción sobre las potencias de 2, $T(n)=n(\lg n+1)$. $\square$ El valor $32$ coincide con el árbol de §8.2.

### 8.4 El Teorema Maestro: la receta rápida

Para recurrencias de la forma
$$T(n)=a\,T(n/b)+f(n),$$
con $a\ge1$ subproblemas, cada uno de tamaño $n/b$, y $f(n)$ el trabajo propio, la receta tiene tres pasos:

1. Calcula $n^{\log_b a}$: es el trabajo de las **hojas** del árbol.
2. Compara $f(n)$ con $n^{\log_b a}$.
3. Elige el caso:

| Caso | Comparación | Quién domina | Resultado |
|---|---|---|---|
| 1 | $f(n)$ es polinomialmente **menor**: $f(n)=O(n^{\log_b a-\varepsilon})$ | las hojas | $T(n)=\Theta(n^{\log_b a})$ |
| 2 | $f(n)=\Theta(n^{\log_b a}\lg^k n)$, con $k\ge0$ | todos los niveles por igual | $T(n)=\Theta(n^{\log_b a}\lg^{k+1}n)$ |
| 3 | $f(n)$ es polinomialmente **mayor**: $f(n)=\Omega(n^{\log_b a+\varepsilon})$, y además $a\,f(n/b)\le c\,f(n)$ con $c<1$ | la raíz | $T(n)=\Theta(f(n))$ |

**Por qué $n^{\log_b a}$ es el trabajo de las hojas.** Cada nivel divide el tamaño entre $b$, así que el árbol tiene $\log_b n$ niveles bajo la raíz. Cada nivel multiplica los nodos por $a$, así que hay $a^{\log_b n}$ hojas. Y $a^{\log_b n}=n^{\log_b a}$: es la regla $a^{\lg n}=n^{\lg a}$ de §5.3 en base $b$ (aplica $\log_b$ a los dos lados y los dos dan $\log_b n\cdot\log_b a$).

**Cuatro ejemplos resueltos:**

| Recurrencia | Algoritmo | Paso 1: $n^{\log_b a}$ | Paso 2: comparar | Paso 3 |
|---|---|---|---|---|
| $T(n)=T(n/2)+1$ | búsqueda binaria | $a=1,b=2$: $n^{0}=1$ | $f=1=\Theta(1\cdot\lg^0n)$ | caso 2: $\Theta(\lg n)$ |
| $T(n)=2T(n/2)+n$ | merge sort | $n^{\lg2}=n$ | $f=n=\Theta(n\lg^0n)$ | caso 2: $\Theta(n\lg n)$ ✓ §8.2 |
| $T(n)=2T(n/2)+1$ | recorrer un árbol balanceado | $n^{1}=n$ | $f=1=n^0=O(n^{1-\varepsilon})$ con $\varepsilon=1$ | caso 1: $\Theta(n)$ |
| $T(n)=3T(n/2)+n$ | multiplicación de Karatsuba | $n^{\lg3}\approx n^{1.585}$ | $f=n=O(n^{1.585-\varepsilon})$ con $\varepsilon=0.5$ | caso 1: $\Theta(n^{\lg 3})$ |

*Qué significa "polinomialmente menor":* que exista un $\varepsilon>0$ fijo con $f(n)=O(n^{\log_b a-\varepsilon})$, o sea, que $f$ quede por debajo de las hojas por un factor $n^\varepsilon$ entero, no solo por un logaritmo. En la tabla, el $\varepsilon$ que sirve está escrito: sirve cualquier $\varepsilon$ con $0<\varepsilon\le$ la diferencia de exponentes.

> ⚠️ **Cuándo NO se aplica:** si el problema se reduce **restando** en lugar de dividir, como en $T(n)=T(n-1)+n$ (quicksort en el peor caso), no tiene la forma $aT(n/b)$. Hay que sumar directamente: $T(n)=n+(n-1)+\dots+1=\Theta(n^2)$ (§7.2).

✏️ **Ejercicio 0.6.** Resuelve $T(n)=4T(n/2)+n^2$.
<details><summary>Solución</summary>

Paso 1: $a=4$, $b=2$, $n^{\log_2 4}=n^2$. Paso 2: $f(n)=n^2=\Theta(n^2\lg^0 n)$. Paso 3: caso 2 con $k=0$, así que $T(n)=\Theta(n^2\lg n)$.
</details>

---

## 9. Contar y probabilidad

**¿Para qué?** Para contar cuántas operaciones hace un algoritmo, cuántos casos tiene que probar una búsqueda exhaustiva, y cuánto cuesta **en promedio** un algoritmo que usa azar (tablas hash en la L04, quicksort en la L05).

### 9.1 Contar con ciclos

| ¿Cuántos…? | Fórmula | El ciclo que los recorre |
|---|---|---|
| pares ordenados $(i,j)$ con $i,j<n$ | $n^2$ | `for i < n: for j < n` |
| pares **sin orden** $\{i,j\}$, $i\neq j$ | $\binom n2=\frac{n(n-1)}2$ | `for i < n: for j < i` |
| subconjuntos de $n$ elementos | $2^n$ | `for mask < 2^n` (bits) |
| ordenamientos (permutaciones) | $n!$ | backtracking (L12) |

**Por qué hay $2^n$ subconjuntos:** cada elemento está o no está, dos opciones independientes por elemento: $2\cdot2\cdots2=2^n$. En código, cada subconjunto es un número de $n$ bits (**máscara**): el bit $i$ dice si el elemento $i$ está. Esta técnica la usan los tests de los labs para comparar contra una búsqueda exhaustiva.

::: {.tabs}
```c
#include <stdio.h>

int main(void) {
    const char *elem[] = {"a", "b", "c"};
    int n = 3;
    for (int mask = 0; mask < (1 << n); mask++) {   // 1 << n es 2^n
        printf("{");
        int primero = 1;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {                  // ¿está el bit i encendido?
                printf("%s%s", primero ? "" : ",", elem[i]);
                primero = 0;
            }
        }
        printf("}\n");
    }
    return 0;
}
```

```cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> elem{"a", "b", "c"};
    int n = static_cast<int>(elem.size());
    for (int mask = 0; mask < (1 << n); ++mask) {   // 1 << n es 2^n
        std::string s = "{";
        for (int i = 0; i < n; ++i)
            if (mask & (1 << i)) {                  // ¿está el bit i encendido?
                if (s.size() > 1) s += ",";
                s += elem[i];
            }
        std::cout << s << "}\n";
    }
}
```

```rust
fn main() {
    let elem = ["a", "b", "c"];
    let n = elem.len();
    for mask in 0u32..(1 << n) {
        // 1 << n es 2^n
        let dentro: Vec<&str> = (0..n)
            .filter(|&i| mask & (1 << i) != 0) // ¿está el bit i encendido?
            .map(|i| elem[i])
            .collect();
        println!("{{{}}}", dentro.join(","));
    }
}
```
:::

```{.text .salida}
{}
{a}
{b}
{a,b}
{c}
{a,c}
{b,c}
{a,b,c}
```

### 9.2 Probabilidad en tres ideas

**Idea 1: espacio, eventos y esperanza.** Un experimento tiene resultados posibles (el **espacio**). Una **variable aleatoria** $X$ asigna un número a cada resultado, y su **esperanza** es el promedio ponderado por la probabilidad:
$$E[X]=\sum_{\text{resultado }r}X(r)\cdot\Pr[r].$$
Un dado justo: $E[X]=\frac{1+2+3+4+5+6}{6}=3.5$.

**Idea 2: linealidad de la esperanza.** $E[X+Y]=E[X]+E[Y]$, **siempre**, aunque $X$ e $Y$ dependan una de otra. Para la suma de dos dados: $E=3.5+3.5=7$, sin revisar los 36 casos. El programa los revisa para convencerte:

::: {.tabs}
```c
#include <stdio.h>

int main(void) {
    double e1 = 0;
    for (int d = 1; d <= 6; d++) e1 += d / 6.0;
    double e2 = 0;
    for (int d1 = 1; d1 <= 6; d1++)
        for (int d2 = 1; d2 <= 6; d2++) e2 += (d1 + d2) / 36.0;   // los 36 casos
    printf("E[un dado] = %.3f\n", e1);
    printf("E[suma de dos dados] = %.3f\n", e2);
    return 0;
}
```

```cpp
#include <cstdio>

int main() {
    double e1 = 0;
    for (int d = 1; d <= 6; ++d) e1 += d / 6.0;
    double e2 = 0;
    for (int d1 = 1; d1 <= 6; ++d1)
        for (int d2 = 1; d2 <= 6; ++d2) e2 += (d1 + d2) / 36.0;   // los 36 casos
    std::printf("E[un dado] = %.3f\n", e1);
    std::printf("E[suma de dos dados] = %.3f\n", e2);
}
```

```rust
fn main() {
    let e1: f64 = (1..=6).map(|d| d as f64 / 6.0).sum();
    let mut e2 = 0.0f64;
    for d1 in 1..=6 {
        for d2 in 1..=6 {
            e2 += (d1 + d2) as f64 / 36.0; // los 36 casos
        }
    }
    println!("E[un dado] = {:.3}", e1);
    println!("E[suma de dos dados] = {:.3}", e2);
}
```
:::

```{.text .salida}
E[un dado] = 3.500
E[suma de dos dados] = 7.000
```

**Idea 3: variables indicadoras.** Para **contar** cuántas veces ocurre algo, se usa una indicadora por cada cosa que podría ocurrir ($I=1$ si ocurre, $0$ si no), y se cumple $E[I]=\Pr[\text{ocurre}]$, porque $E[I]=1\cdot\Pr[\text{ocurre}]+0\cdot\Pr[\text{no ocurre}]$. El total es la suma de las indicadoras, y por linealidad su esperanza es la suma de las probabilidades.

**Ejemplo resuelto (el corazón del hashing, L04).** Se meten $n$ claves al azar en $m$ cubetas. ¿Cuántos **pares de claves chocan** (caen en la misma cubeta), en promedio?
1. Para cada par $\{i,j\}$ defino $I_{ij}=1$ si las claves $i$ y $j$ caen en la misma cubeta.
2. $\Pr[I_{ij}=1]=\frac1m$: cada clave cae en cualquiera de las $m$ cubetas con probabilidad $\frac1m$, independientemente de las demás. Esté donde esté la clave $i$, la clave $j$ cae en esa misma cubeta con probabilidad $\frac1m$.
3. Hay $\binom n2$ pares, así que $E[\text{choques}]=\sum_{\{i,j\}}E[I_{ij}]=\binom n2\cdot\frac1m$.

Con $n=m=100$: $\frac{4950}{100}=49.5$ choques. Una simulación lo confirma. Usa un generador pseudoaleatorio propio (*xorshift*) para que los tres lenguajes produzcan **exactamente** los mismos números:

::: {.tabs}
```c
#include <stdint.h>
#include <stdio.h>

static uint64_t estado = 88172645463325252ULL;
uint64_t xorshift(void) {           // generador pseudoaleatorio de Marsaglia
    estado ^= estado << 13;
    estado ^= estado >> 7;
    estado ^= estado << 17;
    return estado;
}

int main(void) {
    const int n = 100, m = 100, pruebas = 1000;
    long total = 0;
    for (int t = 0; t < pruebas; t++) {
        int cubeta[100] = {0};
        for (int k = 0; k < n; k++) cubeta[xorshift() % m]++;
        for (int b = 0; b < m; b++) total += (long)cubeta[b] * (cubeta[b] - 1) / 2;  // pares en b
    }
    printf("formula:    %.2f\n", (n * (n - 1) / 2.0) / m);
    printf("simulacion: %.2f\n", (double)total / pruebas);
    return 0;
}
```

```cpp
#include <array>
#include <cstdint>
#include <cstdio>

static std::uint64_t estado = 88172645463325252ULL;
std::uint64_t xorshift() {          // generador pseudoaleatorio de Marsaglia
    estado ^= estado << 13;
    estado ^= estado >> 7;
    estado ^= estado << 17;
    return estado;
}

int main() {
    const int n = 100, m = 100, pruebas = 1000;
    long total = 0;
    for (int t = 0; t < pruebas; ++t) {
        std::array<int, 100> cubeta{};
        for (int k = 0; k < n; ++k) ++cubeta[xorshift() % m];
        for (int c : cubeta) total += static_cast<long>(c) * (c - 1) / 2;   // pares en la cubeta
    }
    std::printf("formula:    %.2f\n", (n * (n - 1) / 2.0) / m);
    std::printf("simulacion: %.2f\n", static_cast<double>(total) / pruebas);
}
```

```rust
struct Xorshift(u64);
impl Xorshift {
    fn next(&mut self) -> u64 {
        // generador pseudoaleatorio de Marsaglia
        self.0 ^= self.0 << 13;
        self.0 ^= self.0 >> 7;
        self.0 ^= self.0 << 17;
        self.0
    }
}

fn main() {
    let (n, m, pruebas) = (100u64, 100u64, 1000);
    let mut rng = Xorshift(88172645463325252);
    let mut total: u64 = 0;
    for _ in 0..pruebas {
        let mut cubeta = [0u64; 100];
        for _ in 0..n {
            cubeta[(rng.next() % m) as usize] += 1;
        }
        total += cubeta.iter().map(|&c| c * c.saturating_sub(1) / 2).sum::<u64>(); // pares
    }
    println!("formula:    {:.2}", (n * (n - 1)) as f64 / 2.0 / m as f64);
    println!("simulacion: {:.2}", total as f64 / pruebas as f64);
}
```
:::

```{.text .salida}
formula:    49.50
simulacion: 49.51
```

**Desigualdad de Markov** (la usaremos poco): si $X\ge0$, entonces $\Pr[X\ge t]\le\frac{E[X]}{t}$. Por ejemplo, la probabilidad de que haya **al menos** el doble de choques que el promedio es a lo más $\frac12$: con $t=2E[X]$, la cota es $\frac{E[X]}{2E[X]}=\frac12$.

✏️ **Ejercicio 0.7.** En una permutación al azar de $n$ elementos, ¿cuántos elementos quedan en su lugar original ($\pi(i)=i$), en promedio?
<details><summary>Solución</summary>

Sea $X$ el número de elementos que quedan en su lugar. Lo parto en piezas fáciles: para cada $i$, la **indicadora** $I_i$ vale $1$ si $\pi(i)=i$ y $0$ si no. Entonces $X=I_1+I_2+\dots+I_n$ (cada elemento en su lugar suma 1).

1. **Esperanza de una pieza.** En una permutación al azar, el elemento $i$ puede terminar en cualquiera de los $n$ lugares con la misma probabilidad, así que $\Pr[\pi(i)=i]=\frac1n$. Por la definición de esperanza: $E[I_i]=1\cdot\frac1n+0\cdot\big(1-\frac1n\big)=\frac1n$.
2. **Linealidad.** Las $I_i$ dependen unas de otras (si $n-1$ elementos están en su lugar, el último también), pero la linealidad vale **siempre**: $E[X]=E[I_1]+\dots+E[I_n]=n\cdot\frac1n=1$.

¡Siempre 1, sin importar $n$! *Con números* ($n=3$): de las $6$ permutaciones de $(1,2,3)$, la identidad deja 3 en su lugar, tres permutaciones dejan 1, y dos dejan 0; promedio $\frac{3+1+1+1+0+0}{6}=1$ ✓.
</details>

---

## 10. Vocabulario de grafos (para las lecciones 9 a 11)

![Un grafo no dirigido con un ciclo, un árbol y un DAG](diagramas/grafos.svg)

| Término | Significado |
|---|---|
| Grafo $G=(V,E)$ | vértices $V$ y aristas $E$; $n=\lvert V\rvert$, $m=\lvert E\rvert$ |
| No dirigido / dirigido | la arista $\{u,v\}$ va en ambos sentidos / la arista $(u,v)$ va de $u$ a $v$ |
| Grado de $v$ | número de aristas que tocan a $v$ (en el dibujo, $\text{grado}(2)=3$) |
| Camino / ciclo | sucesión de aristas consecutivas / camino que vuelve al inicio |
| Conexo | hay camino entre cualquier par de vértices |
| Árbol | conexo y sin ciclos; tiene exactamente $n-1$ aristas |
| DAG | dirigido y sin ciclos dirigidos; admite un **orden topológico** |

**Lema del apretón de manos:** en un grafo no dirigido, $\sum_v\text{grado}(v)=2m$.
*Demostración:* cada arista $\{u,v\}$ suma 1 al grado de $u$ y 1 al de $v$: aporta exactamente 2 a la suma. $\square$

::: {.tabs}
```c
#include <stdio.h>

int main(void) {
    int aristas[][2] = {{0, 1}, {0, 2}, {1, 2}, {2, 3}, {3, 4}};   // el grafo del dibujo
    int n = 5, m = 5, grado[5] = {0};
    for (int e = 0; e < m; e++) {
        grado[aristas[e][0]]++;
        grado[aristas[e][1]]++;
    }
    int suma = 0;
    printf("grados:");
    for (int v = 0; v < n; v++) { printf(" %d", grado[v]); suma += grado[v]; }
    printf("\nsuma de grados = %d = 2 x %d aristas\n", suma, m);
    return 0;
}
```

```cpp
#include <iostream>
#include <utility>
#include <vector>

int main() {
    std::vector<std::pair<int, int>> aristas{{0, 1}, {0, 2}, {1, 2}, {2, 3}, {3, 4}};  // el grafo del dibujo
    int n = 5;
    std::vector<int> grado(n, 0);
    for (auto [u, v] : aristas) { ++grado[u]; ++grado[v]; }
    int suma = 0;
    std::cout << "grados:";
    for (int g : grado) { std::cout << " " << g; suma += g; }
    std::cout << "\nsuma de grados = " << suma << " = 2 x " << aristas.size() << " aristas\n";
}
```

```rust
fn main() {
    let aristas = [(0, 1), (0, 2), (1, 2), (2, 3), (3, 4)]; // el grafo del dibujo
    let n = 5;
    let mut grado = vec![0; n];
    for &(u, v) in &aristas {
        grado[u] += 1;
        grado[v] += 1;
    }
    let texto: Vec<String> = grado.iter().map(|g| g.to_string()).collect();
    println!("grados: {}", texto.join(" "));
    println!("suma de grados = {} = 2 x {} aristas", grado.iter().sum::<i32>(), aristas.len());
}
```
:::

```{.text .salida}
grados: 2 2 3 2 1
suma de grados = 10 = 2 x 5 aristas
```

Como $m\le n^2$, se cumple $\lg m\le2\lg n$, así que $O(\lg m)=O(\lg n)$. Lo usaremos para simplificar el costo de Dijkstra.

---

## 11. Cómo se guardan los datos: la memoria

**¿Para qué?** Una estructura de datos es, literalmente, una forma de acomodar datos en la memoria. Para implementar las del curso (y entender por qué un arreglo es más rápido que una lista ligada) hay que saber dónde vive cada cosa.

### 11.1 El mapa de la memoria de un programa

![Mapa de memoria: stack, heap, datos estáticos y código](diagramas/memoria.svg)

| Zona | Qué guarda | Cuánto vive | Tamaño |
|---|---|---|---|
| **Stack** (pila) | variables locales, parámetros, direcciones de retorno | hasta que **termina la función** | pequeño y fijo (~8 MiB): la recursión muy profunda lo **desborda** |
| **Heap** (montículo) | lo que pides en tiempo de ejecución (`malloc`, `new`, `Vec`) | hasta que **lo liberas** (o lo libera su dueño) | tan grande como la RAM |
| Datos estáticos | variables globales, cadenas literales | todo el programa | fijo |

- Un **puntero** (o referencia) es una **dirección** de memoria: un número que dice dónde está un dato.
- Seguir un puntero que ya no es válido (a memoria liberada, nulo o fuera del arreglo) es **comportamiento indefinido (UB)** en C y C++: el programa puede fallar, imprimir basura o, lo peor, **parecer que funciona**.

### 11.2 Un arreglo en el heap, en los tres lenguajes

Mismo programa: pedir un arreglo de $n$ enteros en el heap, llenarlo, sumarlo y liberarlo. Fíjate en **quién libera la memoria** en cada lenguaje:

::: {.tabs}
```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 100;
    int *a = malloc(n * sizeof *a);     // pedir memoria en el heap
    if (a == NULL) return 1;            // malloc puede fallar: hay que revisar
    for (int i = 0; i < n; i++) a[i] = i;
    long suma = 0;
    for (int i = 0; i < n; i++) suma += a[i];
    printf("suma = %ld\n", suma);
    free(a);                            // TÚ debes liberarla, exactamente una vez
    return 0;
}
```

```cpp
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    int n = 100;
    std::vector<int> a(n);              // pide memoria en el heap
    std::iota(a.begin(), a.end(), 0);   // a[i] = i
    long suma = std::accumulate(a.begin(), a.end(), 0L);
    std::cout << "suma = " << suma << "\n";
}   // aquí el destructor de `a` libera la memoria AUTOMÁTICAMENTE (RAII)
```

```rust
fn main() {
    let n = 100;
    let a: Vec<i64> = (0..n).collect(); // pide memoria en el heap, a[i] = i
    let suma: i64 = a.iter().sum();
    println!("suma = {suma}");
} // aquí `a` sale de su ámbito y Rust libera la memoria AUTOMÁTICAMENTE (Drop)
```
:::

```{.text .salida}
suma = 4950
```

### 11.3 El error más famoso: apuntar a memoria que se movió

Cuando un arreglo dinámico se llena, **se muda** a un bloque más grande (lo estudiaremos a fondo en la L01). Cualquier puntero al bloque viejo queda **colgando**. Los tres programas intentan lo mismo: guardar un puntero al primer elemento, agregar muchos elementos y luego leer por el puntero viejo.

::: {.tabs .no-compare}
```{.c .no-ejecutar}
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *v = malloc(2 * sizeof *v);
    if (v == NULL) return 1;
    v[0] = 1;
    int *p = &v[0];                      // puntero al primer elemento
    int *nuevo = realloc(v, 1000 * sizeof *v);  // el bloque puede MUDARSE
    if (nuevo == NULL) { free(v); return 1; }
    v = nuevo;
    printf("%d\n", *p);                  // UB: p apunta al bloque viejo, ya liberado
    free(v);
    return 0;
}
```

```{.cpp .no-ejecutar}
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2};
    int& r = v[0];                       // referencia al primer elemento
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);                  // al crecer, el vector se MUDA
    std::cout << r << "\n";              // UB: r apunta al bloque viejo, ya liberado
}
```

```{.rust .no-compila}
fn main() {
    let mut v = vec![1, 2];
    let r = &v[0]; // préstamo de v
    for i in 0..1000 {
        v.push(i); // ERROR de compilación: no puedes modificar v mientras r lo tiene prestado
    }
    println!("{r}");
}
```
:::

- **C y C++:** compilan (GCC incluso puede advertirte en C). Al ejecutarse, **cualquier cosa** puede pasar. AddressSanitizer, que activamos en los tests del curso, lo detecta como *heap-use-after-free*.
- **Rust:** **no compila**. El compilador ve que `r` es un préstamo de `v` que sigue vivo hasta el `println!`, y que `push` necesita modificar `v`. Lo que en C/C++ es un error en tiempo de ejecución, en Rust es un error de **compilación**:

```text
error[E0502]: cannot borrow `v` as mutable because it is also borrowed as immutable
```

Esta diferencia la volveremos a ver en **cada** lab.

---

## 12. Los tres lenguajes, idea por idea

Esta sección es para que, vengas de C, C++ o Rust, reconozcas la misma idea en los otros dos. Cada ejemplo resuelve **el mismo problema** en los tres.

### 12.1 Modificar una variable del que llama: punteros, referencias y préstamos

::: {.tabs}
```c
#include <stdio.h>

// C solo tiene punteros: pasas la DIRECCIÓN y desreferencias con *
void intercambiar(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int main(void) {
    int a = 1, b = 2;
    printf("antes:   a = %d, b = %d\n", a, b);
    intercambiar(&a, &b);               // &a = "la dirección de a"
    printf("despues: a = %d, b = %d\n", a, b);
    return 0;
}
```

```cpp
#include <iostream>

// C++ añade referencias: un alias que nunca es nulo y no se reasigna
void intercambiar(int& x, int& y) {
    int t = x;
    x = y;
    y = t;
}

int main() {
    int a = 1, b = 2;
    std::cout << "antes:   a = " << a << ", b = " << b << "\n";
    intercambiar(a, b);                 // la sintaxis de la llamada no lo delata
    std::cout << "despues: a = " << a << ", b = " << b << "\n";
}
```

```rust
// Rust: un préstamo mutable &mut, explícito en la firma Y en la llamada
fn intercambiar(x: &mut i32, y: &mut i32) {
    let t = *x;
    *x = *y;
    *y = t;
}

fn main() {
    let (mut a, mut b) = (1, 2);
    println!("antes:   a = {a}, b = {b}");
    intercambiar(&mut a, &mut b); // el &mut en la llamada avisa que a y b cambiarán
    println!("despues: a = {a}, b = {b}");
}
```
:::

```{.text .salida}
antes:   a = 1, b = 2
despues: a = 2, b = 1
```

### 12.2 Liberar recursos: manual, RAII y `Drop`

En C, **tú** llamas a la función de limpieza. En C++ el **destructor** se llama solo al salir del ámbito (*RAII*: *Resource Acquisition Is Initialization*). En Rust lo hace el trait **`Drop`**, con la misma idea.

::: {.tabs}
```c
#include <stdio.h>

typedef struct { const char *nombre; } Recurso;

Recurso abrir(const char *nombre) { printf("abro %s\n", nombre); return (Recurso){nombre}; }
void cerrar(Recurso *r) { printf("cierro %s\n", r->nombre); }

int main(void) {
    Recurso r = abrir("archivo");
    printf("uso %s\n", r.nombre);
    cerrar(&r);              // si olvidas esta línea (o sales antes con return): fuga
    return 0;
}
```

```cpp
#include <iostream>
#include <string>

class Recurso {
public:
    explicit Recurso(std::string n) : nombre_(std::move(n)) { std::cout << "abro " << nombre_ << "\n"; }
    ~Recurso() { std::cout << "cierro " << nombre_ << "\n"; }   // destructor
    const std::string& nombre() const { return nombre_; }
private:
    std::string nombre_;
};

int main() {
    Recurso r("archivo");
    std::cout << "uso " << r.nombre() << "\n";
}   // aquí se llama ~Recurso() automáticamente, aunque salgas con return o excepción
```

```rust
struct Recurso {
    nombre: String,
}

impl Recurso {
    fn abrir(nombre: &str) -> Recurso {
        println!("abro {nombre}");
        Recurso { nombre: nombre.to_string() }
    }
}

impl Drop for Recurso {
    fn drop(&mut self) {
        println!("cierro {}", self.nombre);
    }
}

fn main() {
    let r = Recurso::abrir("archivo");
    println!("uso {}", r.nombre);
} // aquí se llama drop() automáticamente
```
:::

```{.text .salida}
abro archivo
uso archivo
cierro archivo
```

### 12.3 Un dueño a la vez: mover en lugar de copiar

Si dos variables creen que son dueñas de la misma memoria, ambas intentarán liberarla (**doble liberación**, UB). La solución moderna es que **el dueño sea uno solo** y que la propiedad se pueda **mover**.

::: {.tabs}
```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p1 = malloc(sizeof *p1);
    if (p1 == NULL) return 1;
    *p1 = 42;
    int *p2 = p1;     // ahora DOS punteros al mismo bloque: ¿quién lo libera?
    p1 = NULL;        // convención manual: "p1 ya no es dueño"
    printf("p2 apunta a %d\n", *p2);
    printf("p1 esta vacio: %s\n", p1 == NULL ? "si" : "no");
    free(p2);         // solo el dueño libera
    return 0;
}
```

```cpp
#include <iostream>
#include <memory>

int main() {
    auto p1 = std::make_unique<int>(42);    // unique_ptr: dueño único
    auto p2 = std::move(p1);                // la propiedad PASA a p2; p1 queda vacío
    // auto p3 = p2;                        // no compila: unique_ptr no se copia
    std::cout << "p2 apunta a " << *p2 << "\n";
    std::cout << "p1 esta vacio: " << (p1 == nullptr ? "si" : "no") << "\n";
}   // p2 libera la memoria; p1 no hace nada
```

```rust
fn main() {
    let p1: Option<Box<i32>> = Some(Box::new(42)); // Box: dueño único en el heap
    let mut p1 = p1;
    let p2 = p1.take(); // la propiedad pasa a p2; p1 queda en None
    // Con un Box directo bastaría `let p2 = p1;`, y usar p1 después sería ERROR de compilación.
    println!("p2 apunta a {}", p2.as_ref().unwrap());
    println!("p1 esta vacio: {}", if p1.is_none() { "si" } else { "no" });
} // p2 libera la memoria
```
:::

```{.text .salida}
p2 apunta a 42
p1 esta vacio: si
```

### 12.4 Código genérico: una función para muchos tipos

::: {.tabs}
```c
#include <stdio.h>

// C no tiene genéricos de verdad. Opciones: una función por tipo,
// macros, o _Generic (C11), que elige la función según el tipo.
int    max_int(int a, int b)          { return a > b ? a : b; }
double max_double(double a, double b) { return a > b ? a : b; }
#define maximo(a, b) _Generic((a), int: max_int, double: max_double)(a, b)

int main(void) {
    printf("max(3, 7) = %d\n", maximo(3, 7));
    printf("max(2.5, 1.5) = %g\n", maximo(2.5, 1.5));
    return 0;
}
```

```cpp
#include <concepts>
#include <iostream>

// Plantilla: el compilador genera una versión por cada tipo T usado.
// El concept std::totally_ordered (C++20) exige que T se pueda comparar.
template <std::totally_ordered T>
T maximo(T a, T b) { return a > b ? a : b; }

int main() {
    std::cout << "max(3, 7) = " << maximo(3, 7) << "\n";
    std::cout << "max(2.5, 1.5) = " << maximo(2.5, 1.5) << "\n";
}
```

```rust
// Genérico con un "trait bound": T debe poder compararse (PartialOrd).
// Como en C++, el compilador genera una versión por tipo (monomorfización).
fn maximo<T: PartialOrd>(a: T, b: T) -> T {
    if a > b { a } else { b }
}

fn main() {
    println!("max(3, 7) = {}", maximo(3, 7));
    println!("max(2.5, 1.5) = {}", maximo(2.5, 1.5));
}
```
:::

```{.text .salida}
max(3, 7) = 7
max(2.5, 1.5) = 2.5
```

### 12.5 Cuando no hay respuesta: códigos de error, `std::optional` y `Option`

Esta es exactamente la decisión de diseño del axioma 5 de la Pila en la L01 (¿qué devuelve `pop` en una pila vacía?):

::: {.tabs}
```c
#include <stdbool.h>
#include <stdio.h>

// C: devuelve si hubo éxito y escribe el resultado por un puntero
bool dividir(int a, int b, int *resultado) {
    if (b == 0) return false;
    *resultado = a / b;
    return true;
}

int main(void) {
    int r;
    if (dividir(10, 2, &r)) printf("10 / 2 = %d\n", r); else printf("10 / 2 = sin resultado\n");
    if (dividir(10, 0, &r)) printf("10 / 0 = %d\n", r); else printf("10 / 0 = sin resultado\n");
    return 0;   // nada obliga al que llama a revisar el bool…
}
```

```cpp
#include <iostream>
#include <optional>

// C++17: std::optional<int> es "un int, o nada"
std::optional<int> dividir(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

void mostrar(int a, int b) {
    auto r = dividir(a, b);
    std::cout << a << " / " << b << " = ";
    if (r) std::cout << *r << "\n";          // *r sin revisar sería UB si está vacío
    else   std::cout << "sin resultado\n";
}

int main() {
    mostrar(10, 2);
    mostrar(10, 0);
}
```

```rust
// Rust: Option<i32> es Some(valor) o None, y el compilador OBLIGA a tratar ambos
fn dividir(a: i32, b: i32) -> Option<i32> {
    if b == 0 {
        None
    } else {
        Some(a / b)
    }
}

fn mostrar(a: i32, b: i32) {
    match dividir(a, b) {
        Some(r) => println!("{a} / {b} = {r}"),
        None => println!("{a} / {b} = sin resultado"),
    }
}

fn main() {
    mostrar(10, 2);
    mostrar(10, 0);
}
```
:::

```{.text .salida}
10 / 2 = 5
10 / 0 = sin resultado
```

### 12.6 Las reglas de Rust que más sorprenden al venir de C/C++

| Regla | Qué significa | Qué error de C/C++ evita |
|---|---|---|
| **Un solo dueño** | cada valor tiene exactamente un dueño; al salir de su ámbito, se libera | fugas y dobles liberaciones |
| **Mover por defecto** | `let b = a;` mueve: `a` ya no se puede usar. Para copiar hay que pedirlo con `.clone()` (salvo tipos simples como `i32`, que son `Copy`) | copias caras accidentales |
| **Préstamos: muchos lectores O un escritor** | puedes tener muchas `&T` **o** una sola `&mut T`, nunca ambas a la vez | punteros colgantes (§11.3), *data races* |
| **Los préstamos no sobreviven al dueño** | el compilador verifica los *lifetimes* | referencias a variables locales ya destruidas |
| **Sin `null`** | la ausencia se escribe `Option<T>` | desreferenciar nulos |
| **`unsafe`** | desactiva algunas verificaciones; **tú** garantizas las reglas (lo usaremos en L02 y L03) | — es la puerta de escape, a propósito |

---

## 13. La pregunta que nos acompañará todo el curso

> **¿Quién garantiza que la estructura de datos está en un estado válido?**

| | C | C++ | Rust |
|---|---|---|---|
| Memoria | el programador, a mano | el programador, con ayuda (RAII, *smart pointers*, sanitizers) | el compilador (*ownership* + *borrow checker*), salvo en `unsafe` |
| Errores (pila vacía, etc.) | códigos de retorno | excepciones, `std::optional` o UB documentado | `Option` / `Result` en el tipo, o `panic!` |
| Genéricos | macros, `void*`, `_Generic` | plantillas (+ `concepts` en C++20) | genéricos con *trait bounds* |
| Copias | implícitas (copiar bytes) | implícitas (constructor de copia) | explícitas (`.clone()`) |
| Punteros con ciclos (listas dobles, grafos) | naturales | naturales | difíciles: `Rc/Weak` o `unsafe` (L03) |

En los labs implementarás cada estructura en **los tres lenguajes**. En C todo es explícito: tú reservas y liberas la memoria, y los genéricos se escriben con `void*` + tamaño del elemento + comparador, como `qsort`. En C++ y Rust verás cómo cada lenguaje automatiza (o verifica) esas mismas responsabilidades.

---

## 14. Autoevaluación

Si puedes responder esto sin mirar las notas, estás listo para la L01.

1. Niega: "todo alumno aprobó al menos un examen".
2. Demuestra por inducción que $1+3+5+\dots+(2n-1)=n^2$.
3. ¿Cuántas veces se ejecuta el cuerpo de `for (i = 1; i < n; i *= 2)` para $n=1000$? ¿Y en general?
4. Resuelve $T(n)=2T(n/2)+n^2$ con el Teorema Maestro.
5. ¿Qué imprime `printf("%d", -9 / 4)` en C? ¿Y $\lfloor-9/4\rfloor$?
6. Un programa en C++ guarda `int& r = v[0];` y luego hace `v.push_back(x)`. ¿Por qué es peligroso? ¿Qué haría Rust con el código equivalente?
7. ¿Cuál es la esperanza del número de caras al lanzar 10 monedas? Usa linealidad.

<details><summary>Respuestas</summary>

1. "Existe un alumno que no aprobó ningún examen" ($\neg\forall\exists=\exists\forall\neg$).
2. Lo que queremos demostrar: $1+3+5+\dots+(2n-1)=n^2$ para todo $n\ge1$.

   **Paso 1. Pruebo con números chicos.**
   $$
   \begin{aligned}
   n=1:&\quad 1 &&\text{y}\quad 1^2=1\ ✓\\
   n=2:&\quad 1+3=4 &&\text{y}\quad 2^2=4\ ✓\\
   n=3:&\quad 1+3+5=9 &&\text{y}\quad 3^2=9\ ✓
   \end{aligned}
   $$

   **Paso 2. La base.** Con $n=1$ la suma tiene un solo término, $2\cdot1-1=1$, y $1^2=1$ ✓.

   **Paso 3. Supongo que vale para un $n=k\ge1$ cualquiera.**
   $$\boxed{\,1+3+\dots+(2k-1)=k^2\,}\qquad\text{(HI)}$$

   **Paso 4. Escribo lo que quiero.** Cambio $n$ por $(k+1)$. El último término es $2(k+1)-1=2k+1$:
   $$1+3+\dots+(2k-1)+(2k+1)\ \overset{?}{=}\ (k+1)^2$$

   **Paso 5. Busco la caja de la HI dentro del lado izquierdo.** Todo menos el último término es **exactamente** el lado izquierdo de la caja:
   $$\boxed{\,1+3+\dots+(2k-1)\,}+(2k+1)$$

   **Paso 6. Uso la HI.** Cambio la caja por $k^2$:
   $$k^2+(2k+1)$$

   **Paso 7. Quito el paréntesis.**
   $$k^2+2k+1$$

   **Paso 8. Reconozco un binomio al cuadrado.** $(k+1)^2=(k+1)(k+1)=k^2+k+k+1=k^2+2k+1$:
   $$(k+1)^2$$
   Es el lado derecho del paso 4 ✓.

   **Paso 9. Compruebo con $k=3$.** El lado izquierdo del paso 4 es $1+3+5+7=16$; el paso 6 da $9+7=16$; el paso 8, $4^2=16$ ✓.

   **Conclusión.** El paso 2 tiró el primer dominó y los pasos 3 a 8 mostraron que cada uno tumba al siguiente. Por inducción, vale para todo $n\ge1$. $\square$
3. $i=1,2,4,\dots,512$: **10** veces. En general, $\lceil\lg n\rceil$ veces para $n\ge1$.
4. $n^{\log_2 2}=n$; $f=n^2=\Omega(n^{1+\varepsilon})$ y $2\cdot(n/2)^2=\frac12n^2\le c\,n^2$ con $c=\frac12$: caso 3, $\Theta(n^2)$. (Aquí $\varepsilon=1$: $n^2=n^{1+1}$.)
5. `-2` (trunca hacia cero). $\lfloor-2.25\rfloor=-3$.
6. `push_back` puede mudar el arreglo y dejar `r` colgando (UB). En Rust **no compila**: el préstamo `r` impide modificar `v` mientras siga vivo.
7. Indicadora $I_k$ de "la moneda $k$ cae cara", con $E[I_k]=\frac12$. Por linealidad, $E=10\cdot\frac12=5$.
</details>

---

## 15. Tarea en papel: inducción (puntos extra)

**Para qué:** la inducción se aprende escribiéndola a mano, paso por paso. Esta tarea es **opcional** y da puntos extra; tu profesor te dirá cómo se suman a la calificación. Vale hasta **10 puntos**.

**Reglas de entrega.**

- A mano, en papel. Nada de código: aquí el objetivo es la demostración.
- Usa la plantilla de §3.1 en **cada** inducción, con sus cinco partes rotuladas: afirmación $P(n)$, base, HI (escrita con $k$), paso y conclusión.
- En el paso, escribe **primero** $P(k+1)$ completa, sustituyendo cada $n$ por $(k+1)$, con paréntesis.
- Escribe cada demostración en **pasos numerados, un paso por párrafo**, igual que §3.2. Cada paso empieza diciendo qué haces, da la razón (qué regla, qué sustituiste, por qué no se voltea el signo) y hace una sola transformación.
- Escribe la HI **en una caja**, y encierra en otra caja el pedazo donde la usas.
- Cierra cada ejercicio con una **comprobación con números** para un valor pequeño de $k$. No demuestra nada, pero atrapa errores de álgebra.

**Ejercicios.**

1. *(1 punto)* Demuestra que $1^2+2^2+\dots+n^2=\dfrac{n(n+1)(2n+1)}{6}$ para todo $n\ge1$.
2. *(1 punto)* Demuestra que $4^n-1$ es múltiplo de $3$ para todo $n\ge0$. *Pista:* "múltiplo de 3" significa "$=3\cdot(\text{entero})$"; escribe la HI así y busca esa forma en $4^{k+1}-1$.
3. *(1 punto)* Demuestra que $n!\ge2^n$ para todo $n\ge4$. Luego comprueba que la desigualdad es **falsa** para $n=1,2,3$, y explica en una frase por qué eso obliga a que la base sea $n=4$.
4. *(1 punto)* Demuestra que $\displaystyle\sum_{i=1}^{n}i\cdot2^i=(n-1)\,2^{n+1}+2$ para todo $n\ge1$.
5. *(2 puntos, inducción fuerte)* Demuestra que cualquier cantidad de $n\ge12$ pesos se puede pagar usando solo monedas de $4$ y de $5$ pesos. *Pista:* para pagar $k+1$, paga $k+1-4$ y agrega una moneda de 4. ¿Cuántos casos base necesitas para que $k+1-4$ siempre caiga dentro de la HI?
6. *(1 punto, inducción y recursión)* La función $\text{pot}(x,n)$ se define así: si $n=0$ devuelve $1$; si no, devuelve $x\cdot\text{pot}(x,n-1)$. Demuestra, por inducción sobre $n$, que $\text{pot}(x,n)=x^n$ para todo $n\ge0$. Señala qué línea de la definición corresponde a la base y cuál a la HI, como en §3.5.
7. *(2 puntos, encuentra el error)* La siguiente "demostración" concluye algo falso. Di **exactamente** en qué renglón falla y para qué valor de $k$.

   > **Afirmación:** todo entero $n\ge0$ es igual a $0$.
   >
   > **Base:** $n=0$ ✓.
   >
   > **HI fuerte:** $j=0$ para todo $j$ con $0\le j\le k$.
   >
   > **Paso:** escribo $k+1=a+b$ con $a=k$ y $b=1$. Los dos cumplen $0\le a,b\le k$, así que por la HI fuerte $a=0$ y $b=0$. Entonces $k+1=a+b=0+0=0$. $\square$

8. *(1 punto, invariante)* El ciclo `p = 1; for (i = 0; i < n; i++) p = 2 * p;` termina con $p=2^n$. Escribe el invariante al inicio de la vuelta con índice $i$ y demuestra la inicialización, el mantenimiento (sustituyendo $i$ por $i+1$, como en §4.2) y la terminación.

---

## Bibliografía de repaso

- Lehman, Leighton y Meyer. *Mathematics for Computer Science* (MIT 6.042), caps. 1–5 (demostraciones e inducción), 14 (sumas y asintótica), 17–19 (probabilidad). Gratuito en línea.
- Cormen, Leiserson, Rivest y Stein. *Introduction to Algorithms*, 4ª ed.: Apéndices A (sumatorias), B (conjuntos y grafos), C (conteo y probabilidad); cap. 4 (recurrencias).
- Kernighan y Ritchie. *The C Programming Language*, 2ª ed. — Stroustrup. *A Tour of C++*, 3ª ed. — Klabnik y Nichols. *The Rust Programming Language*, caps. 4 (ownership), 10 (genéricos y traits) y 15 (smart pointers).
