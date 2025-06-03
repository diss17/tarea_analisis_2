import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

edmemo_data = pd.read_csv("results_EDMemo.csv")
eddp_data = pd.read_csv("results_EDDp.csv")
eddpo_data = pd.read_csv("results_EDDpO.csv")

edmemo_data["peak_memory_kb"] = pd.to_numeric(edmemo_data["peak_memory_kb"], errors="coerce")
eddp_data["peak_memory_kb"] = pd.to_numeric(eddp_data["peak_memory_kb"], errors="coerce")
eddpo_data["peak_memory_kb"] = pd.to_numeric(eddpo_data["peak_memory_kb"], errors="coerce")

edmemo_data = edmemo_data.dropna(subset=["peak_memory_kb"])
eddp_data = eddp_data.dropna(subset=["peak_memory_kb"])
eddpo_data = eddpo_data.dropna(subset=["peak_memory_kb"])

combinations = [f"Comb {i+1}" for i in range(len(edmemo_data))]

edmemo_memory = edmemo_data["peak_memory_kb"]
eddp_memory = eddp_data["peak_memory_kb"]
eddpo_memory = eddpo_data["peak_memory_kb"]

x = np.arange(len(combinations)) 
width = 0.25  

plt.figure(figsize=(12, 6))  
plt.bar(x - width, edmemo_memory, width, label="Memoización", color="#1f77b4")
plt.bar(x, eddp_memory, width, label="Programación dinámica", color="#ff7f0e")  
plt.bar(x + width, eddpo_memory, width, label="Programación dinámica optimizada", color="#2ca02c")  

plt.xlabel("Combinaciones de Textos", fontsize=12, fontweight="bold")
plt.ylabel("Peak de Memoria (KB)", fontsize=12, fontweight="bold")
plt.title("Uso de Memoria por Algoritmo", fontsize=14, fontweight="bold")
plt.xticks(x, combinations, rotation=45, fontsize=10)
plt.yticks(fontsize=10)
plt.legend(fontsize=10)

plt.grid(axis="y", linestyle="--", alpha=0.7)

plt.gca().get_yaxis().set_major_formatter(plt.FuncFormatter(lambda x, _: f"{int(x):,}"))

plt.tight_layout()

plt.show()