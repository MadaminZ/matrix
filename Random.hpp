#include <random>

template <typename input_type>
input_type random(input_type input_max, input_type input_min) {
  std::random_device random;
  std::mt19937 generator(random());
  std::uniform_int_distribution<input_type> result(input_min, input_max);

  return result(generator);
}