# dependencies
require "numo/narray"

# ext
require "midas/ext"

# modules
require_relative "midas/detector"
require_relative "midas/version"

module Midas
  class Error < StandardError; end

  def self.new(**options)
    Detector.new(**options)
  end
end
