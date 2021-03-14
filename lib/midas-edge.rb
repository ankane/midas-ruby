# ext
require "midas/ext"

# modules
require "midas/detector"
require "midas/version"

module Midas
  class Error < StandardError; end

  def self.new(**options)
    Detector.new(**options)
  end
end
