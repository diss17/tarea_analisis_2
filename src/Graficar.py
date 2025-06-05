import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

edmemo_data = pd.read_csv("results_EDMemo.csv")
eddp_data = pd.read_csv("results_EDDp.csv")
eddpo_data = pd.read_csv("results_EDDpO.csv")

edmemo_data["mean_time"] = pd.to_numeric(edmemo_data["mean_time"], errors="coerce")
eddp_data["mean_time"] = pd.to_numeric(eddp_data["mean_time"], errors="coerce")
eddpo_data["mean_time"] = pd.to_numeric(eddpo_data["mean_time"], errors="coerce")

edmemo_data = edmemo_data.dropna(subset=["mean_time"])
eddp_data = eddp_data.dropna(subset=["mean_time"])
eddpo_data = eddpo_data.dropna(subset=["mean_time"])

combinations = [f"Comb {i+1}" for i in range(len(edmemo_data))]

edmemo_times = edmemo_data["mean_time"] / 1e6 
eddp_times = eddp_data["mean_time"] / 1e6   
eddpo_times = eddpo_data["mean_time"] / 1e6  

x = np.arange(len(combinations))  
width = 0.25 

plt.figure(figsize=(12, 6)) 
plt.bar(x - width, edmemo_times, width, label="Memoización", color="#1f77b4")
plt.bar(x, eddp_times, width, label="Programación dinámica", color="#ff7f0e") 
plt.bar(x + width, eddpo_times, width, label="Programación dinámica optimizada", color="#2ca02c") 

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