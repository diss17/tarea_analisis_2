#include <cstdint>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // para std::min
#include <windows.h>
#include <psapi.h>
#include <atomic>
#include <thread>

#include "utils.cpp"
#include "EDMemo.cpp"
#include "EDDp.cpp"
#include "EDDpO.cpp"
#include "EDRecursivo.cpp"

std::int64_t get_memory_usage()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        return pmc.WorkingSetSize; // memoria en KB en uso actual
    }
    return 0;
}

// Leer texto desde archivo
std::string read_text_from_file(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo " << file_path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line, text;
    bool first_line = true;
    while (std::getline(file, line))
    {
        if (first_line)
        {
            first_line = false;
            continue;
        }
        text += line + "\n";
    }
    file.close();
    return text;
}

// Función para truncar el texto a max_length caracteres
std::string truncate_text(const std::string &text, std::size_t max_length)
{
    if (max_length == 0 || max_length >= text.size())
        return text;
    else
        return text.substr(0, max_length);
}

// Validar inputs: runs, max_chars, mode, algorithm
void validate_input(int argc, char *argv[], std::int64_t &runs, std::size_t &max_chars, std::string &mode, std::string &algorithm)
{
    if (argc < 5)
    {
        std::cerr << "Uso: " << argv[0] << " <runs> <max_chars> <mode> <algorithm>\n";
        std::cerr << "Ejemplo: " << argv[0] << " 10 1000 time EDMemo\n";
        std::cerr << "mode puede ser: time, memory\n";
        std::cerr << "algorithm puede ser: EDMemo, EDDp, EDDpO, EDRecursivo\n";
        std::exit(EXIT_FAILURE);
    }

    try
    {
        runs = std::stoll(argv[1]);
        max_chars = static_cast<std::size_t>(std::stoul(argv[2]));
        mode = argv[3];
        algorithm = argv[4];
        if (mode != "time" && mode != "memory")
            throw std::invalid_argument("Modo inválido");
        if (algorithm != "EDMemo" && algorithm != "EDDp" && algorithm != "EDDpO" && algorithm != "EDRecursivo")
            throw std::invalid_argument("Algoritmo inválido");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error en los argumentos: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// Función para medir pico de memoria mientras la función se ejecuta
template <typename Func>
void run_memory_experiment(const std::string &algorithm_name, Func func, const std::string &str1, const std::string &str2, std::int64_t runs, const std::string &output_file)
{
    std::int64_t peak_memory_overall = 0;

    for (std::int64_t i = 0; i < runs; i++)
    {
        std::atomic<bool> running(true);
        std::int64_t peak_memory = 0;

        // Hilo que monitorea la memoria periódicamente
        std::thread monitor([&]() {
            while (running)
            {
                std::int64_t current_mem = get_memory_usage();
                peak_memory = std::max(peak_memory, current_mem);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });

        func(str1, str2);  // Ejecuta la función a medir

        running = false;    // Señal para terminar el hilo monitor
        monitor.join();     // Espera a que el hilo termine

        peak_memory_overall = std::max(peak_memory_overall, peak_memory);
    }

    std::ofstream mem_data(output_file, std::ios::app);
    if (mem_data.tellp() == 0)
    {
        mem_data << "n,peak_memory_kb,text1_size,text2_size" << std::endl;
    }
    mem_data << runs << "," << peak_memory_overall << "," << str1.size() << "," << str2.size() << std::endl;

    std::cerr << algorithm_name << " - Peak memory observed: " << peak_memory_overall << " KB" << std::endl;

    mem_data.close();
}

// Función para medir solo tiempo
template <typename Func>
void run_time_experiment(const std::string &algorithm_name, Func func, const std::string &str1, const std::string &str2, std::int64_t runs, const std::string &output_file)
{
    std::vector<double> times(runs);
    double mean_time = 0, time_stdev = 0, dev;

    for (std::int64_t i = 0; i < runs; i++)
    {
        auto begin_time = std::chrono::high_resolution_clock::now();
        func(str1, str2);
        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::nano> elapsed_time = end_time - begin_time;
        times[i] = elapsed_time.count();
        mean_time += times[i];
    }

    mean_time /= runs;

    for (std::int64_t i = 0; i < runs; i++)
    {
        dev = times[i] - mean_time;
        time_stdev += dev * dev;
    }

    time_stdev /= runs - 1;
    time_stdev = std::sqrt(time_stdev);

    std::ofstream time_data(output_file, std::ios::app);
    if (time_data.tellp() == 0)
    {
        time_data << "n,mean_time,time_stdev,text1_size,text2_size" << std::endl;
    }
    time_data << runs << "," << mean_time << "," << time_stdev << "," << str1.size() << "," << str2.size() << std::endl;

    std::cerr << algorithm_name << " - Mean time: " << mean_time << " ns, StdDev: " << time_stdev << " ns" << std::endl;

    time_data.close();
}

int main(int argc, char *argv[])
{
    std::int64_t runs;
    std::size_t max_chars;
    std::string mode;
    std::string algorithm;

    validate_input(argc, argv, runs, max_chars, mode, algorithm);

    std::string text1 = truncate_text(read_text_from_file("Extract1.txt"), max_chars);
    std::string text2 = truncate_text(read_text_from_file("Extract2.txt"), max_chars);
    std::string text3 = truncate_text(read_text_from_file("Extract3.txt"), max_chars);
    std::string text4 = truncate_text(read_text_from_file("Extract4.txt"), max_chars);

    std::vector<std::pair<std::string, std::string>> text_combinations = {
        {text1, text2}, {text1, text3}, {text1, text4}, {text2, text3}, {text2, text4}, {text3, text4}, {text2, text1}, {text3, text1}, {text4, text1}, {text3, text2}, {text4, text2}, {text4, text3}};

    for (size_t i = 0; i < text_combinations.size(); ++i)
    {
        const auto &str1 = text_combinations[i].first;
        const auto &str2 = text_combinations[i].second;

        std::string suffix = "";

        if (mode == "time")
        {
            if (algorithm == "EDMemo")
                run_time_experiment("EDMemo" + suffix, [](const std::string &s1, const std::string &s2)
                                    { return editDistanceMemo(s1, s2); }, str1, str2, runs, "results_EDMemo_time" + suffix + ".csv");
            else if (algorithm == "EDDp")
                run_time_experiment("EDDp" + suffix, [](const std::string &s1, const std::string &s2)
                                    { return editDistanceDP(s1, s2); }, str1, str2, runs, "results_EDDp_time" + suffix + ".csv");
            else if (algorithm == "EDDpO")
                run_time_experiment("EDDpO" + suffix, [](const std::string &s1, const std::string &s2)
                                    { return editDistanceDPOptimized(s1, s2); }, str1, str2, runs, "results_EDDpO_time" + suffix + ".csv");
            else if (algorithm == "EDRecursivo")
                run_time_experiment("EDRecursivo" + suffix, [](const std::string &s1, const std::string &s2)
                                    { return editDistanceRecursive(s1, s2, static_cast<int>(s1.size()), static_cast<int>(s2.size())); }, str1, str2, runs, "results_EDRecursivo_time" + suffix + ".csv");
        }
        else if (mode == "memory")
        {
            if (algorithm == "EDMemo")
                run_memory_experiment("EDMemo" + suffix, [](const std::string &s1, const std::string &s2)
                                      { return editDistanceMemo(s1, s2); }, str1, str2, runs, "results_EDMemo_memory" + suffix + ".csv");
            else if (algorithm == "EDDp")
                run_memory_experiment("EDDp" + suffix, [](const std::string &s1, const std::string &s2)
                                      { return editDistanceDP(s1, s2); }, str1, str2, runs, "results_EDDp_memory" + suffix + ".csv");
            else if (algorithm == "EDDpO")
                run_memory_experiment("EDDpO" + suffix, [](const std::string &s1, const std::string &s2)
                                      { return editDistanceDPOptimized(s1, s2); }, str1, str2, runs, "results_EDDpO_memory" + suffix + ".csv");
            else if (algorithm == "EDRecursivo")
                run_memory_experiment("EDRecursivo" + suffix, [](const std::string &s1, const std::string &s2)
                                      { return editDistanceRecursive(s1, s2, static_cast<int>(s1.size()), static_cast<int>(s2.size())); }, str1, str2, runs, "results_EDRecursivo_memory" + suffix + ".csv");
        }
    }

    std::cerr << "\033[1;32mExperimento completado!\033[0m" << std::endl;

    return 0;
}
