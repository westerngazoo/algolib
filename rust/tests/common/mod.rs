//! Utilidades compartidas por los tests de integración.
//!
//! Los tests de cada lab usan un runner propio (`harness = false` en Cargo.toml)
//! para reportar igual que en C++:  [ OK ] / [FAIL] / [TODO].
//! Un `todo!()` en el código del alumno se reporta como [TODO], no como fallo.
#![allow(dead_code)]

use std::cell::RefCell;
use std::panic::{self, AssertUnwindSafe};

/// Generador congruencial lineal determinista (idéntico al `algotest::Lcg` de C++).
pub struct Lcg(u64);

impl Lcg {
    pub fn new(seed: u64) -> Self { Lcg(seed) }

    pub fn next(&mut self) -> u64 {
        self.0 = self
            .0
            .wrapping_mul(6364136223846793005)
            .wrapping_add(1442695040888963407);
        self.0 >> 33
    }

    /// Uniforme en [0, n).
    pub fn below(&mut self, n: u64) -> u64 { self.next() % n }

    /// Uniforme en [lo, hi].
    pub fn range(&mut self, lo: i64, hi: i64) -> i64 {
        lo + self.below((hi - lo + 1) as u64) as i64
    }
}

thread_local! {
    static LAST_LOCATION: RefCell<String> = const { RefCell::new(String::new()) };
}

/// Ejecuta los casos y devuelve código de salida distinto de 0 si algo falla o está pendiente.
pub fn run(suite: &str, cases: &[(&str, fn())]) {
    panic::set_hook(Box::new(|info| {
        let loc = info
            .location()
            .map(|l| format!("{}:{}", l.file(), l.line()))
            .unwrap_or_default();
        LAST_LOCATION.with(|c| *c.borrow_mut() = loc);
    }));

    println!("== {suite} (Rust) ==");
    let (mut ok, mut failed, mut todo) = (0, 0, 0);
    for (name, f) in cases {
        match panic::catch_unwind(AssertUnwindSafe(f)) {
            Ok(()) => {
                ok += 1;
                println!("  [ OK ] {name}");
            }
            Err(payload) => {
                let msg = payload
                    .downcast_ref::<&str>()
                    .map(|s| s.to_string())
                    .or_else(|| payload.downcast_ref::<String>().cloned())
                    .unwrap_or_else(|| "<panic>".into());
                let loc = LAST_LOCATION.with(|c| c.borrow().clone());
                if msg.starts_with("not yet implemented") {
                    todo += 1;
                    println!("  [TODO] {name}  ({loc})");
                } else {
                    failed += 1;
                    println!("  [FAIL] {name}\n         {loc}  {msg}");
                }
            }
        }
    }
    println!("-- {ok} ok, {failed} fallidos, {todo} pendientes");
    if failed + todo > 0 {
        std::process::exit(1);
    }
}

/// `cases![a, b, c]` → `&[("a", a), ("b", b), ("c", c)]`
#[macro_export]
macro_rules! cases {
    ($($f:ident),* $(,)?) => { &[$((stringify!($f), $f as fn())),*] };
}
