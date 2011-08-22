module Etc2
  # Represents a single group on the system
  class Group
    include Common
    extend Enumerable
    
    attr_reader :name, :passwd, :gid, :mem
    alias :members :mem
    
    # Get the User objects for the Group members
    # @return [Array<User>] an Array of User objects
    def users
      mem.inject([]) do |arr, member|
        arr << Etc2::User.find(member)
      end
    end
    
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
end