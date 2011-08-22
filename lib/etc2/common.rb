module Etc2
  # Mixin of common methods between the Etc2 classes
  module Common
    # @param [User,Group,Shadow] other The object to compare *self* against
    # @return *true* if the two objects are of the same class and all instance variables match
    def ==(other)
      return false unless other.class == self.class
      self.instance_variables.each do |v|
        return false unless instance_variable_get(v) == other.instance_variable_get(v)
      end
      return true
    end
  end
end