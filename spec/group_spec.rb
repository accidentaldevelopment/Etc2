require 'spec_helper'

describe Etc2::Group do
	
	context 'finding groups' do
    it 'should find root from the groupname' do
      Etc2::Group.find('wheel').name.should == 'wheel'
    end
    
    it 'should find root from the gid' do
      Etc2::Group.find(0).name.should == Etc.getgrgid(0).name
    end
    
    it "should raise an argument error if the group can't be found" do
      group = 'fake'
      lambda{ Etc2::Group.find(group) }.should raise_error(ArgumentError, "Group not found with groupname: #{group}")
      gid = -11
      lambda{ Etc2::Group.find(gid) }.should raise_error(ArgumentError, "Group not found with gid: #{gid}")
    end
  end

  context 'getgrent and endgrent' do
    after(:each){ Etc2::Group.endgrent }
    
    it 'should return a valid Group object' do
      Etc2::Group.getgrent.class.should == Etc2::Group
    end
    
    it 'should respond to endgrent' do
      lambda{ Etc2::Group.endgrent }.should_not raise_error
      Etc2::Group.endgrent.should == nil
    end
    
    it 'setgrent should reset grent' do
      group = Etc2::Group.getgrent
      Etc2::Group.setgrent
      group.should == Etc2::Group.getgrent
    end
  end

  context 'information correctness' do
    let(:etc_group) { Etc.getgrnam('wheel') }
    let(:etc2_group){ Etc2::Group.find('wheel')  }
    
    it 'should have the correct name' do
      etc2_group.name.should == etc_group.name
    end
    
    it 'should have the right gid' do
      etc2_group.gid.should == etc_group.gid
    end
    
    it 'should have the right members list' do
      etc2_group.mem.should == etc_group.mem
    end
  end
end
