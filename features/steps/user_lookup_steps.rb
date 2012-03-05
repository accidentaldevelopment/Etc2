Given /^a user with username (\w+)$/ do |username|
  @lookup = username
end

Given /^there is no user named (\w+)$/ do |username|
  @lookup = username
end

Given /^a user with uid (\d+)$/ do |uid|
  @lookup = uid.to_i
end

When /^I call ([\w!]+)$/ do |method|
  @result = Etc2::User.send(method.intern, @lookup)
end

When /^I call Etc2::User\.find$/ do
  @result = Etc2::User.find(@lookup)
end

Then /^I should get back an object with name (\w+)$/ do |username|
  @result.name.should == @lookup
end

Then /^I should get back an object with uid (\d+)$/ do |uid|
  @result.uid.should == @lookup
end

Then /^Etc2::User\.find should return nil$/ do
  Etc2::User.find(@lookup).should be_nil
end

Then /^Etc2::User\.find! should raise an exception$/ do
  expect{ Etc2::User.find!(@lookup) }.to raise_exception ArgumentError, 'User not found'
end