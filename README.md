# MIDAS

[MIDAS](https://github.com/bhatiasiddharth/MIDAS) - edge stream anomaly detection - for Ruby

[![Build Status](https://travis-ci.org/ankane/midas.svg?branch=master)](https://travis-ci.org/ankane/midas)

## Installation

Add these lines to your application’s Gemfile:

```ruby
gem 'midas-edge'
gem 'rice', github: 'jasonroelofs/rice' # for now for c++17
```

## Getting Started

Prep your data in the format `[source, destination, time]` (all integers) and sorted by time (ascending)

```ruby
data = [
  [2, 3, 1],
  [3, 4, 2],
  [5, 9, 2],
  [2, 3, 3]
]
```

Get anomaly scores

```ruby
midas = Midas.new
scores = midas.fit_predict(data)
```

Higher scores are more anomalous. There is [not currently](https://github.com/bhatiasiddharth/MIDAS/issues/4) a defined threshold for anomalies.

## Parameters

Pass parameters - default values below

```ruby
Midas.new(
  rows: 2,           # number of hash functions
  buckets: 769,      # number of buckets
  alpha: 0.6,        # temporal decay factor
  threshold: nil,    # [unreleased]
  relations: true,   # whether to use MIDAS-R or MIDAS
  directed: true     # treat the graph as directed or undirected
)
```

## Data

Data can be an array of arrays

```ruby
[[1, 2, 3], [4, 5, 6]]
```

Or a Numo NArray

```ruby
Numo::Int32.new(3, 2).seq
```

## Performance

For large datasets, read data directly from files

```ruby
midas.fit_predict("data.csv")
```

## Resources

- [MIDAS: Microcluster-Based Detector of Anomalies in Edge Streams](https://www.comp.nus.edu.sg/~sbhatia/assets/pdf/midas.pdf)

## History

View the [changelog](https://github.com/ankane/midas/blob/master/CHANGELOG.md)

## Contributing

Everyone is encouraged to help improve this project. Here are a few ways you can help:

- [Report bugs](https://github.com/ankane/midas/issues)
- Fix bugs and [submit pull requests](https://github.com/ankane/midas/pulls)
- Write, clarify, or fix documentation
- Suggest or add new features

To get started with development:

```sh
git clone --recursive https://github.com/ankane/midas.git
cd midas
bundle install
bundle exec rake compile
bundle exec rake test
```
