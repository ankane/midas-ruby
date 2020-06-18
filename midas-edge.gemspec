require_relative "lib/midas/version"

Gem::Specification.new do |spec|
  spec.name          = "midas-edge"
  spec.version       = Midas::VERSION
  spec.summary       = "Edge stream anomaly detection for Ruby"
  spec.homepage      = "https://github.com/ankane/midas"
  spec.license       = "MIT"

  spec.author        = "Andrew Kane"
  spec.email         = "andrew@chartkick.com"

  spec.files         = Dir["*.{md,txt}", "{ext,lib}/**/*", "vendor/MIDAS/{LICENSE,README.md}", "vendor/MIDAS/src/*.hpp"]
  spec.require_path  = "lib"
  spec.extensions    = ["ext/midas/extconf.rb"]

  spec.required_ruby_version = ">= 2.4"

  spec.add_dependency "rice", ">= 2.2"
  spec.add_dependency "numo-narray"

  spec.add_development_dependency "bundler"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "minitest", ">= 5"
end
