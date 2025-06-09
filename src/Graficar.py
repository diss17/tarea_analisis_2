import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

incluir_recursivo = len(sys.argv) > 1 and sys.argv[1] == "1"

edmemo_data = pd.read_csv("results_EDMemo_time.csv")
eddp_data = pd.read_csv("results_EDDp_time.csv")
eddpo_data = pd.read_csv("results_EDDpO_time.csv")
if incluir_recursivo:
    edrec_data = pd.read_csv("results_EDRecursivo_time.csv")

# Convertir tiempos a numérico (nanosegundos)
edmemo_data["mean_time"] = pd.to_numeric(edmemo_data["mean_time"], errors="coerce")
eddp_data["mean_time"] = pd.to_numeric(eddp_data["mean_time"], errors="coerce")
eddpo_data["mean_time"] = pd.to_numeric(eddpo_data["mean_time"], errors="coerce")
if incluir_recursivo:
    edrec_data["mean_time"] = pd.to_numeric(edrec_data["mean_time"], errors="coerce")

# Eliminar filas con valores inválidos
edmemo_data = edmemo_data.dropna(subset=["mean_time"])
eddp_data = eddp_data.dropna(subset=["mean_time"])
eddpo_data = eddpo_data.dropna(subset=["mean_time"])
if incluir_recursivo:
    edrec_data = edrec_data.dropna(subset=["mean_time"])

combinations = [f"Comb {i+1}" for i in range(len(edmemo_data))]

edmemo_times = edmemo_data["mean_time"] 
eddp_times = eddp_data["mean_time"]
eddpo_times = eddpo_data["mean_time"] 
if incluir_recursivo:
    edrec_times = edrec_data["mean_time"] 

x = np.arange(len(combinations))
width = 0.2 if incluir_recursivo else 0.25

plt.figure(figsize=(12, 6))
plt.bar(x - width, edmemo_times, width, label="Memoización", color="#1f77b4")
plt.bar(x, eddp_times, width, label="Programación dinámica", color="#ff7f0e")
plt.bar(x + width, eddpo_times, width, label="Programación dinámica optimizada", color="#2ca02c")
if incluir_recursivo:
    plt.bar(x + 2*width, edrec_times, width, label="Recursivo", color="#d62728")

plt.xlabel("Combinaciones de Textos", fontsize=12, fontweight="bold")
plt.ylabel("Tiempo Promedio (ms)", fontsize=12, fontweight="bold")
plt.title("Comparación de Algoritmos Edit distance", fontsize=14, fontweight="bold")
plt.xticks(x, combinations, rotation=45, fontsize=10)
plt.yticks(fontsize=10)
plt.legend(fontsize=10)
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.gca().get_yaxis().set_major_formatter(plt.FuncFormatter(lambda x, _: f"{int(x):,}"))
plt.tight_layout()
plt.show()
