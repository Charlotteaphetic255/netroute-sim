CC     = gcc
EMCC   = emcc
CFLAGS = -Wall -Wextra -g

SRC = src/graph.c \
      src/unionFind.c \
      src/minHeap.c \
      src/kruskal.c \
      src/prim.c \
      src/dijkstra.c \
      src/simulator.c \
      src/loader.c \
      src/main.c

# ── Native CLI build ──────────────────────────────────────────────────────────
# Usage: make
# Output: ./netroute-sim

all: netroute-sim

netroute-sim: $(SRC)
	$(CC) $(CFLAGS) -o netroute-sim $(SRC)

# ── WebAssembly build ─────────────────────────────────────────────────────────
# Usage: make wasm
# Output: web/netroute.js + web/netroute.wasm

wasm: $(SRC)
	$(EMCC) -O2 \
	  -s WASM=1 \
	  -s MODULARIZE=1 \
	  -s EXPORT_NAME="NetRouteSim" \
	  -s ALLOW_MEMORY_GROWTH=1 \
	  -s EXPORTED_FUNCTIONS='["_wasm_load_graph","_wasm_run_kruskal","_wasm_run_prim","_wasm_run_dijkstra","_wasm_get_mst_cost","_wasm_free_result","_malloc","_free"]' \
	  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","HEAP32","getValue","setValue"]' \
	  -o web/netroute.js \
	  $(SRC)
	@echo "WASM build done → web/netroute.js + web/netroute.wasm"

# ── Run tests ─────────────────────────────────────────────────────────────────
# Usage: make test

test:
	@echo "--- Testing Union-Find ---"
	$(CC) $(CFLAGS) -o _test_uf tests/testUnionFind.c src/unionFind.c && ./_test_uf

	@echo "--- Testing Min-Heap ---"
	$(CC) $(CFLAGS) -o _test_heap tests/testMinHeap.c src/minHeap.c && ./_test_heap

	@echo "--- Testing Graph ---"
	$(CC) $(CFLAGS) -o _test_graph tests/testGraph.c src/graph.c && ./_test_graph

	@echo "--- Testing Kruskal ---"
	$(CC) $(CFLAGS) -o _test_kruskal tests/testKruskal.c src/graph.c src/unionFind.c src/kruskal.c && ./_test_kruskal

	@echo "--- Testing Prim ---"
	$(CC) $(CFLAGS) -o _test_prim tests/testPrim.c src/graph.c src/minHeap.c src/prim.c && ./_test_prim

	@echo "--- Testing Dijkstra ---"
	$(CC) $(CFLAGS) -o _test_dijkstra tests/testDijkstra.c src/graph.c src/minHeap.c src/dijkstra.c && ./_test_dijkstra

	@echo "--- Testing Simulator ---"
	$(CC) $(CFLAGS) -o _testSimulator tests/testSimulator.c src/graph.c src/minHeap.c src/dijkstra.c src/loader.c src/simulator.c && ./_testSimulator

	@echo "All tests passed."

# ── Local web server (for testing WASM in browser) ───────────────────────────
# Usage: make serve

serve:
	@echo "Serving at http://localhost:8080"
	cd web && python3 -m http.server 8080

# ── Clean up all build artifacts ─────────────────────────────────────────────
# Usage: make clean

clean:
	rm -f netroute-sim
	rm -f _test_uf _test_heap _test_graph _test_kruskal _test_prim _test_dijkstra _testSimulator
	rm -f web/netroute.js web/netroute.wasm

.PHONY: all wasm test serve clean