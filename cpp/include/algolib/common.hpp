// algolib — utilidades comunes.
#pragma once

#include <stdexcept>
#include <string>

namespace algolib {

// Excepción que lanzan las funciones aún no implementadas por el alumno.
// El runner de tests la reporta como [TODO] en lugar de [FAIL].
struct NotImplemented : std::logic_error {
    explicit NotImplemented(const std::string& where)
        : std::logic_error("no implementado: " + where) {}
};

}  // namespace algolib

// Úsalo como cuerpo de cualquier función pendiente.
// Equivalente en Rust: todo!()
#define ALGOLIB_TODO() throw ::algolib::NotImplemented(__func__)
