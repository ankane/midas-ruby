// stdlib
#include <iostream>
#include <vector>

// midas
#include <FilteringCore.hpp>
#include <NormalCore.hpp>
#include <RelationalCore.hpp>

// rice
#include <rice/Module.hpp>
#include <rice/String.hpp>

void load_str(std::vector<int>& src, std::vector<int>& dst, std::vector<int>& times, const std::string& input, bool directed) {
  int* input_ptr = (int*) input.data();
  size_t n = input.size() / sizeof(int);

  if (directed) {
    for (size_t i = 0; i < n; i += 3) {
      src.push_back(input_ptr[i]);
      dst.push_back(input_ptr[i + 1]);
      times.push_back(input_ptr[i + 2]);
    }
  } else {
    for (size_t i = 0; i < n; i += 3) {
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
void load_file(std::vector<int>& src, std::vector<int>& dst, std::vector<int>& times, const std::string& input_file, bool undirected) {
  FILE* infile = fopen(input_file.c_str(), "r");
  if (infile == NULL) {
    throw std::runtime_error("Could not read file: " + input_file);
  }

  int s, d, t;

  if (undirected == false) {
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
}

std::string fit_predict(std::vector<int>& src, std::vector<int>& dst, std::vector<int>& times, int num_rows, int num_buckets, float factor, float threshold, bool relations, int seed) {
  srand(seed);
  size_t n = src.size();
  std::vector<float> result;
  result.reserve(n);

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

  // std::string copies data
  return std::string((char*) result.data(), sizeof(float) / sizeof(char) * n);
}

extern "C"
void Init_ext() {
  auto rb_mMidas = Rice::define_module("Midas");

  Rice::define_class_under(rb_mMidas, "Detector")
    .define_method(
      "_fit_predict_str",
      *[](const std::string& input, int num_rows, int num_buckets, float factor, float threshold, bool relations, bool directed, int seed) {
        std::vector<int> src, dst, times;
        load_str(src, dst, times, input, directed);
        return fit_predict(src, dst, times, num_rows, num_buckets, factor, threshold, relations, seed);
      })
    .define_method(
      "_fit_predict_file",
      *[](const std::string& input, int num_rows, int num_buckets, float factor, float threshold, bool relations, bool directed, int seed) {
        std::vector<int> src, dst, times;
        load_file(src, dst, times, input, !directed);
        return fit_predict(src, dst, times, num_rows, num_buckets, factor, threshold, relations, seed);
      });
}
