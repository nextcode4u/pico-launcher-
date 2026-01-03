#!/usr/bin/env bash
set -euo pipefail
MODE="${1:-build}"
OUT="${2:-bundle.txt}"
ts() { date -u +"%Y-%m-%dT%H:%M:%SZ"; }
say() { printf "\n===== %s =====\n" "$1"; }
repo_root() { git rev-parse --show-toplevel 2>/dev/null || pwd; }
ROOT="$(repo_root)"
cd "$ROOT"
: > "$OUT"
{
  say "TIME"
  ts
  say "PWD"
  pwd
  say "SYSTEM"
  uname -a || true
  lsb_release -a 2>/dev/null || true
  say "SHELL"
  echo "${SHELL:-}"
  echo "PATH=$PATH"
  say "TOOLCHAIN"
  for t in make gcc g++ clang clang++ cmake ninja python3 node git; do
    command -v "$t" >/dev/null 2>&1 && printf "%s=%s\n" "$t" "$(command -v "$t")"
  done
  for t in arm-none-eabi-gcc arm-none-eabi-g++ arm-none-eabi-ld ndstool wf-pacman wf-config; do
    command -v "$t" >/dev/null 2>&1 && printf "%s=%s\n" "$t" "$(command -v "$t")"
  done
  say "VERSIONS"
  git --version 2>/dev/null || true
  make --version 2>/dev/null | head -n 2 || true
  gcc --version 2>/dev/null | head -n 2 || true
  g++ --version 2>/dev/null | head -n 2 || true
  cmake --version 2>/dev/null | head -n 2 || true
  ninja --version 2>/dev/null || true
  python3 --version 2>/dev/null || true
  arm-none-eabi-gcc --version 2>/dev/null | head -n 2 || true
  arm-none-eabi-g++ --version 2>/dev/null | head -n 2 || true
  ndstool --help 2>/dev/null | head -n 3 || true
  say "GIT STATUS"
  git status -sb 2>/dev/null || true
  say "GIT REMOTES"
  git remote -v 2>/dev/null || true
  say "GIT LAST 10 COMMITS"
  git log --oneline -n 10 2>/dev/null || true
  say "GIT DIFF"
  git diff 2>/dev/null || true
  say "GIT DIFF (STAGED)"
  git diff --staged 2>/dev/null || true
  say "GIT SUBMODULES"
  git submodule status --recursive 2>/dev/null || true
  say "TREE (TOP)"
  ls -la 2>/dev/null || true
  if [[ "$MODE" == "build" ]]; then
    say "BUILD"
    rm -f build.log
    if command -v nproc >/dev/null 2>&1; then
      make -j"$(nproc)" V= 2>&1 | tee build.log || true
    else
      make V= 2>&1 | tee build.log || true
    fi
    say "BUILD.LOG (LAST 400 LINES)"
    tail -n 400 build.log 2>/dev/null || true
  else
    say "BUILD (SKIPPED)"
  fi
} | tee "$OUT"
printf "\nSaved: %s\n" "$OUT"
