# CP Notebook: what changed

New file: `CP_Notebook_Final.tex` (also zipped as `CP_Notebook_Final.zip` with `main.tex` inside, ready for Overleaf).
The original zips are untouched.

## Status

- 98 snippets, 3796 code lines, every line at most 57 characters.
- Every C++ snippet was compiled with `g++ -std=c++17 -Wall -Wextra` under your template (`#define int long long`) and checked against a brute-force solution on thousands of random tests. All pass.
- Page count is **calculated, not compiled** (there is no LaTeX on this laptop): 1 index page + about 23.5 code pages = 25 pages. Please compile once on Overleaf and check the last page number.

## Layout

- Font size is unchanged (`\footnotesize` typewriter).
- `basewidth=4.25pt`: listings was stretching every character to 5.1pt, so only 46 characters fit per line and long lines wrapped. Now the letters use their natural width and 57 characters fit.
- Column gap 22pt -> 18pt, line-number margin 12pt -> 9pt.
- No code line is longer than the column, so nothing wraps or overlaps.
- Straight quotes `'` in code (`upquote`), so `'a'` prints as you type it.

## Bugs fixed

| Snippet | Problem in the old version |
|---|---|
| Segment tree | `push()` in `query_sum` was swallowed by a comment, so lazy values were never pushed in queries (HLD was wrong too) |
| Kruskal | stray half-deleted lines, did not compile |
| Dijkstra Algorithm | compared `dist[v] > w` instead of `d + w` |
| FFT Struct | `power()` called `multiply_long` which did not exist; NTT existed only as comments (now a real NTT) |
| Counting pair sum | returned nothing and indexed past the end of the result |
| BinaryTrie | `count_xor_less_than` always returned 0 (root index 0 was treated as "no node") |
| Mo's algorithm | `remove(--cur_r)` removed the wrong element |
| Kadane | second half was loose code outside any function |
| Convex Hull Trick | comment said minimum; the structure answers maximum |
| Tangents from a point | used `asin` where the angle is `acos` (wrong touching points) |
| Common tangents | outer tangents wrong when the first circle is smaller |
| Segment intersection | wrong for a zero-length segment |
| Bit tricks | `1 << x` overflows for x >= 31 (now `1LL << x`); `countSetBits_upto_n` shifted by -1 |
| Suffix array | `max(k - 1, 0)` does not compile with `#define int long long` |
| Lucas, stars and bars | called an `nCr(n, r, p)` that did not exist; negative arguments |
| Dynamic segment tree | had a `main()` inside and four 6M-element static arrays |
| Fraction, prefix sum 2D, rerooting | missing `;`, loose code, skeleton only |
| Miller-Rabin | bases extended to 37 so it is exact for all 64-bit numbers |
| Li Chao tree | mixed half-open and closed ranges, rewritten with a closed range |

## Merged duplicates

- Fraction x2, Euler totient x3, Suffix array x3, Dynamic segment tree x2, DSU x2, Dijkstra x2, Prim x2, Fenwick x2, Prefix sum 2D x2, Convex hull x3: one each.
- Articulation, bridge, bridge tree, "Articulation and bridge": one struct `CutBridge`.
- Prime sieve, small prime factor, sieve number of divisors: one linear sieve.
- Number of divisors, sum of divisors, divisors from prime factors: one snippet, all take the same `{prime, power}` list.
- binpow / powmod / modexp: one `power(a, b, m)`.
- nCr x3 + nPr: factorial tables plus Pascal.
- Matrix expo + matrix multiplication: one snippet. Miller-Rabin + Pollard rho: one snippet.
- Angle sort, ccw, polar sort around a ray: "Integer geometry (exact)".
- "Geometry 1" split into Point basics, Lines, Polygons, Circles.
- HLD, 2-SAT, Kruskal, virtual tree, inversion count now say "Needs SegTree / SCC / DSU / LCA / Fenwick" instead of repeating the code.

## Removed

- Vector2D transform (same result as the spiral).
- nCr with doubles, `strict_positive` option of BSGS, `hashWithout`, `sectorArea_signed`, double-based `polarCmp`.
- Lazy version of the iterative segment tree (replaced by a simple point-update one; use the recursive tree for lazy).
- Merge-sort inversion count (replaced by the Fenwick version).
- `get_kth_max_xor`, `get_min_xor`: use `kthXor(x, k)`.

## Added (new, all tested)

The merged notebook was about 20 pages, so the spare pages hold algorithms that ICPC/IUPC sets use often:

- General: binary and ternary search, custom hash, coordinate compression, sliding window minimum, largest rectangle in histogram, stress-test script and a wrong-answer checklist.
- Number theory: determinant modulo prime, formula sheet.
- DP: edit distance, knapsack patterns and bitset subset sum, digit DP, bitmask DP (TSP), SOS DP, divide and conquer optimization.
- Graph: grid BFS and 0-1 BFS, tree diameter and Euler tour, small-to-large, centroid decomposition, 2-SAT, Hopcroft-Karp matching, min cost max flow, Euler path.
- Range queries: Fenwick with range add and range sum, k-th smallest with the persistent tree.
- Geometry: closest pair of points, minimum enclosing circle.
- Strings: per-pattern occurrence counts in Aho-Corasick, O(1) LCP queries in the suffix array.

## Naming

- All snippets assume the Template: `int` is 64-bit, `all(x)`, `MOD`, `inf`, `rng`.
- `N` = size constant, `adj` = adjacency list, `add_edge(u, v)`, `{v, w}` pairs for weighted edges, `power`, `nCr`, `multiply`, `SegTree`, `DSU`, `LCA`, `SCC`, `Fenwick` are used with the same names everywhere.
- Comments marked `CHANGE` show the lines to edit for a different problem; `Usage:` shows how to initialise and call.
