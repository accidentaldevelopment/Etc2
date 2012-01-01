require 'spec_helper'

describe 'etc2/replace' do
  after(:all) do
    orig_verbose = $VERBOSE
    $VERBOSE = nil
    Etc = BuiltinEtc
    $VERBOSE = orig_verbose
  end
  
  it 'replaces the builtin Etc' do
    require 'etc2/replace'
    Etc.should == Etc2
  end
  
  it 'assigns builtin Etc to BuiltinEtc' do
    orig_verbose = $VERBOSE
    $VERBOSE = nil
    Etc = BuiltinEtc
    $VERBOSE = orig_verbose
    Etc.should == Etc
  end
end