#include "stdral.h"

#include <math.h>

int ral_div(int numerator, int denominator) {
  int d = numerator / denominator;
  int r = numerator % denominator;
  if (r < 0) {
    r += denominator;
  }
  return (numerator - r) / denominator;
}

int mod(int numerator, int denominator) {
  int d = numerator / denominator;
  int r = numerator % denominator;
  if (r < 0) {
    r += denominator;
  }
  return r;
}

int ral_int(double value) { return static_cast<int>(round(value)); }
