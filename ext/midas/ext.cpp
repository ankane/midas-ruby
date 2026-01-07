// stdlib
#include <cstdio>
#include <iostream>
#include <vector>

// midas
#include <FilteringCore.hpp>
#include <NormalCore.hpp>
#include <RelationalCore.hpp>

// rice
#include <rice/rice.hpp>
#include <rice/stl.hpp>

// numo
#include "numo.hpp"

template<typename T>
void load_array(T& midas, Rice::Array input, bool directed, std::vector<float>& result) {
  for (auto r : input) {
    Rice::Array row(r);
    int s = Rice::detail::From_Ruby<int>().convert(row[0].value());
    int d = Rice::detail::From_Ruby<int>().convert(row[1].value());
    int t = Rice::detail::From_Ruby<int>().convert(row[2].value());
    result.push_back(midas(s, d, t));
    if (!directed) {
      result.push_back(midas(d, s, t));
    }
  }
}

template<typename T>
void load_numo_array(T& midas, numo::Int32 input, bool directed, std::vector<float>& result) {
  auto shape = input.shape();
  if (input.ndim() == 1 && shape[0] == 0) {
    return;
  }

  if (input.ndim() != 2 || shape[1] != 3) {
    throw Rice::Exception(rb_eArgError, "Bad shape");
  }

  auto input_ptr = input.read_ptr();
  auto sz = input.size();

  for (size_t i = 0; i < sz; i += 3) {
    int s = input_ptr[i];
    int d = input_ptr[i + 1];
    int t = input_ptr[i + 2];
    result.push_back(midas(s, d, t));
    if (!directed) {
      result.push_back(midas(d, s, t));
    }
  }
}

// load_data from main.cpp
// modified to throw std::runtime_error when cannot find file
// instead of exiting
template<typename T>
void load_file(T& midas, Rice::String input_file, bool directed, std::vector<float>& result) {
  FILE* infile = fopen(input_file.c_str(), "r");
  if (infile == NULL) {
    throw std::runtime_error("Could not read file: " + input_file.str());
  }

  int s, d, t;
  while (fscanf(infile, "%d,%d,%d", &s, &d, &t) == 3) {
    result.push_back(midas(s, d, t));
    if (!directed) {
      result.push_back(midas(d, s, t));
    }
  }

  fclose(infile);
}

template<typename T>
Rice::Object fit_predict(T& midas, Rice::Object input, bool directed) {
  std::vector<float> result;
  if (input.is_a(rb_cString)) {
    load_file(midas, input, directed, result);
  // TODO uncomment in 0.6.0
  // } else if (input.is_instance_of(rb_cArray)) {
  //   load_array(midas, input, directed, result);
  } else {
    load_numo_array(midas, input, directed, result);
  }

  size_t n = result.size();
  auto ary = numo::SFloat({n});
  auto out = ary.write_ptr();
  for (size_t i = 0; i < n; i++) {
    out[i] = result[i];
  }
  return ary;
}

extern "C"
void Init_ext() {
  auto rb_mMidas = Rice::define_module("Midas");

  // TODO make seed part of Core classes
  Rice::define_class_under(rb_mMidas, "Detector")
    .define_function(
      "_set_seed",
      [](int seed) {
        srand(seed);
      });

  Rice::define_class_under<MIDAS::NormalCore>(rb_mMidas, "NormalCore")
    .define_constructor(Rice::Constructor<MIDAS::NormalCore, int, int>())
    .define_method(
      "fit_predict",
      [](MIDAS::NormalCore& self, Rice::Object input, bool directed) {
        return fit_predict(self, input, directed);
      });

  Rice::define_class_under<MIDAS::RelationalCore>(rb_mMidas, "RelationalCore")
    .define_constructor(Rice::Constructor<MIDAS::RelationalCore, int, int, float>())
    .define_method(
      "fit_predict",
      [](MIDAS::RelationalCore& self, Rice::Object input, bool directed) {
        return fit_predict(self, input, directed);
      });

  Rice::define_class_under<MIDAS::FilteringCore>(rb_mMidas, "FilteringCore")
    .define_constructor(Rice::Constructor<MIDAS::FilteringCore, int, int, float, float>())
    .define_method(
      "fit_predict",
      [](MIDAS::FilteringCore& self, Rice::Object input, bool directed) {
        return fit_predict(self, input, directed);
      });
}
