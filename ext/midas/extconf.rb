require "mkmf-rice"

$CXXFLAGS << " -std=c++17"

midas = File.expand_path("../../vendor/MIDAS/src", __dir__)
$INCFLAGS << " -I#{midas}"

create_makefile("midas/ext")
