## 1. ubuntu install docker
```
apt install docker.io
systemctl start docker
systemctl enable docker

install docker-ce
首先保证能够访问Docker仓库地址：https://download.docker.com/linux/ubuntu
apt update
apt install apt-transport-https ca-certificates curl software-properties-common

在/etc/apt/sources.list.d/docker.list文件中添加下面内容
deb [arch=amd64] https://download.docker.com/linux/ubuntu bionic stable

添加秘钥
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

安装docker-ce
apt install docker-ce

 ```
