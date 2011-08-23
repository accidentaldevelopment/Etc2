require File.expand_path('../etc2_api', __FILE__)
require File.expand_path('../etc2/common', __FILE__)
require File.expand_path('../etc2/user', __FILE__)
require File.expand_path('../etc2/group', __FILE__)

# Only require shadow.rb if the system supports it
if Etc2.has_shadow?
require File.expand_path('../etc2/shadow', __FILE__)
end

# Functional equivalent of Ruby's builtin Etc library.  But I'm hoping to make this one a little more Ruby-like.
# Uses objects instead of Structs, and includes libshadow support.
module Etc2
  # The version of the library
  VERSION = '1.0.0'
  
  # Generates a libshadow-compatible hash from the supplied data
  #
  # @overload crypt(text)
  #   Generates a hash with a randomly generated salt
  #   @param [String] text The text to hash
  # @overload crypt(text, salt)
  #   Generates a hash using the supplied salt
  #   @param [String] text The text to hash
  #   @param [String] salt The salt to use for the text
  # @overload crypt(text, opts={})
  #   Generates a hash using the supplied options
  #   @param [String] text The text to hash
  #   @param [Hash] opts Various options
  #   @option opts [String] :salt The salt to use
  # 
  # @return [String] A hash of the supplied text
  #
  def self.crypt(text, opts=nil)
    if opts == nil
      c_crypt(text)
    elsif opts.class == String
      c_crypt(text,opts)
    elsif opts.class == Hash
      if opts[:salt]
        c_crypt(text, opts[:salt])
      end
    end
  end
end