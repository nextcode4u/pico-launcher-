#!/usr/bin/env bash
set -euo pipefail

# Run a verbose build and capture the output to a timestamped log for debugging.
ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT_DIR"

mkdir -p build/logs
LOG_FILE="build/logs/build-$(date +%Y%m%d-%H%M%S).log"
echo "Writing verbose build log to $LOG_FILE"

make VERBOSE=1 2>&1 | tee "$LOG_FILE"
