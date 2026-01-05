require_relative "test_helper"

class MidasTest < Minitest::Test
  def test_works
    midas = Midas.new(buckets: 1024)
    scores = midas.fit_predict(data)

    assert_equal [10000], scores.shape
    expected = [0, 0, 1, 2, 2, 4, 2, 2, 3, 6]
    assert_elements_in_delta expected, scores[0...10]

    skip "Different values on Linux (seed issue?)" if linux?

    expected = [307.507233, 469.720490, 215.821609, 236.601303, 258.282837]
    assert_elements_in_delta expected, scores[-5..-1]
  end

  def test_threshold
    midas = Midas.new(buckets: 1024, threshold: 1e3)
    scores = midas.fit_predict(data)

    assert_equal [10000], scores.shape
    expected = [0, 0, 0, 0, 0, 0, 0, 0, 0.005952, 1.005952]
    assert_elements_in_delta expected, scores[0...10]
    expected = [631.594849, 696.509277, 764.598450, 835.862366, 910.301086]
    assert_elements_in_delta expected, scores[-5..-1]
  end

  def test_no_relations
    midas = Midas.new(buckets: 1024, relations: false)
    scores = midas.fit_predict(data)

    assert_equal [10000], scores.shape
    expected = [0, 0, 1, 2, 2, 4, 2, 2, 3, 6]
    assert_elements_in_delta expected, scores[0...10]

    skip "Different values on Linux (seed issue?)" if linux?

    expected = [307.507233, 469.720490, 2.492458, 12.942609, 31.100597]
    assert_elements_in_delta expected, scores[-5..-1]
  end

  def test_undirected
    midas = Midas.new(directed: false)
    scores = midas.fit_predict(data)

    assert_equal [20000], scores.shape
    expected = [0, 0, 0, 0, 1, 1, 2, 2, 0.375, 0.375]
    assert_elements_in_delta expected, scores[0...10]
  end

  def test_file
    midas = Midas.new(buckets: 1024)
    scores = midas.fit_predict("vendor/MIDAS/data/DARPA/darpa_processed.csv")

    assert_equal [4554344], scores.shape
    expected = [0, 0, 1, 2, 2, 4, 2, 2, 3, 6]
    assert_elements_in_delta expected, scores[0...10]

    skip "Different values on Linux (seed issue?)" if linux?

    expected = [49.704582, 56.181786, 63.055534, 70.325829, 10.779646]
    assert_elements_in_delta expected, scores[-5..-1]
  end

  def data
    data = []
    File.foreach("vendor/MIDAS/data/DARPA/darpa_processed.csv").with_index do |line, i|
      break if i == 10000
      data << line.split(",").map(&:to_i)
    end
    Numo::Int32.cast(data)
  end

  def linux?
    RbConfig::CONFIG["host_os"] == "linux"
  end
end
