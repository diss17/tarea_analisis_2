# Análisis de Algoritmos de Distancia de Edición

Este proyecto compara diferentes implementaciones del algoritmo de distancia de edición (edit distance) usando solo operaciones de inserción y eliminación, aplicados a textos literarios clásicos.

## Estructura del Proyecto

- **src/**  
  Contiene las implementaciones en C++:
  - `EDDp.cpp`: Programación dinámica clásica.
  - `EDDpO.cpp`: Programación dinámica optimizada en espacio.
  - `EDMemo.cpp`: Solución con memoización (top-down).
  - `EDRecursivo.cpp`: Solución recursiva pura.
  - `uhrPro.cpp`: Script principal para ejecutar experimentos y medir tiempo/memoria.
  - `utils.cpp`: Utilidades para validación de argumentos.
  - `Graficar.py`, `GraficoMemoria.py`: Scripts en Python para graficar resultados.

- **Text/**  
  Textos de entrada utilizados para las pruebas (`Extract1.txt`, `Extract2.txt`, etc.).

- **Tiempo/**  
  Resultados de tiempo de ejecución y gráficos (`results_EDDp.csv`, etc.).

- **Memoria/**  
  Resultados de uso de memoria y gráficos (`results_EDDp.csv`, etc.).

## Ejecución

### Compilar

Desde la carpeta `src/`:

```sh
g++ uhrPro.cpp -o experimento.exe -std=c++17 -lpsapi
```

### Ejecutar

```sh
./experimento.exe <runs> <max_chars> <mode> <algorithm>
```

- `<runs>`: Número de repeticiones por experimento (ej: 64)
- `<max_chars>`: Máximo de caracteres a considerar de cada texto
- `<mode>`: `time` o `memory`
- `<algorithm>`: `EDMemo`, `EDDp`, `EDDpO`, `EDRecursivo`

**Ejemplo:**

```sh
./experimento.exe 64 1000 time EDDp
```

### Graficar resultados

Desde la carpeta raíz:

```sh
python src/Graficar.py
python src/GraficoMemoria.py
```

## Resultados

Los resultados de tiempo y memoria se almacenan en las carpetas `Tiempo/` y `Memoria/` respectivamente, en formato CSV. Los scripts de Python generan gráficos comparativos.

## Créditos

- Textos de dominio público obtenidos de [Project Gutenberg](https://www.gutenberg.org/).
- Implementaciones y experimentos desarrollados por [Tu Nombre].

---