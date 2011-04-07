class Sync < Thor
  include Thor::Actions
  
  desc 'rhel5', 'Sync directory to rhel5.brianfaga.com'
  def rhel5
    rsync('root@rhel5.brianfaga.com')
  end
  
  desc 'rhel6', 'Sync directory to rhel6.brianfaga.com'
  def rhel6
    rsync('root@rhel6.brianfaga.com')
  end
  
  desc 'bfaga', 'Sync directory to bfaga.tech.ais.msu.edu'
  def bfaga
    rsync('bfaga@bfaga.tech.ais.msu.edu')
  end

private
  def rsync(dest)
    res = run "rsync -avH --progress ./ #{dest}:etc2/ --exclude=Makefile --exclude=*.bundle --exclude=*.o --exclude=.git 2>&1", :capture => true
    if $?.exitstatus == 0
      say_status 'sync', 'Successful', :green
    else
      say_status 'sync', 'Failed', :red
      puts res
    end
  end
end

class Rhel < Thor::Group
  desc 'Sync to all rhel servers'
  
  invoke :'sync:rhel5'
  invoke :'sync:rhel6'
end

class All < Thor::Group
  desc 'Sync to all servers'
  
  invoke :'sync:rhel5'
  invoke :'sync:rhel6'
  invoke :'sync:bfaga'
end