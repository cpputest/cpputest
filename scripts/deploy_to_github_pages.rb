#!/usr/bin/env ruby -w

class GithubPagesDeployerForCppUTest
  
  def self.push_artifacts
    deployer = GithubPagesDeployerForCppUTest.new
    deployer.clone_cpputest_pages
    deployer.prepare_credentials_based_on_environment_variables
    system("echo " " >> README.md")
    system("git add README.md")
    system('git commit README.md -m "Commit from Travis CI"')
    system("git push")
  end

  def clone_cpputest_pages
    system("git clone https://github.com/cpputest/cpputest.github.io.git github_pages")
    Dir.chdir("github_pages")
  end
  
  def prepare_credentials_based_on_environment_variables
    set_username_based_on_environment_variable
    set_email_based_on_environment_variable
    set_repository_token_based_on_enviroment_variable
  end
  
  def set_repository_token_based_on_enviroment_variable
    git_token = environment_variable_value("GIT_TOKEN")
    system("git config credential.helper 'store --file=.git/travis_deploy_credentials'")
    File.open(".git/travis_deploy_credentials", "w") { |credential_file|
      credential_file.write("https://#{git_token}:@github.com")
    }
  end
  
  def set_username_based_on_environment_variable
    git_config_based_on_enviroment_variable("user.name", "GIT_NAME")
  end
  
  def set_email_based_on_environment_variable
    git_config_based_on_enviroment_variable("user.email", "GIT_EMAIL")
  end
  
  def git_config_based_on_enviroment_variable(config_parameter, environment_variable)
    config_value = environment_variable_value(environment_variable)
    system("git config #{config_parameter} '#{config_value}'")
  end
  
  def environment_variable_value (environment_variable_name)
    value = ENV[environment_variable_name]
    raise StandardError.new("The #{environment_variable_name} environment variable wasn't set.") if value.nil?
    value
  end
end


if __FILE__ == $0 then
  GithubPagesDeployerForCppUTest.push_artifacts
end