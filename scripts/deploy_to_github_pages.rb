#!/usr/bin/env ruby -w

class GithubPagesDeployerForCppUTest
  
  def self.push_artifacts
    deployer = GithubPagesDeployerForCppUTest.new
    deployer.clone_cpputest_pages
    deployer.set_credentials
  end

  def clone_cpputest_pages
    system("git clone https://github.com/cpputest/cpputest.github.io.git github_pages")
  end
  
  def set_credentials
    username = ENV['GIT_NAME']
    raise StandardError.new("The GIT_NAME environment variable wasn't set.") if username.empty?
    system("git config user.name '#{username}'")

    
  end
  
end


if __FILE__ == $0 then
  GithubPagesDeployerForCppUTest.push_artifacts
end