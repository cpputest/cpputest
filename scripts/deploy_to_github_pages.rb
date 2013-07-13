#!/usr/bin/env ruby -w

class GithubPagesDeployerForCppUTest
  
  def self.push_artifacts
    deployer = GithubPagesDeployerForCppUTest.new
    deployer.clone_cpputest_pages
    deployer.prepare_credentials_based_on_environment_variables
  end

  def clone_cpputest_pages
    system("git clone https://github.com/cpputest/cpputest.github.io.git github_pages")
  end
  
  def prepare_credentials_based_on_environment_variables
    set_username_based_on_environment_variable
    set_email_based_on_environment_variable
    set_repository_token_based_on_enviroment_variable
  end
  
  def set_repository_token_based_on_enviroment_variable
  end
  
  def set_username_based_on_environment_variable
    git_config_based_on_enviroment_variable("user.name", "GIT_NAME")
  end
  
  def set_email_based_on_environment_variable
    git_config_based_on_enviroment_variable("user.email", "GIT_EMAIL")
  end
  
  def git_config_based_on_enviroment_variable(config_parameter, environment_variable)
    config_value = ENV[environment_variable]
    raise StandardError.new("The #{environment_variable} environment variable wasn't set.") if config_value.nil?
    system("git config #{config_parameter} '#{config_value}'")
  end
  
  
end


if __FILE__ == $0 then
  GithubPagesDeployerForCppUTest.push_artifacts
end