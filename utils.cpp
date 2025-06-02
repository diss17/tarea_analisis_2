#ifndef UTILS
#define UTILS

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

inline void validate_input(int argc, char *argv[], std::int64_t& runs)
{
    if (argc != 2) { // Cambiar a 2 argumentos: <RUNS>
        std::cerr << "Usage: <RUNS>" << std::endl;
        std::cerr << "<RUNS>: numbers of runs per test case: should be >= 32." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Read command line arguments
    try {
        runs = std::stoll(argv[1]);
    } catch (std::invalid_argument const& ex) {
        std::cerr << "std::invalid_argument::what(): " << ex.what() << std::endl;
        std::exit(EXIT_FAILURE);
    } catch (std::out_of_range const& ex) {
        std::cerr << "std::out_of_range::what(): " << ex.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Validate arguments
    if (runs < 32) { // Cambiar el mínimo de <RUNS> a 32
        std::cerr << "<RUNS> must be at least 32 for statistical confidence." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#endif