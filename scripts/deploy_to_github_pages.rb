#!/usr/bin/env ruby -w

class GithubPagesDeployerForCppUTest
  
  def self.push_artifacts
    deployer = GithubPagesDeployerForCppUTest.new
    deployer.clone_cpputest_pages
  end

  def clone_cpputest_pages
    system("git clone https://github.com/cpputest/cpputest.github.io.git github_pages")
  end
  
  def set_credentials
    system("git config user.name '#{ENV['GIT_NAME']}'")
  end
  
end


if __FILE__ == $0 then
  GithubPagesDeployerForCppUTest.push_artifacts
end