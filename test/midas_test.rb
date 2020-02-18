require_relative "test_helper"

class MidasTest < Minitest::Test
  def test_works
    midas = Midas.new
    scores = midas.fit_predict(data)

    assert_equal [10000], scores.shape
    expected = [0.0, 0.0, 0.693147, 1.098612, 0.693147, 1.098612, 0.693147, 0.693147, 0.693147, 0.693147]
    assert_elements_in_delta expected, scores[0...10]
  end

  def test_no_relations
    midas = Midas.new(relations: false)
    scores = midas.fit_predict(data)

    assert_equal [10000], scores.shape
    expected = [0, 0, 1, 2, 1, 2, 1, 1, 1, 1]
    assert_elements_in_delta expected, scores[0...10]
  end

  def test_undirected
    skip "Segmentation fault with MIDAS CLI so nothing to compare"

    midas = Midas.new(directed: false)
    scores = midas.fit_predict(data)

    assert_equal [20000], scores.shape
    expected = []
    assert_elements_in_delta expected, scores[0...10]
  end

  def test_file
    midas = Midas.new
    scores = midas.fit_predict("vendor/MIDAS/example.csv")

    assert_equal [2000000], scores.shape
    expected = [0.0, 0.0, 0.693147, 1.098612, 0.693147, 1.098612, 0.693147, 0.693147, 0.693147, 0.693147]
    assert_elements_in_delta expected, scores[0...10]
    expected = [14.613912, 14.613986, 14.614060, 14.614135, 14.614209]
    assert_elements_in_delta expected, scores[-5..-1]
  end

  def test_example
    data = [
      [2, 3, 1],
      [2, 3, 1],
      [3, 4, 2],
      [3, 4, 2]
    ]
    midas = Midas.new
    scores = midas.fit_predict(data)

    assert_equal [4], scores.shape
    expected = [0.0, 0.0, 0.693147, 1.098612]
    assert_elements_in_delta expected, scores.to_a
  end

  def data
    data = []
    File.foreach("vendor/MIDAS/example.csv").with_index do |line, i|
      break if i == 10000
      data << line.split(",").map(&:to_i)
    end
    Numo::Int32.cast(data)
  end
end
