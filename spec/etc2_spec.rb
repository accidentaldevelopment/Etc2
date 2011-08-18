require 'spec_helper'

describe Etc2 do
  context 'classes' do
    # Classes that always exist
    it('should include a User class') { Etc2.constants.include?(:User).should be_true }
    it('should include a Group class') { Etc2.constants.include?(:Group).should be_true }
    
    context 'libshadow on Darwin', :if => darwin? do
      it('should raise NotImplementedError when trying to access shadow') do
        lambda{ Etc2::Shadow.find('bfaga') }.should raise_error(NotImplementedError, /shadow not available on this platform/)
        lambda{ Etc2::Shadow.new }.should raise_error(NotImplementedError, /shadow not available on this platform/)
      end
      
      it('should return false for has_shadow?'){ Etc2.has_shadow?.should be_false}
    end
    
    context 'libshadow on Linux', :if => linux? do
      it('should include a Shadow class') { Etc2.constants.include?(:Shadow).should be_true }
      it('should return true for has_shadow?'){ Etc2.has_shadow?.should be_true}
    end
  end
  
  context '#crypt' do
    it 'should encrypt a string with DES' do
      txt,salt = 'this is a test', 's1'
      result   = 's1DxyEuMylla6'
      Etc2.crypt(txt,salt).should == result
    end
    
    it "should autogenerate a salt if one isn't provided" do
      lambda{ Etc2.crypt('testpw') }.should_not raise_error
    end
    
    it 'should have two different hashes even when generated in rapid succession' do
      Etc2.crypt('testpw').should_not == Etc2.crypt('testpw')
    end
    
    context 'second argument as a hash' do
      it "should raise an ArgumentError if it's not a String or Hash" do
        lambda{ Etc2.crypt('blah', 3) }.should raise_error(ArgumentError)
      end
      
      it 'should take :salt as a key' do
        txt,salt = 'this is a test', 's1'
        result   = 's1DxyEuMylla6'
        Etc2.crypt(txt, :salt => salt).should == result
      end
    end
  
    context 'with different magic numbers', :if => linux? do
      let(:text){ 'this is a test' }
      let(:salt){ 'saltystring'}
    
      it 'should do MD5 encrypting' do
        Etc2.crypt(text, '$1$' + salt + '$').should == '$1$saltystr$DDQHJy3Lz/pqQGuF57hbd.'
      end
    
      it 'should do SHA256 encrypting' do
        Etc2.crypt(text, '$5$' + salt + '$').should == '$5$saltystring$13k9Wc4IgCrmY/S1depNkWHh9eFvy62s7xFlmW8KZ2D'
      end
    
      it 'should do SHA512 encrypting' do
        Etc2.crypt(text, '$6$' + salt + '$').should == '$6$saltystring$9hcyNJduz/BgCm1tNClS/jv7Xc7VhZ1DJSZk5wXsfB/X/aLI6MSq2c3wS0e.uynaY2VvWUXU55JBVzhSTM4o70'
      end
    end
  
  end
end