#ifndef HELPERS_HXX
#define HELPERS_HXX
#include <functional>
#include <random>

using ProbabilityGenerator = std::function<double()>;
extern ProbabilityGenerator probability_generator;
double default_probability_generator();
#endif

