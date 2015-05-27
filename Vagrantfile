Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/trusty64"
  config.vm.provision "shell", inline: %Q{
    sudo apt-get -y update
    sudo apt-get -y install valgrind
  }
end
