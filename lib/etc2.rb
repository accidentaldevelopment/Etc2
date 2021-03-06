require 'etc2_api'
require 'etc2/common'
require 'etc2/user'
require 'etc2/group'

# Only require shadow.rb if the system supports it
if Etc2.has_shadow?
require 'etc2/shadow'
end

# Functional equivalent of Ruby's builtin Etc library.  But I'm hoping to make this one a little more Ruby-like.
# Uses objects instead of Structs, and includes libshadow support.
module Etc2
  # The version of the library
  VERSION = '0.2.0'
  
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