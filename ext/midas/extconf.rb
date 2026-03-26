require "mkmf-rice"

$CXXFLAGS << " -std=c++17 $(optflags)"

midas = File.expand_path("../../vendor/MIDAS/src", __dir__)
abort "Midas not found" unless find_header("NormalCore.hpp", midas)

create_makefile("midas/ext")
