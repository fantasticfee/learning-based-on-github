```
set -e
mkdir -p gocode/src
echo "GOPATH=~/gocode" >> /etc/profile
echo "GOROOT=~/go" >> /etc/profile
echo "PATH=$PATH:\$GOPATH/bin:\$GOROOT/bin" >> /etc/profile
source /etc/profile

yum install -y wget git
yum install -y gcc vim
wget https://github.com/golang/go/archive/go1.4.tar.gz

tar xvf go1.4.tar.gz
mv go-go1.4 go1.4
cd go1.4/src
bash -x all.bash

cd ~
wget https://github.com/golang/go/archive/go1.12.4.tar.gz
tar xvf go1.12.4.tar.gz
mv go-go1.12.4 go 
cd go/src/
bash -x all.bash

#install docker
yum install -y yum-utils device-mapper-persistent-data lvm2 
yum-config-manager --add-repo https://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo 
yum -y install docker-ce
systemctl  start docker

#install docker-compose
curl -L https://github.com/docker/compose/releases/download/1.18.0/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose
chmod a+x /usr/local/bin/docker-compose

#prepare for run 
ssh-keygen -t rsa
mkdir -p /date/etcd/
mkdir -p /date/ezk8s/conf
mkdir -p /data/mysql

```
