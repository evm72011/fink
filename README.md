# fink

`fink` is a **learning C++ project** focused on building a clean and extensible
infrastructure for numerical pricing of financial instruments.

The project is intentionally **not production-ready**.  
Its purpose is to explore architecture, numerical methods, and tooling used in
quantitative finance and high-performance computing.

---

## Current status

At the moment, the following functionality is implemented:

### ✔ Financial logic
- Analytic **Black–Scholes** formula for a European call option
- A **naive Monte Carlo** implementation for European option pricing
- Geometric Brownian Motion (GBM) under the risk-neutral measure
- Basic payoff models (call / put)

The Monte Carlo implementation is deliberately kept **simple and monolithic**.
It serves as a **reference implementation** for validating future refactoring
and more advanced architectures.

---

### ✔ Math & utilities
- Standard normal PDF and CDF
- Basic statistics (mean, variance, standard deviation)
- Random number generation (PCG32 + normal distribution)

---

### ✔ Architecture & tooling
- Modular structure (`libs/math`, `libs/rng`, `libs/models`,
  `libs/instruments`, `libs/pricers`)
- CMake-based build with:
  - GoogleTest
  - strict warning profiles
  - sanitizers
  - clang-tidy
- Tests:
  - unit tests for math and Black–Scholes
  - integration test verifying `Monte Carlo ≈ Black–Scholes`

---

## Not implemented yet (planned)

The project is under active development. Planned next steps include:

- Extracting a reusable **Monte Carlo engine**
- Variance reduction techniques:
  - antithetic variates
  - control variates
- Clear separation of concerns:
  - `pricers` — financial logic
  - `mc` — Monte Carlo algorithm
  - `backends` — execution (CPU / GPU)
- CPU backend (parallelism, SIMD)
- CUDA backend
- Benchmarking (Google Benchmark)
- Additional instruments (Asian options, barriers, Greeks)

---

## Project goals

The goal of `fink` is to **learn and experiment with**:

- clean architecture for numerical finance libraries
- separation of mathematical models from execution backends
- safe and testable refactoring of Monte Carlo algorithms
- foundations for CPU/GPU accelerated pricing

This is a **research and learning project**, not a production trading system.

---

## Build

The project is built using CMake and GoogleTest.

WSL:

```bash
make venv
make all
