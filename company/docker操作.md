## 1. ubuntu install docker
```
apt install docker.io
systemctl start docker
systemctl enable docker

install docker-ce
ubuntu:
apt update
apt-get install apt-transport-https ca-certificates curl software-properties-common -y
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
apt-get update
apt-get install docker-ce -y

centos:
yum remove docker \
                  docker-common \
                  docker-selinux \
                  docker-engine
yum install -y yum-utils
yum-config-manager \
    --add-repo \
    https://download.docker.com/linux/centos/docker-ce.repo
yum install docker-ce -y
systemctl start docker
systemctl enable docker

//配置docker镜像加速，log限制大小
cat << EOF > /etc/docker/daemon.json
{
  "log-driver": "json-file",
  "log-opts": {
    "max-size": "200m"
  },
  "registry-mirrors": ["https://zrduw2by.mirror.aliyuncs.com"]
}
EOF
systemctl reload docker
 ```
