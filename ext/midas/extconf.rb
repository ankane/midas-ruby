require "mkmf-rice"
require "numo/narray"

$CXXFLAGS << " -std=c++17 $(optflags)"

numo = File.join(Gem.loaded_specs["numo-narray"].require_path, "numo")
abort "Numo header not found" unless find_header("numo/narray.h", numo)
abort "Numo library not found" if Gem.win_platform? && !find_library("narray", nil, numo)

midas = File.expand_path("../../vendor/MIDAS/src", __dir__)
abort "Midas not found" unless find_header("NormalCore.hpp", midas)

create_makefile("midas/ext")
