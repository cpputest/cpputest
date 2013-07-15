

class GitCommandLine
  
  def clone(repository, destination)
    git("clone " + repository + " " + destination)
  end
  
  def config(key, value)
    git("config #{key} '#{value}'")
  end
  
  def config_username(username)
    config("user.name", username)
  end
  
  def config_email(email)
    config("user.email", email)
  end
  
  def config_credential_helper_store_file(filename)
    config("credential.helper", "store --file=#{filename}")
  end
  
  def git(command)
    output = `git #{command} 2>&1`
    raise StandardError, "Git command: '#{command}' failed. Message: : " + output unless $?.success?
    output
  end
  
end