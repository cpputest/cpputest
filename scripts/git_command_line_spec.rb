
require File.join(File.dirname(__FILE__), "git_command_line.rb")


describe "simple ruby interface around git command line" do
  
  subject { GitCommandLine.new}
  
  it "can do a git clone" do
    subject.should_receive(:git).with("clone repository destination")
    subject.clone("repository", "destination")
  end
  
  it "can do a config" do
    subject.should_receive(:git).with("config key 'value'")
    subject.config("key", "value")
  end
  
  it "can configure the username" do
    subject.should_receive(:config).with("user.name", "basvodde")
    subject.config_username("basvodde")
  end
  
  it "can configure the email" do
    subject.should_receive(:config).with("user.email", "basv@sokewl.com")
    subject.config_email("basv@sokewl.com")
  end
  
  it "can configure the credential helper" do
    subject.should_receive(:config).with("credential.helper", "store --file=filename")
    subject.config_credential_helper_store_file("filename")
  end
  
  
  context "do_system" do
    it "Should be able to do a successful command" do
      subject.git('version').should start_with("git version")
    end
    
    it "Should be able to raise an StandardError on failed commands" do
      expect {
        subject.git('error')
      }.to raise_error(StandardError, "Git command: 'error' failed. Message: : git: 'error' is not a git command. See 'git --help'.\n\nDid you mean this?\n	rerere\n")
    end
  end
  
  
end