Given /^a user with groupname (\w+)$/ do |groupname|
  @lookup = groupname
end

Given /^there is no group named (\w+)$/ do |groupname|
  @lookup = groupname
end

Given /^a user with gid (\d+)$/ do |gid|
  @lookup = gid.to_i
end

When /^I call Etc2::Group\.([\w!]+)$/ do |method|
  @result = Etc2::Group.send(method.intern, @lookup)
end

Then /^I should get back an object with gid (\d+)$/ do |gid|
  @result.gid.should == @lookup
end

Then /^Etc2::Group\.find should return nil$/ do
  Etc2::Group.find(@lookup).should be_nil
end

Then /^Etc2::Group\.find! should raise an exception$/ do
  expect{ Etc2::Group.find!(@lookup) }.to raise_exception ArgumentError, 'Group not found'
end