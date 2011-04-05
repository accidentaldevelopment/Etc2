require 'spec_helper'

describe Etc2::User do
	
	context 'finding users' do
    it 'should find root from the username' do
      Etc2::User.find('root').name.should == 'root'
    end
    
    it 'should find root from the uid' do
      Etc2::User.find(0).name.should == 'root'
    end
    
    it "should raise an argument error if the user can't be found" do
      user = 'fake'
      lambda{ Etc2::User.find(user) }.should raise_error(ArgumentError, "User not found with username: #{user}")
      uid = -1
      lambda{ Etc2::User.find(uid) }.should raise_error(ArgumentError, "User not found with uid: #{uid}")
    end
  end

  context 'information correctness' do
    let(:etc_user) { Etc.getpwnam('root') }
    let(:etc2_user){ Etc2::User.find('root')  }
    
    it 'should have the correct username' do
      etc2_user.name.should == etc_user.name
    end
    
    it 'should have the right passwd'
    
    it 'should have the right uid' do
      etc2_user.uid.should == etc_user.uid
    end
    
    it 'should have the right gid' do
      etc2_user.gid.should == etc_user.gid
    end
  
    it 'should have the correct gecos' do
      etc2_user.gecos.should == etc_user.gecos
      etc2_user.full_name.should == etc2_user.gecos
    end
    
    it 'should have the correct homedir' do
      etc2_user.dir.should == etc_user.dir
      # aliases
      etc2_user.home_dir.should == etc2_user.dir
      etc2_user.homedir.should  == etc2_user.dir
      etc2_user.home.should     == etc2_user.dir
    end
    
    it 'should have the right shell' do
      etc2_user.shell.should == etc_user.shell
    end
  end
end
