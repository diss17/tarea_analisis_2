#include <cstdint>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <psapi.h>

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
        return pmc.PeakWorkingSetSize / 1024;
    }
    return 0;
}

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

template <typename Func>
void run_experiment(const std::string &algorithm_name, Func func, const std::string &str1, const std::string &str2, std::int64_t runs, const std::string &output_file)
{
    std::vector<double> times(runs);
    double mean_time = 0, time_stdev = 0, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> elapsed_time;

    std::int64_t peak_memory = 0;

    for (std::int64_t i = 0; i < runs; i++)
    {
        begin_time = std::chrono::high_resolution_clock::now();
        func(str1, str2);
        end_time = std::chrono::high_resolution_clock::now();

        elapsed_time = end_time - begin_time;
        times[i] = elapsed_time.count();
        mean_time += times[i];

        peak_memory = std::max(peak_memory, get_memory_usage());
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
        time_data << "n,mean_time,time_stdev,peak_memory_kb,text1_size,text2_size" << std::endl;
    }
    time_data << runs << "," << mean_time << "," << time_stdev << "," << peak_memory << "," << str1.size() << "," << str2.size() << std::endl;

    std::cerr << algorithm_name << " - Mean time: " << mean_time << " ns, StdDev: " << time_stdev << " ns, Peak memory: " << peak_memory << " KB" << std::endl;

    time_data.close();
}

int main(int argc, char *argv[])
{
    std::int64_t runs;
    validate_input(argc, argv, runs);

    std::string text1 = read_text_from_file("Text/Extract1.txt");
    std::string text2 = read_text_from_file("Text/Extract2.txt");
    std::string text3 = read_text_from_file("Text/Extract3.txt");
    std::string text4 = read_text_from_file("Text/Extract4.txt");

    std::vector<std::pair<std::string, std::string>> text_combinations = {
        {text1, text2}, {text1, text3}, {text1, text4}, {text2, text3}, {text2, text4}, {text3, text4}, {text2, text1}, {text3, text1}, {text4, text1}, {text3, text2}, {text4, text2}, {text4, text3}};

    for (std::size_t i = 0; i < text_combinations.size(); i++)
    {
        const std::string &str1 = text_combinations[i].first;
        const std::string &str2 = text_combinations[i].second;

        // Para medir tiempo se pueden dejar todos los algoritmos a la vez, pero para medir memoria se debe hacer solo de uno en uno.
        std::cerr << "Processing combination " << i + 1 << " of " << text_combinations.size() << std::endl;

        run_experiment("EDMemo", [](const std::string &s1, const std::string &s2)
                       { return editDistanceMemo(s1, s2); }, str1, str2, runs, "results_EDMemo.csv");

        std::cerr << "Finished EDMemo for combination " << i + 1 << std::endl;

        run_experiment("EDDp", [](const std::string &s1, const std::string &s2)
                       { return editDistanceDP(s1, s2); }, str1, str2, runs, "results_EDDp.csv");

        std::cerr << "Finished EDDp for combination " << i + 1 << std::endl;

        run_experiment("EDDpO", [](const std::string &s1, const std::string &s2)
                       { return editDistanceDPOptimized(s1, s2); }, str1, str2, runs, "results_EDDpO.csv");

        std::cerr << "Finished EDDpO for combination " << i + 1 << std::endl;
    }

    std::cerr << "\033[1;32mAll experiments completed!\033[0m" << std::endl;

    return 0;
}