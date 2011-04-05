require 'spec_helper'

describe Etc2::User do
	let(:valid_user){ Etc2::User.from_name('root')  }

	context 'creating user from username' do
		it 'should return the same username it is supplied with' do
			valid_user.name.should == 'root'
		end

		it 'should have the correct full_name' do
		  name = (RUBY_PLATFORM =~ /darwin/) ? 'System Administrator' : 'root'
			valid_user.gecos.should == name
			# test full_name alias as well
			valid_user.full_name.should == name
		end
		
		it 'should have the correct homedir' do
		  dir = (RUBY_PLATFORM =~ /darwin/) ? '/var/root' : '/root'
		  valid_user.dir.should == dir
		  # aliases
		  valid_user.home_dir.should == dir
		  valid_user.homedir.should == dir
		  valid_user.home.should == dir
	  end

		it "should raise an argument error if the user doesn't exist" do
			user = 'fake'
			lambda{ Etc2::User.from_name(user) }.should raise_error(ArgumentError, "User not found: #{user}")
		end
	end
end
