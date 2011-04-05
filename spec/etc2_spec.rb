require 'spec_helper'

describe Etc2 do
  context 'classes' do
    # Classes that always exist
    it('should include a User class') { Etc2.constants.include?(:User).should be_true }
    it('should include a Group class') { Etc2.constants.include?(:Group).should be_true }
    
    # Classes dependent on platform
    case RUBY_PLATFORM
    when /darwin/ then
      it('should not include a Shadow class') { Etc2.constants.include?(:Shadow).should_not be_true }
      it('should return false for has_shadow?'){ Etc2.has_shadow?.should be_false}
    when /linux/ then
      it('should include a Shadow class') { Etc2.constants.include?(:Shadow).should be_true }
      it('should return true for has_shadow?'){ Etc2.has_shadow?.should be_true}
    end
  end
end