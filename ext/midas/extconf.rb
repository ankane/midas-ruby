require "mkmf-rice"

$CXXFLAGS << " -std=c++17"

ext = File.expand_path(".", __dir__)
midas = File.expand_path("../../vendor/MIDAS", __dir__)

$srcs = Dir["{#{ext},#{midas}}/*.{cc,cpp}"]
$INCFLAGS << " -I#{midas}"
$VPATH << midas

create_makefile("midas/ext")
