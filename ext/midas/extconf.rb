require "mkmf-rice"

$CXXFLAGS << " -std=c++17 $(optflags)"

numo = $LOAD_PATH.find { |v| File.exist?("#{v}/numo/numo/narray.h") }
abort "Numo header not found" unless numo && find_header("numo/narray.h", "#{numo}/numo")
abort "Numo library not found" if Gem.win_platform? && !find_library("narray", nil, "#{numo}/numo")

midas = File.expand_path("../../vendor/MIDAS/src", __dir__)
abort "Midas not found" unless find_header("NormalCore.hpp", midas)

create_makefile("midas/ext")
