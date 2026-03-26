// stdlib
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

// midas
#include <FilteringCore.hpp>
#include <NormalCore.hpp>
#include <RelationalCore.hpp>

// rice
#include <rice/rice.hpp>
#include <rice/stl.hpp>

template<typename T>
Rice::Array load_array(T& midas, Rice::Array input, bool directed) {
  Rice::Array result(input.size());

  for (auto r : input) {
    Rice::Array row(r);
    if (row.size() != 3) {
      throw Rice::Exception(rb_eArgError, "Bad shape");
    }
    int s = Rice::detail::From_Ruby<int>().convert(row[0].value());
    int d = Rice::detail::From_Ruby<int>().convert(row[1].value());
    int t = Rice::detail::From_Ruby<int>().convert(row[2].value());
    result.push(midas(s, d, t), false);
    if (!directed) {
      result.push(midas(d, s, t), false);
    }
  }

  return result;
}

// load_data from main.cpp
// modified to throw std::runtime_error when cannot find file
// instead of exiting
template<typename T>
Rice::Array load_file(T& midas, Rice::String input_file, bool directed) {
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

template<typename T>
Rice::Array fit_predict(T& midas, Rice::Object input, bool directed) {
  if (input.is_a(rb_cString)) {
    return load_file(midas, input, directed);
  } else {
    return load_array(midas, input.call("to_a"), directed);
  }
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
      "fit_predict",
      [](MIDAS::NormalCore& self, Rice::Object input, bool directed) {
        return fit_predict(self, input, directed);
      });

  Rice::define_class_under<MIDAS::RelationalCore>(rb_mMidas, "RelationalCore")
    .define_constructor(Rice::Constructor<MIDAS::RelationalCore, int, int, float>())
    .define_method(
      "update",
      [](MIDAS::RelationalCore& self, int source, int destination, int time) {
        return self(source, destination, time);
      })
    .define_method(
      "fit_predict",
      [](MIDAS::RelationalCore& self, Rice::Object input, bool directed) {
        return fit_predict(self, input, directed);
      });

  Rice::define_class_under<MIDAS::FilteringCore>(rb_mMidas, "FilteringCore")
    .define_constructor(Rice::Constructor<MIDAS::FilteringCore, int, int, float, float>())
    .define_method(
      "update",
      [](MIDAS::FilteringCore& self, int source, int destination, int time) {
        return self(source, destination, time);
      })
    .define_method(
      "fit_predict",
      [](MIDAS::FilteringCore& self, Rice::Object input, bool directed) {
        return fit_predict(self, input, directed);
      });
}
