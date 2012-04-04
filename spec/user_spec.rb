require 'spec_helper'

describe Etc2::User do
	
	context '#find' do
	  context 'by username' do
      it 'should find root from the username' do
        Etc2::User.find('root').name.should == 'root'
      end
      
      it 'should work if passed a symbol' do
        Etc2::User.find(:root).name.should == 'root'
      end
      
      it "should return nil if the user can't be found" do
        Etc2::User.find('fake').should == nil
      end
      
      it "should raise an ArgumentError if the user can't be found with bang method" do
        user = 'fake'
        lambda{ Etc2::User.find!(user) }.should raise_error(ArgumentError, "User not found")
      end
    end
    
    context 'by uid' do
      it 'should find root from the uid' do
        Etc2::User.find(0).name.should == 'root'
      end
    
      it "should raise an ArgumentError if the user can't be found with bang method" do
        uid = -1
        lambda{ Etc2::User.find!(uid) }.should raise_error(ArgumentError, "User not found")
      end
    end
  end
  
  context 'Enumerable' do
    it 'should respond to #each' do
      Etc2::User.should respond_to(:each)
    end
  end
  
  context 'getpwent and endpwent' do
    after(:each){ Etc2::User.endpwent }
    
    it 'should return a valid User object' do
      Etc2::User.getpwent.class.should == Etc2::User
    end
    
    it 'should respond to endpwent' do
      lambda{ Etc2::User.endpwent }.should_not raise_error
      Etc2::User.endpwent.should == nil
    end
    
    it 'setpwent should reset pwent' do
      user = Etc2::User.getpwent
      Etc2::User.setpwent
      user.should == Etc2::User.getpwent
    end
  end

  context '#current_user' do
    before(:all){ @user = Etc2::User.current }
  
    it 'should return the correct current user' do
      @user.name.should == Etc.getlogin
    end
  
    it 'should have the correct homedir' do
      @user.dir.should == Etc.getpwnam(Etc.getlogin).dir
    end
  
    it 'should have the right UID' do
      @user.uid.should == Etc.getpwnam(Etc.getlogin).uid
    end
  end

  context 'information correctness' do
    before(:all) do
      @etc_user = Etc.getpwnam('root')
      @etc2_user = Etc2::User.find('root')
    end
    
    it 'should have the correct username' do
      @etc2_user.name.should == @etc_user.name
    end
    
    it 'should not have a passwd field' do
      lambda{ @etc2_user.passwd }.should raise_error(NoMethodError)
    end
    
    it 'should have the right uid' do
      @etc2_user.uid.should == @etc_user.uid
    end
    
    it 'should have the right gid' do
      @etc2_user.gid.should == @etc_user.gid
    end
  
    it 'should have the correct gecos' do
      @etc2_user.gecos.should == @etc_user.gecos
      @etc2_user.full_name.should == @etc2_user.gecos
    end
    
    it 'should have the correct homedir' do
      @etc2_user.dir.should == @etc_user.dir
      # aliases
      @etc2_user.home_dir.should == @etc2_user.dir
      @etc2_user.homedir.should  == @etc2_user.dir
      @etc2_user.home.should     == @etc2_user.dir
    end
    
    it 'should have the right shell' do
      @etc2_user.shell.should == @etc_user.shell
    end
  end
  
  context 'relationships' do
    before(:all){ @user = Etc2::User.find(0) }
    it 'should have the associated group object' do
      @user.group.should == Etc2::Group.find(@user.gid)
    end
    
    it 'should have the associated shadow object', :if => shadow? && root? do
      @user.shadow.should == Etc2::Shadow.find(@user.name)
    end
    
    it 'should not have the shadow method', :unless => shadow? do
      @user.should_not respond_to :shadow
    end
  end
end
