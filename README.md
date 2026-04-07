# MIDAS Ruby

[MIDAS](https://github.com/Stream-AD/MIDAS) - edge stream anomaly detection - for Ruby

[![Build Status](https://github.com/ankane/midas-ruby/actions/workflows/build.yml/badge.svg)](https://github.com/ankane/midas-ruby/actions)

## Installation

Add this line to your application’s Gemfile:

```ruby
gem "midas-edge"
```

## Getting Started

Create a detector

```ruby
midas = Midas::Detector.new
```

Update with an event

```ruby
score = midas.update(source_id, destination_id, time)
```

IDs should be integers and events should be in order by time (ascending). Higher scores are more anomalous. There is [not currently](https://github.com/Stream-AD/MIDAS/issues/4) a defined threshold for anomalies.

## Parameters

Pass parameters - default values below

```ruby
Midas::Detector.new(
  rows: 2,           # number of hash functions
  buckets: 769,      # number of buckets
  alpha: 0.5,        # temporal decay factor
  threshold: nil,    # todo
  relations: true,   # whether to use MIDAS-R or MIDAS
  seed: 0            # random seed
)
```

## Performance

For large datasets, read data directly from files

```ruby
midas.batch_update("data.csv")
```

## Resources

- [MIDAS: Microcluster-Based Detector of Anomalies in Edge Streams](https://arxiv.org/pdf/1911.04464)

## History

View the [changelog](https://github.com/ankane/midas-ruby/blob/master/CHANGELOG.md)

## Contributing

Everyone is encouraged to help improve this project. Here are a few ways you can help:

- [Report bugs](https://github.com/ankane/midas-ruby/issues)
- Fix bugs and [submit pull requests](https://github.com/ankane/midas-ruby/pulls)
- Write, clarify, or fix documentation
- Suggest or add new features

To get started with development:

```sh
git clone --recursive https://github.com/ankane/midas-ruby.git
cd midas-ruby
bundle install
bundle exec rake compile
bundle exec rake test
```
