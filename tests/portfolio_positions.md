# Portfolio position synchronization regression

Run from the repository root:

```sh
python3 tests/test_portfolio_positions.py
python3 tests/test_portfolio_positions.py --tsan
```

Requires Python 3 and a C++17 compiler (`CXX`, default `clang++`). The second
command also requires compiler/runtime support for ThreadSanitizer.

The test extracts the production `get_or_create_position` and
`update_fund_dynprofit` functions and compiles them with the repository's actual
`wt_hashmap`. Broker, logger, clock and fund surroundings are isolated test doubles.
It checks empty/normal aggregation, null entries preserving the previous result,
existing-position identity, and four writers inserting 16,000 positions while a
reader aggregates. It does not exercise the entire engine, persistence, CTA/SEL
callbacks, or broker integration.

The portfolio mutex protects table operations and affected position-field access.
It is recursive to support strategy update -> position update/save nesting.
Lock order is portfolio mutex before the existing individual position mutex.
CTA/SEL release it before calling trading executers. Local persistence and trade
logging remain inside protected operations: slow disk I/O can delay other
portfolio updates; no throughput or real-market capacity guarantee is implied.
A null entry logs an error and aborts the affected update/save, retaining the last
published aggregate or saved file. Investigate the error; it is not repaired by
silently treating the position as zero.

Rebuild WtCore and dependent binaries together using the matching Windows x64
Release toolchain before deployment: WtEngine's layout changed. Full MSVC build,
offline engine replay and restart/persistence verification remain required.
No live front connection or deployment is part of this regression test.
