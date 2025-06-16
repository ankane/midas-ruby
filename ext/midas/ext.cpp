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

void load_array(std::vector<int>& src, std::vector<int>& dst, std::vector<int>& times, numo::Int32 input, bool directed) {
  auto shape = input.shape();
  if (input.ndim() != 2 || shape[1] != 3) {
    throw Rice::Exception(rb_eArgError, "Bad shape");
  }

  auto input_ptr = input.read_ptr();
  auto n = shape[0];
  auto sz = input.size();

  if (directed) {
    src.reserve(n);
    dst.reserve(n);
    times.reserve(n);

    for (size_t i = 0; i < sz; i += 3) {
      src.push_back(input_ptr[i]);
      dst.push_back(input_ptr[i + 1]);
      times.push_back(input_ptr[i + 2]);
    }
  } else {
    src.reserve(n * 2);
    dst.reserve(n * 2);
    times.reserve(n * 2);

    for (size_t i = 0; i < sz; i += 3) {
      src.push_back(input_ptr[i]);
      dst.push_back(input_ptr[i + 1]);
      times.push_back(input_ptr[i + 2]);

      src.push_back(input_ptr[i + 1]);
      dst.push_back(input_ptr[i]);
      times.push_back(input_ptr[i + 2]);
    }
  }
}

// load_data from main.cpp
// modified to throw std::runtime_error when cannot find file
// instead of exiting
void load_file(std::vector<int>& src, std::vector<int>& dst, std::vector<int>& times, Rice::String input_file, bool directed) {
  FILE* infile = fopen(input_file.c_str(), "r");
  if (infile == NULL) {
    throw std::runtime_error("Could not read file: " + input_file.str());
  }

  int s, d, t;

  if (directed) {
    while (fscanf(infile, "%d,%d,%d", &s, &d, &t) == 3) {
      src.push_back(s);
      dst.push_back(d);
      times.push_back(t);
    }
  } else {
    while (fscanf(infile, "%d,%d,%d", &s, &d, &t) == 3) {
      src.push_back(s);
      dst.push_back(d);
      times.push_back(t);

      src.push_back(d);
      dst.push_back(s);
      times.push_back(t);
    }
  }

  fclose(infile);
}

Rice::Object fit_predict(std::vector<int>& src, std::vector<int>& dst, std::vector<int>& times, int num_rows, int num_buckets, float factor, float threshold, bool relations, int seed) {
  srand(seed);
  size_t n = src.size();

  auto ary = numo::SFloat({n});
  auto result = ary.write_ptr();

  if (!std::isnan(threshold)) {
    MIDAS::FilteringCore midas(num_rows, num_buckets, threshold, factor);
    for (size_t i = 0; i < n; i++) {
      result[i] = midas(src[i], dst[i], times[i]);
    }
  } else if (relations) {
    MIDAS::RelationalCore midas(num_rows, num_buckets, factor);
    for (size_t i = 0; i < n; i++) {
      result[i] = midas(src[i], dst[i], times[i]);
    }
  } else {
    MIDAS::NormalCore midas(num_rows, num_buckets);
    for (size_t i = 0; i < n; i++) {
      result[i] = midas(src[i], dst[i], times[i]);
    }
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
        std::vector<int> src, dst, times;
        if (input.is_a(rb_cString)) {
          load_file(src, dst, times, input, directed);
        } else {
          load_array(src, dst, times, input, directed);
        }
        return fit_predict(src, dst, times, num_rows, num_buckets, factor, threshold, relations, seed);
      });
}
