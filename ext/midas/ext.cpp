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
  if (directed) {
    for (auto r : input) {
      Rice::Array row(r);
      int s = Rice::detail::From_Ruby<int>().convert(row[0].value());
      int d = Rice::detail::From_Ruby<int>().convert(row[1].value());
      int t = Rice::detail::From_Ruby<int>().convert(row[2].value());
      result.push_back(midas(s, d, t));
    }
  } else {
    for (auto r : input) {
      Rice::Array row(r);
      int s = Rice::detail::From_Ruby<int>().convert(row[0].value());
      int d = Rice::detail::From_Ruby<int>().convert(row[1].value());
      int t = Rice::detail::From_Ruby<int>().convert(row[2].value());
      result.push_back(midas(s, d, t));
      result.push_back(midas(d, s, t));
    }
  }
}

template<typename T>
void load_numo_array(T& midas, numo::Int32 input, bool directed, std::vector<float>& result) {
  auto shape = input.shape();
  if (input.ndim() != 2 || shape[1] != 3) {
    throw Rice::Exception(rb_eArgError, "Bad shape");
  }

  auto input_ptr = input.read_ptr();
  auto sz = input.size();

  if (directed) {
    for (size_t i = 0; i < sz; i += 3) {
      result.push_back(midas(input_ptr[i], input_ptr[i + 1], input_ptr[i + 2]));
    }
  } else {
    for (size_t i = 0; i < sz; i += 3) {
      result.push_back(midas(input_ptr[i], input_ptr[i + 1], input_ptr[i + 2]));
      result.push_back(midas(input_ptr[i + 1], input_ptr[i], input_ptr[i + 2]));
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

  if (directed) {
    while (fscanf(infile, "%d,%d,%d", &s, &d, &t) == 3) {
      result.push_back(midas(s, d, t));
    }
  } else {
    while (fscanf(infile, "%d,%d,%d", &s, &d, &t) == 3) {
      result.push_back(midas(s, d, t));
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
  } else if (input.is_instance_of(rb_cArray)) {
    load_array(midas, input, directed, result);
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

  Rice::define_class_under(rb_mMidas, "Detector")
    .define_function(
      "_fit_predict",
      [](Rice::Object input, int num_rows, int num_buckets, float factor, float threshold, bool relations, bool directed, int seed) {
        srand(seed);

        if (!std::isnan(threshold)) {
          MIDAS::FilteringCore midas(num_rows, num_buckets, threshold, factor);
          return fit_predict(midas, input, directed);
        } else if (relations) {
          MIDAS::RelationalCore midas(num_rows, num_buckets, factor);
          return fit_predict(midas, input, directed);
        } else {
          MIDAS::NormalCore midas(num_rows, num_buckets);
          return fit_predict(midas, input, directed);
        }
      });
}
