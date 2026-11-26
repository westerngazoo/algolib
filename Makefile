# algolib — atajos para correr los labs en C, C++ y Rust.
#
#   make lab01         # el lab 01 en los tres lenguajes
#   make c-lab01       # solo C
#   make cpp-lab01     # solo C++
#   make rust-lab01    # solo Rust
#   make c-fugas-lab01 # busca fugas de memoria en tu C (en macOS usa `leaks`)
#   make all-labs      # todos los labs, los tres lenguajes
#   make extraer-L00   # copia los programas de las notas de L00 a ejemplos/L00/
#   make clean

LABS := $(patsubst cpp/tests/%,%,$(wildcard cpp/tests/lab*))
BUILD := cpp/build
CBUILD := c/build

# Nota: las reglas lab%/cpp-lab%/rust-lab% son patrones; no se declaran .PHONY
# porque make omite la búsqueda de reglas implícitas para objetivos .PHONY.
.PHONY: all-labs clean configure cconfigure html diagramas ejemplos extraer actualizar

configure:
	@cmake -S cpp -B $(BUILD) > /dev/null

cconfigure:
	@cmake -S c -B $(CBUILD) > /dev/null

c-lab%: cconfigure
	@cmake --build $(CBUILD) --target lab$*_tests -- --no-print-directory 2>&1 | grep -Ev '^\[|^Built target|^gmake' || true
	@./$(CBUILD)/lab$*_tests || true

cpp-lab%: configure
	@cmake --build $(BUILD) --target lab$*_tests -- --no-print-directory 2>&1 | grep -Ev '^\[|^Built target|^gmake' || true
	@./$(BUILD)/lab$*_tests || true

# Fugas de memoria en C. En Linux, LeakSanitizer ya corre dentro de c-labNN.
# En macOS LeakSanitizer no existe: compilamos SIN sanitizers (leaks no convive con
# ASan) y corremos los tests bajo `leaks --atExit`, que reporta cada bloque sin liberar.
c-fugas-lab%:
ifeq ($(shell uname -s),Darwin)
	@cmake -S c -B c/build-fugas -DALGOLIB_SANITIZE=OFF > /dev/null
	@cmake --build c/build-fugas --target lab$*_tests -- --no-print-directory 2>&1 | grep -Ev '^\[|^Built target|^gmake' || true
	@MallocStackLogging=lite leaks --quiet --atExit -- ./c/build-fugas/lab$*_tests 2>&1 \
	  | grep -Ev 'MallocStackLogging|^Process [0-9]+ is not debuggable|^$$' ; true
else
	@$(MAKE) --no-print-directory c-lab$*
endif

rust-lab%:
	@cd rust && cargo test -q --test lab$* 2>&1 | grep -Ev '^\s*$$|Finished|Running|Compiling|error: test failed|Caused by|process didn.t exit' || true

lab%:
	@$(MAKE) --no-print-directory c-lab$*
	@echo
	@$(MAKE) --no-print-directory cpp-lab$*
	@echo
	@$(MAKE) --no-print-directory rust-lab$*

all-labs:
	@for l in $(LABS); do $(MAKE) --no-print-directory $$l; echo; done

diagramas:
	python3 notas/herramientas/gen_diagramas.py

# Actualiza el curso desde un zip nuevo SIN tocar tus labs (ver herramientas/actualizar.sh)
actualizar:
	@test -n "$(ZIP)" || (echo "uso: make actualizar ZIP=ruta/al/algolib.zip"; exit 1)
	@bash herramientas/actualizar.sh "$(ZIP)" "$(CURDIR)"

ejemplos:
	python3 notas/herramientas/check_ejemplos.py

# Copia los programas de las notas a ejemplos/LNN/ para correrlos y modificarlos.
# No sobrescribe carpetas existentes; FORZAR=1 las regenera.
extraer-L%:
	@cd notas/herramientas && python3 extraer_ejemplos.py L$* $(if $(FORZAR),--forzar)

extraer:
	@cd notas/herramientas && python3 extraer_ejemplos.py $(if $(FORZAR),--forzar)

html: diagramas
	python3 notas/herramientas/build_html.py

clean:
	rm -rf $(BUILD) $(CBUILD) c/build-fugas rust/target
