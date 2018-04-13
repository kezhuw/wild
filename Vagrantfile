Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/bionic64"

  config.vm.synced_folder "saltstack/salt/", "/srv/salt/"

  config.vm.provision :salt do |salt|
    salt.minion_config = "saltstack/minion"
    salt.run_highstate = true
  end
end
