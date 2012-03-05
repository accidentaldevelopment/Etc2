@shadow
Feature: Searching for a Shadow entry

  @root
  Scenario Outline: Looking up a user by username as root
    Given a user with username <username>
    When I call Etc2::Shadow.<method>
    Then I should get back an object with name <username>
    
    Examples:
      | username | method |
      | root     | find   |
      | root     | find!  |
    
  @root
  Scenario: Using ::find to lookup a nonexistent user as root
    Given there is no user named imfake
    Then Etc2::Shadow.find should return nil
  
  @root
  Scenario: Using ::find! to lookup a nonexistent user as root
    Given there is no user named imfake
    Then Etc2::Shadow.find! should raise an exception
  
  @nonroot
  Scenario: Usage ::find to lookup anything as non-root
    Given I am not root
    Then Etc2::Shadow.find should raise an exception