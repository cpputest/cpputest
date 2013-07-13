
require File.join(File.dirname(__FILE__), "deploy_to_github_pages.rb")

describe "configuring the git environment for deploying to github pages" do
  
  subject { GithubPagesDeployerForCppUTest.new }
  
  it "it should be able to clone the CppUTest github pages" do
    subject.should_receive(:system).with("git clone https://github.com/cpputest/cpputest.github.io.git github_pages")
    subject.clone_cpputest_pages
  end
  
  it "Should be able to set the username, email and credentials in order to push" do
    ENV['GIT_NAME'] = "basvodde"
    subject.should_receive(:system).with("git config user.name 'basvodde'")
    subject.set_credentials    
  end
  
  it "Should give an error message when the GIT_NAME isn't set" do
    ENV['GIT_NAME'] = ""
    expect {subject.set_credentials}.to raise_error(StandardError, "The GIT_NAME environment variable wasn't set.")
  end
  
  it "should push the right artifacts up to github" do
    subject.should_receive(:clone_cpputest_pages)
    subject.should_receive(:set_credentials)
    GithubPagesDeployerForCppUTest.should_receive(:new).and_return(subject)
    GithubPagesDeployerForCppUTest.push_artifacts
  end
end