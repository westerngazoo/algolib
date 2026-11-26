# algolib — atajos para correr los labs en C, C++ y Rust.
#
#   make lab01         # el lab 01 en los tres lenguajes
#   make c-lab01       # solo C
#   make cpp-lab01     # solo C++
#   make rust-lab01    # solo Rust
#   make all-labs      # todos los labs, los tres lenguajes
#   make clean

LABS := $(patsubst cpp/tests/%,%,$(wildcard cpp/tests/lab*))
BUILD := cpp/build
CBUILD := c/build

# Nota: las reglas lab%/cpp-lab%/rust-lab% son patrones; no se declaran .PHONY
# porque make omite la búsqueda de reglas implícitas para objetivos .PHONY.
.PHONY: all-labs clean configure cconfigure html diagramas ejemplos actualizar

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

html: diagramas
	python3 notas/herramientas/build_html.py

clean:
	rm -rf $(BUILD) $(CBUILD) rust/target
