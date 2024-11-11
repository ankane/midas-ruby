require "bundler/gem_tasks"
require "rake/testtask"
require "rake/extensiontask"
require "ruby_memcheck"

task default: :test
test_config = lambda do |t|
  t.libs << "test"
  t.pattern = "test/**/*_test.rb"
end
Rake::TestTask.new(:test, &test_config)

namespace :test do
  RubyMemcheck::TestTask.new(:valgrind, &test_config)
end

Rake::ExtensionTask.new("midas") do |ext|
  ext.name = "ext"
  ext.lib_dir = "lib/midas"
end

task :remove_ext do
  path = "lib/midas/ext.bundle"
  File.unlink(path) if File.exist?(path)
end

Rake::Task["build"].enhance [:remove_ext]
