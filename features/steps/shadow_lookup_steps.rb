Given /^I am not root$/ do
end

When /^I call Etc2::Shadow\.([\w!]+)$/ do |method|
  @result = Etc2::Shadow.send(method.intern, @lookup)
end

Then /^Etc2::Shadow\.find should return nil$/ do
  @result.should be_nil
end

Then /^Etc2::Shadow\.find! should raise an exception$/ do
  expect{ Etc2::Shadow.find!(@lookup) }.to raise_exception ArgumentError
end

Then /^Etc2::Shadow\.find should raise an exception$/ do
  expect{ Etc2::Shadow.find('root') }.to raise_exception Errno::EACCES
end