#!/usr/bin/env bash

apt-get update
apt-get install -y salt-common salt-minion
mv /etc/salt/minion /etc/salt/minion.origin
cp /vagrant/saltstack/minion /etc/salt/minion
salt-call state.highstate
