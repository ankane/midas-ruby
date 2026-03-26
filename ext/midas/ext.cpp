// stdlib
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

// midas
#include <FilteringCore.hpp>
#include <NormalCore.hpp>
#include <RelationalCore.hpp>

// rice
#include <rice/rice.hpp>

// load_data from main.cpp
// modified to throw std::runtime_error when cannot find file
// instead of exiting
template<typename T>
Rice::Array update_file(T& midas, Rice::String input_file, bool directed) {
  std::ifstream infile(input_file.str());
  if (!infile.is_open()) {
    throw std::runtime_error{"Could not read file: " + input_file.str()};
  }

  Rice::Array result;
  std::string line;

  while (std::getline(infile, line)) {
    int s;
    int d;
    int t;

    // TODO replace
    if (std::sscanf(line.c_str(), "%d,%d,%d", &s, &d, &t) != 3) {
      break;
    }

    result.push(midas(s, d, t), false);
    if (!directed) {
      result.push(midas(d, s, t), false);
    }
  }

  return result;
}

extern "C"
void Init_ext() {
  auto rb_mMidas = Rice::define_module("Midas");

  // TODO make seed part of Core classes
  Rice::define_class_under(rb_mMidas, "Detector")
    .define_function(
      "_set_seed",
      [](int seed) {
        std::srand(seed);
      });

  Rice::define_class_under<MIDAS::NormalCore>(rb_mMidas, "NormalCore")
    .define_constructor(Rice::Constructor<MIDAS::NormalCore, int, int>())
    .define_method(
      "update",
      [](MIDAS::NormalCore& self, int source, int destination, int time) {
        return self(source, destination, time);
      })
    .define_method(
      "update_file",
      [](MIDAS::NormalCore& self, Rice::String input, bool directed) {
        return update_file(self, input, directed);
      });

  Rice::define_class_under<MIDAS::RelationalCore>(rb_mMidas, "RelationalCore")
    .define_constructor(Rice::Constructor<MIDAS::RelationalCore, int, int, float>())
    .define_method(
      "update",
      [](MIDAS::RelationalCore& self, int source, int destination, int time) {
        return self(source, destination, time);
      })
    .define_method(
      "update_file",
      [](MIDAS::RelationalCore& self, Rice::String input, bool directed) {
        return update_file(self, input, directed);
      });

  Rice::define_class_under<MIDAS::FilteringCore>(rb_mMidas, "FilteringCore")
    .define_constructor(Rice::Constructor<MIDAS::FilteringCore, int, int, float, float>())
    .define_method(
      "update",
      [](MIDAS::FilteringCore& self, int source, int destination, int time) {
        return self(source, destination, time);
      })
    .define_method(
      "update_file",
      [](MIDAS::FilteringCore& self, Rice::String input, bool directed) {
        return update_file(self, input, directed);
      });
}
