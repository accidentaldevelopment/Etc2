Feature: Searching for a user

  Scenario Outline: Looking up a user by username
    Given a user with username <username>
    When I call <method>
    Then I should get back an object with name <username>
    
    Examples:
      | username | method |
      | root     | find   |
      | root     | find!  |
  
  Scenario Outline: Looking up a user by uid
    Given a user with uid <uid>
    When I call <method>
    Then I should get back an object with uid <uid>
    
    Examples:
      | uid | method |
      | 0   | find   |
      | 0   | find!  |
    
  Scenario: Using ::find to lookup a nonexistent user
    Given there is no user named imfake
    Then Etc2::User.find should return nil
  
  Scenario: Using ::find! to lookup a nonexistent user
    Given there is no user named imfake
    Then Etc2::User.find! should raise an exception