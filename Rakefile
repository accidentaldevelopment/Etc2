require 'rake/extensiontask'
require 'rspec/core/rake_task'
require 'cucumber/rake/task'
require 'yard'

# because I'm too lazy to remember to change this with each update
version = open(File.expand_path('./lib/etc2.rb')).read[/\s+VERSION\s+=\s+'(.*)'/,1]

spec = Gem::Specification.new do |s|
  s.name = 'etc2'
  s.version = version
  s.author = 'Brian Faga'
  s.email = 'brian@accidentaldevelopment.com'
  s.homepage = 'http://www.accidentaldevelopment.com'
  s.summary = 'A slightly more Ruby-ish version of the Etc library'
  s.extensions << 'ext/etc2_api/extconf.rb'
  s.files = FileList['Rakefile', 'ext/etc2_api/*.c', 'ext/etc2_api/*.h', 'lib/**/*.rb', 'spec/**/*']
  s.description = <<EOD
This is basically a functional clone of the builtin Etc library, except it uses actual Ruby
objects instead of Ruby Structs.  It also adds shadow.h bindings, if available on the platform
EOD
end

# Build gem
Gem::PackageTask.new(spec).define

# Compile native code
Rake::ExtensionTask.new('etc2_api', spec)

YARD::Rake::YardocTask.new do |yard|
  yard.options = ['--no-private']
end

RSpec::Core::RakeTask.new(:specs => :compile) do |r|
  r.rspec_opts = '-f d' if ENV['FDOC']
  r.pattern = 'spec/**/*_spec.rb'
end

Cucumber::Rake::Task.new(:features => :compile) do |c|
  c.cucumber_opts = []
  c.cucumber_opts << '--format=progress' unless ENV['DOC']
  c.cucumber_opts << '--tags ~@shadow'   unless File.exists? '/etc/shadow'
  c.cucumber_opts << (Process.euid == 0 ? '--tags ~@nonroot' : '--tags ~@root')
end

task :default => :features
