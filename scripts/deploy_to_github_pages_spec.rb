
require File.join(File.dirname(__FILE__), "deploy_to_github_pages.rb")

describe "configuring the git environment for deploying to github pages" do
  
  subject { GithubPagesDeployerForCppUTest.new }
  
  it "it should be able to clone the CppUTest github pages" do
    subject.should_receive(:system).with("git clone https://github.com/cpputest/cpputest.github.io.git github_pages")
    subject.clone_cpputest_pages
  end
  
  it "Should be able to set the username, email and credentials in order to push" do
    subject.should_receive(:system).with("git config user.name '#{ENV['GIT_NAME']}'")
    subject.set_credentials    
  end
  
  it "should push the right artifacts up to github" do
    subject.should_receive(:clone_cpputest_pages)
    GithubPagesDeployerForCppUTest.should_receive(:new).and_return(subject)
    GithubPagesDeployerForCppUTest.push_artifacts
  end
end