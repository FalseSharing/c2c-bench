#!/usr/bin/env bash
set -euo pipefail

echo "[*] Recording cache-to-cache (c2c) profile..."
perf c2c record -F 60000 -- ../cpp/bench
echo "[*] Generating text report..."
perf c2c report --stdio > perf_c2c_report.txt
echo "[+] Done. Saved to perf_c2c_report.txt"
