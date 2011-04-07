require 'spec_helper'

if Etc2.has_shadow?
  describe Etc2::Shadow do
    if Process.uid == 0
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
    
    else
      # Non-root specs
      context 'Using Shadow as non-root' do
        it 'should raise a permissions error' do
          lambda{ Etc2::Shadow.find('root') }.should raise_error(Errno::EACCES, 'Permission denied - Must be root to access shadow database')
        end
      end
    end
  end
end
