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
      threshold = @threshold || Float::NAN
      if x.is_a?(String)
        _fit_predict_file(x, @rows, @buckets, @alpha, threshold, @relations, @directed, @seed)
      else
        _fit_predict_array(x.to_a, @rows, @buckets, @alpha, threshold, @relations, @directed, @seed)
      end
    end
  end
end
