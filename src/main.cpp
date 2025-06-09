#include "uhrPro.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdint>
#include <cstdlib> 

int main(int argc, char *argv[]) {
    std::int64_t runs;
    std::size_t max_chars;
    std::string mode, algorithm;

    
    std::cout << "Ingrese el numero de iteraciones: ";
    std::cin >> runs;
    std::cout << "Ingrese la cantidad maxima de caracteres: ";
    std::cin >> max_chars;

    // Menú para elegir modo
    int mode_option = 0;
    std::cout << "Seleccione el modo:\n";
    std::cout << "  1. Tiempo\n";
    std::cout << "  2. Memoria\n";
    do {
        std::cout << "Opción (1-2): ";
        std::cin >> mode_option;
    } while (mode_option < 1 || mode_option > 2);

    mode = (mode_option == 1) ? "time" : "memory";

    // Menú para elegir algoritmo
    std::cout << "Seleccione el algoritmo a probar:\n";
    std::cout << "  1. EDMemo\n";
    std::cout << "  2. EDDp\n";
    std::cout << "  3. EDDpO\n";
    std::cout << "  4. EDRecursivo\n";
    int alg_option = 0;
    do {
        std::cout << "Opcion (1-4): ";
        std::cin >> alg_option;
    } while (alg_option < 1 || alg_option > 4);

    switch (alg_option) {
        case 1: algorithm = "EDMemo"; break;
        case 2: algorithm = "EDDp"; break;
        case 3: algorithm = "EDDpO"; break;
        case 4: algorithm = "EDRecursivo"; break;
    }

    std::string text1 = truncate_text(read_text_from_file("../Text/Extract1.txt"), max_chars);
    std::string text2 = truncate_text(read_text_from_file("../Text/Extract2.txt"), max_chars);
    std::string text3 = truncate_text(read_text_from_file("../Text/Extract3.txt"), max_chars);
    std::string text4 = truncate_text(read_text_from_file("../Text/Extract4.txt"), max_chars);

    std::vector<std::pair<std::string, std::string>> text_combinations = {
        {text1, text2}, {text1, text3}, {text1, text4}, {text2, text3},
        {text2, text4}, {text3, text4}, {text2, text1}, {text3, text1},
        {text4, text1}, {text3, text2}, {text4, text2}, {text4, text3}
    };

    // Menú principal extendido
    int main_option = 0;
    std::cout << "Seleccione una opción:\n";
    std::cout << "  1. Ejecutar experimento\n";
    std::cout << "  2. Graficar resultados de tiempo\n";
    std::cout << "  3. Graficar resultados de memoria\n";
    do {
        std::cout << "Opción (1-3): ";
        std::cin >> main_option;
    } while (main_option < 1 || main_option > 3);

    if (main_option == 2 || main_option == 3) {
        char incluir_recursivo = 'n';
        std::cout << "¿Incluir el algoritmo recursivo en la gráfica? (s/n): ";
        std::cin >> incluir_recursivo;
        std::string arg = (incluir_recursivo == 's' || incluir_recursivo == 'S') ? "1" : "0";
        if (main_option == 2) {
            system(("python ../src/Graficar.py " + arg).c_str());
            return 0;
        }
        if (main_option == 3) {
            system(("python ../src/GraficoMemoria.py " + arg).c_str());
            return 0;
        }
    }

    for (size_t i = 0; i < text_combinations.size(); ++i) {
        const auto &str1 = text_combinations[i].first;
        const auto &str2 = text_combinations[i].second;

        std::string suffix = "";

        if (mode == "time") {
            if (algorithm == "EDMemo")
                run_time_experiment("EDMemo", [](const std::string &s1, const std::string &s2) {
                    return editDistanceMemo(s1, s2);
                }, str1, str2, runs, "../Tiempo/results_EDMemo.csv");

            else if (algorithm == "EDDp")
                run_time_experiment("EDDp", [](const std::string &s1, const std::string &s2) {
                    return editDistanceDP(s1, s2);
                }, str1, str2, runs, "../Tiempo/results_EDDp.csv");

            else if (algorithm == "EDDpO")
                run_time_experiment("EDDpO", [](const std::string &s1, const std::string &s2) {
                    return editDistanceDPOptimized(s1, s2);
                }, str1, str2, runs, "../Tiempo/results_EDDpO.csv");

            else if (algorithm == "EDRecursivo")
                run_time_experiment("EDRecursivo", [](const std::string &s1, const std::string &s2) {
                    return editDistanceRecursive(s1, s2, static_cast<int>(s1.size()), static_cast<int>(s2.size()));
                }, str1, str2, runs, "../Tiempo/results_EDRecursivo.csv");
        }

        else if (mode == "memory") {
            if (algorithm == "EDMemo")
                run_memory_experiment("EDMemo", [](const std::string &s1, const std::string &s2) {
                    return editDistanceMemo(s1, s2);
                }, str1, str2, runs, "../Memoria/results_EDMemo.csv");

            else if (algorithm == "EDDp")
                run_memory_experiment("EDDp", [](const std::string &s1, const std::string &s2) {
                    return editDistanceDP(s1, s2);
                }, str1, str2, runs, "../Memoria/results_EDDp.csv");

            else if (algorithm == "EDDpO")
                run_memory_experiment("EDDpO", [](const std::string &s1, const std::string &s2) {
                    return editDistanceDPOptimized(s1, s2);
                }, str1, str2, runs, "../Memoria/results_EDDpO.csv");

            else if (algorithm == "EDRecursivo")
                run_memory_experiment("EDRecursivo", [](const std::string &s1, const std::string &s2) {
                    return editDistanceRecursive(s1, s2, static_cast<int>(s1.size()), static_cast<int>(s2.size()));
                }, str1, str2, runs, "../Memoria/results_EDRecursivo.csv");
        }
    }

    std::cerr << "\033[1;32mExperimento completado!\033[0m" << std::endl;
    return 0;
}
