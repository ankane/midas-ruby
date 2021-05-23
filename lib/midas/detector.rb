module Midas
  class Detector
    def initialize(rows: 2, buckets: 769, alpha: 0.5, threshold: nil, relations: true, directed: true, seed: 0)
      @rows = rows
      @buckets = buckets
      @alpha = alpha
      @threshold = threshold
      @relations = relations
      @directed = directed
      @seed = seed
    end

    def fit_predict(x)
      _fit_predict(x, @rows, @buckets, @alpha, @threshold || Float::NAN, @relations, @directed, @seed)
    end
  end
end
