CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SRC_DIR = src
SRCS = $(SRC_DIR)/main.cpp
TARGET = tarea_analisis_2.exe

all: $(TARGET)

$(TARGET): $(SRCS)
    $(CXX) $(CXXFLAGS) -o $@ $^

clean:
    del /Q $(TARGET)

# Dependencias de Python para graficar
python-deps:
    pip install pandas matplotlib numpy

.PHONY: all clean python-deps