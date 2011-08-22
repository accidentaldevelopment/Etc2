module Etc2
  # Represents a shadow (spwd) entry on the system
  class Shadow
    include Common
    extend Enumerable
    
    attr_reader :name, :passwd
    alias :password :passwd
  	alias :encrypted_password :passwd
    
    # @private
    def initialize
      raise NotImplementedError.new('shadow not available on this platform') unless Etc2.has_shadow?
    end
    
    # @return [User] The User object that own this shadow entry
    def user
      Etc2::User.find(self.name)
    end
    
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