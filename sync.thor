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
  
  desc 'rhel', 'Sync to all rhel systems'
  def rhel
    invoke :rhel5
    invoke :rhel6
  end
  
  desc 'all', 'Sync to all known systems'
  def all
    invoke :bfaga
    invoke :rhel
  end

private
  def rsync(dest)
    res = run "rsync -avH --delete --progress ./ #{dest}:etc2/ --exclude=tmp --exclude=.git 2>&1", :capture => true
    if $?.exitstatus == 0
      say_status 'sync', 'Successful', :green
    else
      say_status 'sync', 'Failed', :red
      puts res
    end
  end
end