#! /bin/bash
set -e

BIN="./build/program"

EXPR_FILE="${1:-expression.txt}"

BUILD_DIR="build"
TEX_DIR="latex"
SCR_DIR="scripts"
OUT_TEX="$TEX_DIR/out"
OUT_SCR="$SCR_DIR/out"

if [ "$1" == "clean" ]; then
    echo "[CLEAN] Removing outputs"
    rm -rf "$OUT_TEX" "$OUT_SCR" "$SCR_DIR" "$TEX_DIR"
    exit 0
fi

mkdir -p "$SCR_DIR" "$TEX_DIR" "$OUT_SCR" "$OUT_TEX"

TS=$(date +"%Y%m%d_%H%M%S")

GV_FILE="$SCR_DIR/graph.gv"
TEX_FILE="$TEX_DIR/differenciator.tex"
PY_FILE="$SCR_DIR/graph.py"

[ -x "$BIN" ] || make

"$BIN" "$EXPR_FILE" "$GV_FILE" "$TEX_FILE" "$PY_FILE"

echo "[INFO] Running Graphviz"
dot -Tpng "$GV_FILE" -o "$OUT_SCR/graph_$TS.png"

echo "[INFO] Running Python"
python3 "$PY_FILE" "$OUT_SCR/plot_$TS.png"

echo "[INFO] Running LaTeX"
pdflatex -interaction=nonstopmode -output-directory="$OUT_TEX" "$TEX_FILE" >/dev/null

mv "$OUT_TEX/differenciator.pdf" "$OUT_TEX/differenciator_$TS.pdf"

echo "[OK] Done"

