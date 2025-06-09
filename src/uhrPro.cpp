#include <cstdint>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <psapi.h>
#include <atomic>
#include <thread>

#include "utils.cpp"
#include "EDMemo.cpp"
#include "EDDp.cpp"
#include "EDDpO.cpp"
#include "EDRecursivo.cpp"

// Funciones auxiliares
std::int64_t get_memory_usage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}

std::string read_text_from_file(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << file_path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line, text;
    bool first_line = true;
    while (std::getline(file, line)) {
        if (first_line) {
            first_line = false;
            continue;
        }
        text += line + "\n";
    }
    file.close();
    return text;
}

std::string truncate_text(const std::string &text, std::size_t max_length) {
    if (max_length == 0 || max_length >= text.size())
        return text;
    else
        return text.substr(0, max_length);
}

void validate_input(int argc, char *argv[], std::int64_t &runs, std::size_t &max_chars, std::string &mode, std::string &algorithm) {
    if (argc < 5) {
        std::cerr << "Uso: " << argv[0] << " <runs> <max_chars> <mode> <algorithm>\n";
        std::cerr << "Ejemplo: " << argv[0] << " 10 1000 time EDMemo\n";
        std::cerr << "mode puede ser: time, memory\n";
        std::cerr << "algorithm puede ser: EDMemo, EDDp, EDDpO, EDRecursivo\n";
        std::exit(EXIT_FAILURE);
    }

    try {
        runs = std::stoll(argv[1]);
        max_chars = static_cast<std::size_t>(std::stoul(argv[2]));
        mode = argv[3];
        algorithm = argv[4];
        if (mode != "time" && mode != "memory")
            throw std::invalid_argument("Modo inválido");
        if (algorithm != "EDMemo" && algorithm != "EDDp" && algorithm != "EDDpO" && algorithm != "EDRecursivo")
            throw std::invalid_argument("Algoritmo inválido");
    } catch (const std::exception &e) {
        std::cerr << "Error en los argumentos: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

template <typename Func>
void run_memory_experiment(const std::string &algorithm_name, Func func, const std::string &str1, const std::string &str2, std::int64_t runs, const std::string &output_file) {
    std::int64_t peak_memory_overall = 0;

    for (std::int64_t i = 0; i < runs; i++) {
        std::atomic<bool> running(true);
        std::int64_t peak_memory = 0;

        std::thread monitor([&]() {
            while (running) {
                std::int64_t current_mem = get_memory_usage();
                peak_memory = std::max(peak_memory, current_mem);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });

        func(str1, str2);
        running = false;
        monitor.join();

        peak_memory_overall = std::max(peak_memory_overall, peak_memory);
    }

    std::ofstream mem_data(output_file, std::ios::app);
    if (mem_data.tellp() == 0) {
        mem_data << "n,peak_memory_kb,text1_size,text2_size" << std::endl;
    }
    mem_data << runs << "," << peak_memory_overall << "," << str1.size() << "," << str2.size() << std::endl;

    std::cerr << algorithm_name << " - Peak memory observed: " << peak_memory_overall << " KB" << std::endl;
}

template <typename Func>
void run_time_experiment(const std::string &algorithm_name, Func func, const std::string &str1, const std::string &str2, std::int64_t runs, const std::string &output_file) {
    std::vector<double> times(runs);
    double mean_time = 0, time_stdev = 0;

    for (std::int64_t i = 0; i < runs; i++) {
        auto begin_time = std::chrono::high_resolution_clock::now();
        func(str1, str2);
        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::nano> elapsed_time = end_time - begin_time;
        times[i] = elapsed_time.count();
        mean_time += times[i];
    }

    mean_time /= runs;

    for (std::int64_t i = 0; i < runs; i++) {
        double dev = times[i] - mean_time;
        time_stdev += dev * dev;
    }

    time_stdev = std::sqrt(time_stdev / (runs - 1));

    std::ofstream time_data(output_file, std::ios::app);
    if (time_data.tellp() == 0) {
        time_data << "n,mean_time,time_stdev,text1_size,text2_size" << std::endl;
    }
    time_data << runs << "," << mean_time << "," << time_stdev << "," << str1.size() << "," << str2.size() << std::endl;

    std::cerr << algorithm_name << " - Mean time: " << mean_time << " ns, StdDev: " << time_stdev << " ns" << std::endl;
}
