$: << File.expand_path('ext/etc2')
require 'etc2'

# exit(1) unless system('make 1>/dev/null')

RSpec.configure do |config|
	config.color_enabled = true
end