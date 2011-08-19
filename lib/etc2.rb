require File.expand_path('../etc2_api', __FILE__)

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
  
  # Mixin of common methods between the Etc2 classes
  module Common
    def ==(other)
      return false unless other.class == self.class
      self.instance_variables.each do |v|
        return false unless instance_variable_get(v) == other.instance_variable_get(v)
      end
      return true
    end
  end
  
  # Represents a single user on the system
  class User
    include Common
    extend Enumerable
    
    def self.each
      setpwent
      user, users = nil, []
      while(user = getpwent) do
        yield user if block_given?
        users << user
      end
      users
    end
  end
  
  # Represents a single group on the system
  class Group
    include Common
    extend Enumerable
    
    def self.each
      setgrent
      group, groups = nil, []
      while(group = getgrent) do
        yield user if block_given?
        groups << group
      end
      groups
    end
  end
  
  
  # Represents a shadow (spwd) entry on the system
  class Shadow
    include Common
    extend Enumerable
    
    def self.each
      setspent
      shadow, shadows = nil, []
      while(shadow = getspent) do
        yield shadow if block_given?
        shadows << shadow
      end
      shadows
    end
  end
end