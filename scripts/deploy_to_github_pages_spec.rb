
require File.join(File.dirname(__FILE__), "deploy_to_github_pages.rb")

describe "travis github deployer" do
  
  subject { TravisGithubDeployer.new}
    
  before(:each) do
    @git = mock
    GitCommandLine.should_receive(:new).and_return(@git)
  end
  
  it "can deploy to an destination repository" do
    subject.should_receive(:load_configuration)
    subject.should_receive(:clone_destination_repository)
    subject.should_receive(:change_current_directory_to_cloned_repository)
    subject.should_receive(:prepare_credentials_based_on_environment_variables)
    subject.should_receive(:copy_files_in_destination_repository)
    subject.should_receive(:commit_and_push_files)
    subject.deploy
  end
  
  context "Prepare repository for being able to commit" do
    
    it "can clone the destination repository" do
      subject.should_receive(:destination_repository).and_return("https://github.com/cpputest/cpputest")
      subject.should_receive(:destination_repository_dir).and_return("destdir")
      @git.should_receive(:clone).with("https://github.com/cpputest/cpputest", "destdir")
      
      subject.clone_destination_repository
    end
    
    it "can change the directory to the cloned directory" do
      subject.should_receive(:destination_repository_dir).and_return("destinationdir")
      Dir.should_receive(:chdir).with("destinationdir")
      subject.change_current_directory_to_cloned_repository
    end
        
    it "Should be able to set the credentials for pushing stuff up" do
      subject.should_receive(:set_username_based_on_environment_variable)
      subject.should_receive(:set_email_based_on_environment_variable)
      subject.should_receive(:set_repository_token_based_on_enviroment_variable)
      subject.prepare_credentials_based_on_environment_variables
    end
    
    it "Should be able to set the username based on an environment variable" do
      ENV['GIT_NAME'] = "basvodde"
      @git.should_receive(:config_username).with("basvodde")
      subject.set_username_based_on_environment_variable    
    end
      
    it "Should give an error message when the GIT_NAME isn't set" do
      ENV['GIT_NAME'] = nil
      expect {subject.set_username_based_on_environment_variable}.to raise_error(StandardError, "The GIT_NAME environment variable wasn't set.")
    end
      
    it "Should be able to set the password based on an environment variable" do
      ENV['GIT_EMAIL'] = "basv@bestcompanythatexists.com"
      @git.should_receive(:config_email).with("basv@bestcompanythatexists.com")
      subject.set_email_based_on_environment_variable
    end
      
    it "Should be able to write the github token based on an environment variable" do
      credential_file = mock
      ENV['GIT_TOKEN'] = "Token"
    
      @git.should_receive(:config_credential_helper_store_file).with(".git/travis_deploy_credentials")
      File.should_receive(:open).with(".git/travis_deploy_credentials", "w").and_yield(credential_file)
      credential_file.should_receive(:write).with("https://Token:@github.com")
    
      subject.set_repository_token_based_on_enviroment_variable
    end  
  end

  context "Prepare the changes that need to be made commit" do
    
    it "should be able to copy a file from the root of the source repository to the root of the destination reportistory" do
      subject.should_receive(:files_to_deploy).and_return( { "sourcefile" => ""})
      FileUtils.should_receive(:copy).with(Pathname.new("sourcefile"), Pathname.new("github_pages/sourcefile"))
      subject.copy_files_to_deployment_repository
    end
    
    it "Should be able to copy multiple files" do
      subject.should_receive(:files_to_deploy).and_return({ "dir/onefile" => "destonefile", "twofile" => "dir/desttwofile"})
      FileUtils.should_receive(:copy).with(Pathname.new("dir/onefile"), Pathname.new("github_pages/destonefile"))
      FileUtils.should_receive(:copy).with(Pathname.new("twofile"), Pathname.new("github_pages/dir/desttwofile"))
      subject.copy_files_to_deployment_repository      
    end
  end
  
  it "can read configuration parameters out of the .travis_github_deployer.yml" do
    configuration = { 
      "destination_repository" => "https://github.com/cpputest/cpputest.github.io.git",
      "files_to_deploy" => {
        "source_dir/source_file" => "destination_dir/destination_file"
      }
    }
    
    YAML.should_receive(:load_file).with(".travis_github_deployer.yml").and_return(configuration)
    subject.load_configuration
    
    subject.destination_repository.should== "https://github.com/cpputest/cpputest.github.io.git"
    subject.files_to_deploy.should== { "source_dir/source_file" => "destination_dir/destination_file" }
    
  end
    
end
