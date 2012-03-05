Feature: Searching for a group

  Scenario Outline: Looking up a user by groupname
    Given a user with groupname <groupname>
    When I call Etc2::Group.<method>
    Then I should get back an object with name <groupname>
    
    Examples:
      | groupname | method |
      | wheel     | find   |
      | wheel     | find!  |
  
  Scenario Outline: Looking up a user by gid
    Given a user with gid <gid>
    When I call Etc2::Group.<method>
    Then I should get back an object with gid <gid>
    
    Examples:
      | gid | method |
      | 0   | find   |
      | 0   | find!  |
    
  Scenario: Using ::find to lookup a nonexistent user
    Given there is no group named imfake
    Then Etc2::Group.find should return nil
  
  Scenario: Using ::find! to lookup a nonexistent group
    Given there is no group named imfake
    Then Etc2::Group.find! should raise an exception