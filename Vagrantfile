Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/wily64"

  config.vm.synced_folder "saltstack/salt/", "/srv/salt/"
  config.vm.provision "shell", path: "saltstack/bootstrap.sh"
  # Salt provisioner fails to upload minion config to
  # privileged directory in version v1.7.3, v1.7.4, SEE:
  #
  # https://github.com/mitchellh/vagrant/issues/5973
  # https://github.com/mitchellh/vagrant/pull/6073
  #
  # config.vm.provision :salt do |salt|
  #   salt.minion_config = "saltstack/minion"
  #   salt.run_highstate = true
  # end
end
