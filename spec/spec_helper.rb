require 'etc2'

# Use built-in Etc module for verification since we know it works
require 'etc'

RSpec.configure do |config|
	config.color_enabled = true
end

# @return true if platform is darwin
def darwin?
  RUBY_PLATFORM =~ /darwin/
end

# @return true if platform is linux
def linux?
  RUBY_PLATFORM =~ /linux/
end

# @return true if user is root
def root?
  Process.euid == 0 || Process.uid == 0
end

# @return true if the system includes shadow
def shadow?
  Etc2.has_shadow?
end