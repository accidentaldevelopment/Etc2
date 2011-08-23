module Etc2
  # Represents a shadow (spwd) entry on the system
  # @note 
  #   This class only exists if shadow was present when the library was compiled.
  #   It won't be present on platforms like Mac OS X
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

if Etc2.has_shadow?  
    # @return [User] The User object that own this shadow entry
    def user
      Etc2::User.find(self.name)
    end
    
    # Iterate through each Shadow on the system
    # @yield [shadow] Executes the block for each shadow entry
    # @yieldparam shadow The Shadow object
    def self.each
      setspent
      shadow, shadows = nil, []
      while(shadow = getspent) do
        yield shadow if block_given?
        shadows << shadow
      end
      block_given? ? shadows : Enumerator.new(shadows)
    end
  end
end
end