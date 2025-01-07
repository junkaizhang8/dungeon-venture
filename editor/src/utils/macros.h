#pragma once

#include <cstdlib>

#define EPSILON 0.0001f

#define FP_EQUAL(a, b) (std::abs((a) - (b)) <= EPSILON)