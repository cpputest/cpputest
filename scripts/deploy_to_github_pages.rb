#!/usr/bin/env ruby -w

require File.join(File.dirname(__FILE__), "git_command_line.rb")
require 'YAML'

class TravisGithubDeployer
  
  def initialize
    @git = GitCommandLine.new
  end
  
  ## Configuration values
  
  def destination_repository
    @destination_repository
  end
  
  def destination_repository_dir
    @destination_repository_dir
  end
  
  def files_to_deploy
    @files_to_deploy
  end
    
  ## Deployment 
    
  def deploy
    load_configuration
    clone_destination_repository
    change_current_directory_to_cloned_repository
    prepare_credentials_based_on_environment_variables
    copy_files_in_destination_repository
    commit_and_push_files
  end

  ## Preparing for deployment

  def load_configuration
    configuration = YAML.load_file(".travis_github_deployer.yml")
    @destination_repository = configuration["destination_repository"]
    @files_to_deploy = configuration["files_to_deploy"]
  end

  def clone_destination_repository
    @git.clone(destination_repository, destination_repository_dir)
  end
  
  def change_current_directory_to_cloned_repository
    Dir.chdir(destination_repository_dir)
  end

  def prepare_credentials_based_on_environment_variables
    set_username_based_on_environment_variable
    set_email_based_on_environment_variable
    set_repository_token_based_on_enviroment_variable
  end
  
  def set_repository_token_based_on_enviroment_variable
    git_token = environment_variable_value("GIT_TOKEN")
    @git.config_credential_helper_store_file(".git/travis_deploy_credentials")
    File.open(".git/travis_deploy_credentials", "w") { |credential_file|
      credential_file.write("https://#{git_token}:@github.com")
    }
  end
  
  def set_username_based_on_environment_variable
    @git.config_username(environment_variable_value("GIT_NAME"))
  end
  
  def set_email_based_on_environment_variable
    @git.config_email(environment_variable_value("GIT_EMAIL"))
  end
  
  def environment_variable_value (environment_variable_name)
    value = ENV[environment_variable_name]
    raise StandardError.new("The #{environment_variable_name} environment variable wasn't set.") if value.nil?
    value
  end
    
  def copy_files_to_deployment_repository
    
    files_to_deploy.each { |source_location, destination_location|
      source = Pathname.new(source_location)
      destination = Pathname.new("github_pages")
      destination += destination_location.empty? ? source_location : destination_location
      FileUtils.copy(source, destination)
    }
    
  end
  
  def commit_and_push_files
  end
    
end


if __FILE__ == $0 then
  GithubPagesDeployer.new.deploy
end
