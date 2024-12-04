#include "cpp_argv.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <tbb/tbb.h>

#define DEFAULT_NAME "pearson"

/**
 * @brief Data measurement set.
 *
 */
struct Data_Set {
  size_t n;  /** Number of measurements.  */
  double *x; /** Variable X measurements. */
  double *y; /** Variable Y measurements. */
};

/**
 * @brief Pearson correlation.
 *
 */
struct Correlation {
  double a; /** Right slope.         */
  double b; /** Y-axis shift.        */
  double r; /** Pearson coefficient. */
};

/**
 * @brief Loads a data set from a input stream then returns it.
 *
 * @param stream The input stream.
 * @return Data_Set The data set.
 */
Data_Set load_file(std::istream &stream) noexcept;

/**
 * @brief Calculates then returns the Pearson correlation of a data set.
 *
 * @param data_set The data set.
 * @return Correlation The corresponding Pearson correlation.
 */
Correlation calculate(const Data_Set &data_set) noexcept;

/**
 * @brief Main program.
 *
 * @param argc number of arguments in the command line.
 * @param argv arguments of the command line.
 * @return @c EXIT_SUCCESS if command succeeds else @c EXIT_FAILURE.
 */
int main(int argc, char *argv[]) {

  // User expects help.
  CPP_ARGV_TEST_HELP_REQUEST(argc, argv[0], DEFAULT_NAME, "filename")

  // Bad argument number.
  CPP_ARGV_TEST_ARG_NUM(argc, 2)

  // Retrieves the data filename.
  const char *const filename = argv[1];

  // Tries to open it.
  std::ifstream stream(filename);
  if (not stream) {
    std::cerr << std::strerror(errno) << std::endl;
    return EXIT_FAILURE;
  }

  // Loads the data set.
  const Data_Set data_set = load_file(stream);

  // Calculates the corresponding Pearson correlation.
  const Correlation result = calculate(data_set);

  // Prints results onto the standard output.
  std::cout << "a: " << result.a << "\tb: " << result.b << "\tr: " << result.r
            << std::endl;

  // It's over.
  return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/*                                  load_file                                 */
/* -------------------------------------------------------------------------- */

Data_Set load_file(std::istream &stream) noexcept {
  Data_Set res;

  stream >> res.n;
  res.x = new double[res.n];
  res.y = new double[res.n];

  for (size_t i = 0; i < res.n; i++) {
    stream >> res.x[i] >> res.y[i];
  }

  return res;
}

/* -------------------------------------------------------------------------- */
/*                                  calculate                                 */
/* -------------------------------------------------------------------------- */


// utilisation parallel_ reduce de tbb

struct PartialSums {
    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_xx = 0.0;
    double sum_yy = 0.0;
    double sum_xy = 0.0;
    size_t n = 0;

    PartialSums() = default; // pour le constructeur par défaut

    // combinaison des res partiels
    PartialSums(const PartialSums& a, const PartialSums& b) {
        sum_x = a.sum_x + b.sum_x;
        sum_y = a.sum_y + b.sum_y;
        sum_xx = a.sum_xx + b.sum_xx;
        sum_yy = a.sum_yy + b.sum_yy;
        sum_xy = a.sum_xy + b.sum_xy;
        n = a.n + b.n;
    }

    // fusion 
    void operator+=(const PartialSums& other) {
        sum_x += other.sum_x;
        sum_y += other.sum_y;
        sum_xx += other.sum_xx;
        sum_yy += other.sum_yy;
        sum_xy += other.sum_xy;
        n += other.n;
    }
};

Correlation calculate(const Data_Set &data_set) noexcept {
    PartialSums total = tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, data_set.n),
        PartialSums(),
        [&](const tbb::blocked_range<size_t>& range, PartialSums partial) {
            for (size_t i = range.begin(); i < range.end(); ++i) {
                const double x = data_set.x[i];
                const double y = data_set.y[i];
                partial.sum_x += x;
                partial.sum_y += y;
                partial.sum_xx += x * x;
                partial.sum_yy += y * y;
                partial.sum_xy += x * y;
                partial.n++;
            }
            return partial;
        },
        [](const PartialSums& a, const PartialSums& b) {
            return PartialSums(a, b);
        }
    );

    const double mean_x = total.sum_x / total.n;
    const double mean_y = total.sum_y / total.n;

    const double covariance = total.sum_xy - total.n * mean_x * mean_y;
    const double variance_x = total.sum_xx - total.n * mean_x * mean_x;
    const double variance_y = total.sum_yy - total.n * mean_y * mean_y;

    Correlation res;
    res.a = covariance / variance_x;
    res.b = mean_y - res.a * mean_x;
    res.r = covariance / std::sqrt(variance_x * variance_y);

    return res;
}