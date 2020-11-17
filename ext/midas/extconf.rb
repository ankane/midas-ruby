require "mkmf-rice"

$CXXFLAGS << " -std=c++11"

midas = File.expand_path("../../vendor/MIDAS/src", __dir__)
$INCFLAGS << " -I#{midas}"

create_makefile("midas/ext")
