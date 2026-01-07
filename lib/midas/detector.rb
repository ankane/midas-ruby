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
      # TODO move core (including seed) to initialize
      _set_seed(@seed)
      core =
        if @threshold && !@threshold.to_f.nan?
          FilteringCore.new(@rows, @buckets, @threshold, @alpha)
        elsif @relations
          RelationalCore.new(@rows, @buckets, @alpha)
        else
          NormalCore.new(@rows, @buckets)
        end
      core.fit_predict(x, @directed)
    end
  end
end
