module Etc2
  # Represents a single user on the system
  class User
    include Common
    extend Enumerable
    
    attr_reader :name, :uid, :gid, :gecos, :dir, :shell
    alias :full_name :gecos
    alias :home_dir  :dir
    alias :homedir   :dir
    alias :home      :dir
    
    # Iterate through each User on the system
    # @yield [user] Executes the block for each user
    # @yieldparam user The User object
    def self.each
      setpwent
      user, users = nil, []
      while(user = getpwent) do
        yield user if block_given?
        users << user
      end
      block_given? ? users : Enumerator.new(users)
    end
    
    # @return the Group object for this User's primary group
    def group
      Etc2::Group.find(self.gid)
    end
    
if Etc2.has_shadow?
    # Grabs the Shadow object for this user.  
    # @note This method only exists if shadow is available on the system
    # @return the Shadow entry for this User
    def shadow
      Etc2::Shadow.find(self.name)
    end
  end
end
end