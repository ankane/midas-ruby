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
      result =
        if x.is_a?(String)
          _fit_predict_file(x, @rows, @buckets, @alpha, @threshold || Float::NAN, @relations, @directed, @seed)
        else
          x = Numo::Int32.cast(x) unless x.is_a?(Numo::NArray)
          x = x.cast_to(Numo::Int32) unless x.is_a?(Numo::Int32)
          raise ArgumentError, "Bad shape: #{x.shape}" unless x.rank == 2 && x.shape[1] == 3
          _fit_predict_str(x.to_binary, @rows, @buckets, @alpha, @threshold || Float::NAN, @relations, @directed, @seed)
        end

      Numo::SFloat.from_binary(result)
    end
  end
end
