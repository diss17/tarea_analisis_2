import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

edmemo_data = pd.read_csv("results_EDMemo_time.csv")
eddp_data = pd.read_csv("results_EDDp_time.csv")
eddpo_data = pd.read_csv("results_EDDpO_time.csv")
edrec_data = pd.read_csv("results_EDRecursivo_time.csv")  #

# Convertir tiempos a numérico (nanosegundos)
edmemo_data["mean_time"] = pd.to_numeric(edmemo_data["mean_time"], errors="coerce")
eddp_data["mean_time"] = pd.to_numeric(eddp_data["mean_time"], errors="coerce")
eddpo_data["mean_time"] = pd.to_numeric(eddpo_data["mean_time"], errors="coerce")
edrec_data["mean_time"] = pd.to_numeric(edrec_data["mean_time"], errors="coerce")  # 

# Eliminar filas con valores inválidos
edmemo_data = edmemo_data.dropna(subset=["mean_time"])
eddp_data = eddp_data.dropna(subset=["mean_time"])
eddpo_data = eddpo_data.dropna(subset=["mean_time"])
edrec_data = edrec_data.dropna(subset=["mean_time"])  # 

combinations = [f"Comb {i+1}" for i in range(len(edmemo_data))]

# Convertir de nanosegundos a milisegundos
edmemo_times = edmemo_data["mean_time"] / 1e6
eddp_times = eddp_data["mean_time"] / 1e6
eddpo_times = eddpo_data["mean_time"] / 1e6
edrec_times = edrec_data["mean_time"] / 1e6  # 

x = np.arange(len(combinations))
width = 0.2  # Reducido para acomodar 4 barras

plt.figure(figsize=(12, 6))
plt.bar(x - 1.5 * width, edmemo_times, width, label="Memoización", color="#1f77b4")
plt.bar(x - 0.5 * width, eddp_times, width, label="Programación dinámica", color="#ff7f0e")
plt.bar(x + 0.5 * width, eddpo_times, width, label="Programación dinámica optimizada", color="#2ca02c")
plt.bar(x + 1.5 * width, edrec_times, width, label="Recursivo", color="#d62728")  # 

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
