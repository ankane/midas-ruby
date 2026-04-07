module Midas
  class Detector
    def initialize(
      rows: 2,
      buckets: 769,
      alpha: 0.5,
      threshold: nil,
      relations: true,
      seed: 0
    )
      @rows = rows
      @buckets = buckets
      @alpha = alpha
      @threshold = threshold
      @relations = relations
      @seed = seed
    end

    def update(source, destination, time)
      @core ||= core
      @core.update(source, destination, time.to_i)
    end

    def batch_update(x, directed: true)
      if x.is_a?(String)
        @core ||= core
        @core.update_file(x, directed)
      else
        x = x.to_a
        result = []
        x.each do |v|
          if !v.is_a?(Array) || v.size != 3
            raise ArgumentError, "Bad shape"
          end
          result << update(v[0], v[1], v[2])
          result << update(v[1], v[0], v[2]) if !directed
        end
        result
      end
    end

    # legacy
    alias_method :partial_fit_predict, :batch_update

    # legacy
    def fit_predict(x, directed: true)
      @core = nil # reset core
      batch_update(x, directed:)
    end

    private

    def core
      _set_seed(@seed)
      if @threshold && !@threshold.to_f.nan?
        FilteringCore.new(@rows, @buckets, @threshold, @alpha)
      elsif @relations
        RelationalCore.new(@rows, @buckets, @alpha)
      else
        NormalCore.new(@rows, @buckets)
      end
    end
  end
end
