$: << File.expand_path('ext/etc2')
require 'etc2'

# Use built-in Etc module for verification since we know it works
require 'etc'

RSpec.configure do |config|
	config.color_enabled = true
end