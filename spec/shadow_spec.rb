require 'spec_helper'

if shadow?
describe Etc2::Shadow do
  context 'as root', :if => root? do
    # root specs
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
      let(:etc2_shadow){ Etc2::Shadow.find('root')  }
  
      it 'should have the correct usrename' do
        etc2_shadow.name.should == 'root'
      end
  
      it 'should have an encrypted password' do
        correct_shadow = `getent shadow root`.split(':')[1].strip
        etc2_shadow.passwd.should == correct_shadow
        etc2_shadow.password.should == etc2_shadow.passwd
        etc2_shadow.encrypted_password.should == etc2_shadow.passwd
      end
    end
    
    context 'getspent and endspent' do
      after(:each){ Etc2::Shadow.endspent }

      it 'should return a valid Group object' do
        Etc2::Shadow.getspent.class.should == Etc2::Shadow
      end

      it 'should respond to endgrent' do
        lambda{ Etc2::Shadow.endspent }.should_not raise_error
        Etc2::Shadow.endspent.should == nil
      end

      it 'setgrent should reset grent' do
        shadow = Etc2::Shadow.getspent
        Etc2::Shadow.setspent
        shadow.should == Etc2::Shadow.getspent
      end
    end
    
    context 'relationships' do
      before(:all){ @shadow = Etc2::Shadow.find('root') }
      it 'should respond to #user' do
        @shadow.should respond_to :user
      end
      
      it 'should return a User for root' do
        @shadow.user.should == Etc2::User.find('root')
      end
    end
  end
  
  context 'as non-root', :unless => root? do
    context 'Using Shadow as non-root' do
      it 'should raise a permissions error' do
        lambda{ Etc2::Shadow.find('root') }.should raise_error(Errno::EACCES, 'Permission denied - Must be root to access shadow database')
      end
    end
  end
end
end