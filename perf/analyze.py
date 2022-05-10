import re
import sys
import os

def parse_report(path="perf_c2c_report.txt"):
    if not os.path.exists(path):
        print(f"Report file {path} not found.")
        return

    hitm_count = 0
    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            if "HITM" in line:
                hitm_count += 1

    print(f"Identified {hitm_count} cacheline conflict records in trace.")

if __name__ == "__main__":
    parse_report()
