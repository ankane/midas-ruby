require "mkmf-rice"

abort "Missing stdc++" unless have_library("stdc++")

$CXXFLAGS << " -std=c++17"

ext = File.expand_path(".", __dir__)
midas = File.expand_path("../../vendor/midas", __dir__)

$srcs = Dir["{#{ext},#{midas}}/*.{cc,cpp}"]
$INCFLAGS << " -I#{midas}"
$VPATH << midas

create_makefile("midas/ext")
