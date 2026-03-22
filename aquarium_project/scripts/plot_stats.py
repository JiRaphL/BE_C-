#!/usr/bin/env python3
"""Plot simulation statistics from CSV and save PNG charts."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path
from typing import Dict, List

import matplotlib.pyplot as plt


def read_rows(csv_path: Path) -> Dict[str, List[float]]:
    with csv_path.open("r", encoding="utf-8", newline="") as f:
        reader = csv.DictReader(f)
        rows = list(reader)

    if not rows:
        raise ValueError(f"No rows found in {csv_path}")

    fields = [
        "tick",
        "population",
        "average_age",
        "collisions",
        "deaths_collision",
        "deaths_age",
        "births_clone",
        "births_spontaneous",
    ]
    data: Dict[str, List[float]] = {k: [] for k in fields}

    for row in rows:
        for k in fields:
            data[k].append(float(row[k]))

    return data


def save_population_chart(data: Dict[str, List[float]], outdir: Path) -> None:
    plt.figure(figsize=(11, 5))
    plt.plot(data["tick"], data["population"], linewidth=1.5)
    plt.title("Population over time")
    plt.xlabel("Tick")
    plt.ylabel("Population")
    plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig(outdir / "population.png", dpi=150)
    plt.close()


def save_events_chart(data: Dict[str, List[float]], outdir: Path) -> None:
    plt.figure(figsize=(11, 5))
    plt.plot(data["tick"], data["collisions"], label="Collisions", linewidth=1.1)
    plt.plot(data["tick"], data["deaths_collision"], label="Deaths by collision", linewidth=1.1)
    plt.plot(data["tick"], data["deaths_age"], label="Deaths by age", linewidth=1.1)
    plt.plot(data["tick"], data["births_clone"], label="Births by clone", linewidth=1.1)
    plt.plot(data["tick"], data["births_spontaneous"], label="Births spontaneous", linewidth=1.1)
    plt.title("Events per tick")
    plt.xlabel("Tick")
    plt.ylabel("Count")
    plt.legend()
    plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig(outdir / "events_per_tick.png", dpi=150)
    plt.close()


def save_age_chart(data: Dict[str, List[float]], outdir: Path) -> None:
    plt.figure(figsize=(11, 5))
    plt.plot(data["tick"], data["average_age"], color="tab:orange", linewidth=1.5)
    plt.title("Average age over time")
    plt.xlabel("Tick")
    plt.ylabel("Average age")
    plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.savefig(outdir / "average_age.png", dpi=150)
    plt.close()


def print_summary(data: Dict[str, List[float]]) -> None:
    ticks = len(data["tick"])
    coll = sum(data["collisions"])
    dcol = sum(data["deaths_collision"])
    dage = sum(data["deaths_age"])
    bclone = sum(data["births_clone"])
    bsp = sum(data["births_spontaneous"])

    print(f"Ticks: {ticks}")
    print(f"Population start/end: {int(data['population'][0])} -> {int(data['population'][-1])}")
    print(f"Population min/max: {int(min(data['population']))} / {int(max(data['population']))}")
    print(f"Total collisions: {int(coll)}")
    print(f"Deaths by collision: {int(dcol)}")
    print(f"Deaths by age: {int(dage)}")
    print(f"Births by clone: {int(bclone)}")
    print(f"Births spontaneous: {int(bsp)}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Plot simulation statistics from CSV.")
    parser.add_argument(
        "--input",
        default="simulation_stats.csv",
        help="Path to input CSV file (default: simulation_stats.csv)",
    )
    parser.add_argument(
        "--outdir",
        default="plots",
        help="Output directory for PNG files (default: plots)",
    )
    args = parser.parse_args()

    in_path = Path(args.input)
    outdir = Path(args.outdir)
    outdir.mkdir(parents=True, exist_ok=True)

    data = read_rows(in_path)
    save_population_chart(data, outdir)
    save_events_chart(data, outdir)
    save_age_chart(data, outdir)
    print_summary(data)
    print(f"Plots written to: {outdir.resolve()}")


if __name__ == "__main__":
    main()
