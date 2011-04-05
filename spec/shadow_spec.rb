require 'spec_helper'

if Etc2.has_shadow?
  
describe Etc2::Shadow do
	
	context 'finding Shadow entries' do
    it 'should find root from the username' do
      Etc2::Shadow.find('root').name.should == 'root'
    end
    
    it "should raise an argument error if the user can't be found" do
      user = 'fake'
      lambda{ Etc2::Shadow.find(user) }.should raise_error(ArgumentError, "Shadow entry not found for username: #{user}")
    end
  end

  context 'information correctness' do
    # let(:etc_user) { Etc.getpwnam('root') }
    let(:etc2_user){ Etc2::Shadow.find('root')  }
  end
end

end
