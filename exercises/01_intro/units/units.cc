// clang++ --std=c++11 units.cc -o ../../build/units && ../../build/units

#include <iostream>
#include <limits>
#include <string>

int convert_unit() {
  double measure_in, measure_out;
  std::string unit_in, unit_out;
  std::cout << "Enter measure and relative unit, space-separated.\n";
  std::cin >> measure_in >> unit_in;
  if (unit_in == "inch" || unit_in == "inches") {
    measure_out = measure_in * 25.4;
    unit_out = "mm";
  } else if (unit_in == "gallon" || unit_in == "gallons") {
    measure_out = measure_in * 4.54609;
    unit_out = "litre";
  } else {
    std::cout << "unit " << measure_in << " not known" << std::endl;
    return 1;
  }

  std::cout << measure_out << " " << unit_out << std::endl;
  return 0;
}

int main() { return convert_unit(); }
