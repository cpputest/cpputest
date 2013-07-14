
require File.join(File.dirname(__FILE__), "deploy_to_github_pages.rb")

describe "configuring the git environment for deploying to github pages" do
  
  subject { GithubPagesDeployerForCppUTest.new }
  
  it "it should be able to clone the CppUTest github pages" do
    subject.should_receive(:do_system).with("git clone https://github.com/cpputest/cpputest.github.io.git github_pages")
    Dir.should_receive(:chdir).with("github_pages")
    subject.clone_cpputest_pages
  end

  it "Should be able to set the credentials for pushing stuff up" do
    subject.should_receive(:set_username_based_on_environment_variable)
    subject.should_receive(:set_email_based_on_environment_variable)
    subject.should_receive(:set_repository_token_based_on_enviroment_variable)
    subject.prepare_credentials_based_on_environment_variables
  end
  
  it "Should be able to set the username based on an environment variable" do
    ENV['GIT_NAME'] = "basvodde"
    subject.should_receive(:do_system).with("git config user.name 'basvodde'")
    subject.set_username_based_on_environment_variable    
  end
  
  it "Should give an error message when the GIT_NAME isn't set" do
    ENV['GIT_NAME'] = nil
    expect {subject.set_username_based_on_environment_variable}.to raise_error(StandardError, "The GIT_NAME environment variable wasn't set.")
  end

  it "Should be able to set the password based on an environment variable" do
    ENV['GIT_EMAIL'] = "basv@bestcompanythatexists.com"
    subject.should_receive(:do_system).with("git config user.email 'basv@bestcompanythatexists.com'")
    subject.set_email_based_on_environment_variable
  end

  it "Should be able to write the github token based on an environment variable" do
    credential_file = mock
    ENV['GIT_TOKEN'] = "Token"
    
    subject.should_receive(:do_system).with("git config credential.helper 'store --file=.git/travis_deploy_credentials'")
    File.should_receive(:open).with(".git/travis_deploy_credentials", "w").and_yield(credential_file)
    credential_file.should_receive(:write).with("https://Token:@github.com")
    
    subject.set_repository_token_based_on_enviroment_variable
  end
  
  it "should push the right artifacts up to github" do
    subject.should_receive(:clone_cpputest_pages)
    subject.should_receive(:prepare_credentials_based_on_environment_variables)
    GithubPagesDeployerForCppUTest.should_receive(:new).and_return(subject)
    GithubPagesDeployerForCppUTest.push_artifacts
  end
  
  context "do_system" do
    it "Should be able to do a successful command" do
      GithubPagesDeployerForCppUTest.do_system('echo "hello"').should== "hello\n"
    end
    
    it "Should be able to raise an StandardError on failed commands" do
      expect {
        GithubPagesDeployerForCppUTest.do_system('doesnotexistonanysystem').should== "hello\n"
      }.to raise_error(StandardError, "Command: 'doesnotexistonanysystem' failed. Message: sh: doesnotexistonanysystem: command not found\n")
    end
    
  end
  
end