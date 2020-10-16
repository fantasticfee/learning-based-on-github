## 1、给git账户生产ssh key

       ssh-keygen -t rsa -C "youremail@example.com"
       登录github。打开setting->SSH keys，点击右上角 New SSH key，
       把生成好的公钥id_rsa.pub放进 key输入框中，再为当前的key起一个title来区分每个key。
       
       生成ssh密钥对： ssh-keygen -t rsa

## 2、linux挂载windows共享文件

       问题：[root@x home]# mount -t cifs -o username=zy,password=1qaz2WSX# \\10.93.0.247\ezk8s ./windows
       mount: wrong fs type, bad option, bad superblock on \10.93.0.247ezk8s,
              missing codepage or helper program, or other error
              (for several filesystems (e.g. nfs, cifs) you might
              need a /sbin/mount.<type> helper program)

              In some cases useful info is found in syslog - try
              dmesg | tail or so.
       解决方法：yum install cifs-utils

       重新执行mount命令，遇到问题：
         mount.cifs: bad UNC (\10.93.0.247ezk8s)
  
## 3、mysql修改密码
       问题：启动beego二进制程序，出现error：
       register db Ping `default`, Error 1044: Access denied for user 'test'@'%' to database 'ezk8s'
       解决：
       在mysql服务器上执行：
       GRANT ALL PRIVILEGES ON *.* TO 'test'@'%';

       改密码：update user set password=password("123") where user="root";
       FLUSH PRIVILEGES;(刷新权限，任何涉及权限的操作都要执行此步来生效）

## 4、修改密码错误
       问题：
       [root@linux1 /]# passwd Oracle
       Changing password for user oracle.
       New password: 
       Retype new password: 
       passwd: Authentication token manipulation error
       解决方法：
       [root@linux1 /]# chattr -i /etc/passwd
       [root@linux1 /]# chattr -i /etc/shadow

       [root@linux1 /]# passwd oracle
       Changing password for user oracle.
       New password: 
       Retype new password: 
       passwd: all authentication tokens updated successfully.

       [root@linux1 /]# chattr +i /etc/shadow
       [root@linux1 /]# chattr +i /etc/passwd

## 5、vim没有权限保存修改

       :w !sudo tee %
## 6、git上传超过100M文件

Git LFS的官方网址： https://git-lfs.github.com/
* 先安装 Git LFS 的客户端，然后在将要push的仓库里重新打开一个bash命令行： 
* 只需设置1次 LFS : git lfs install
* 跟踪你要push的大文件的文件或指定文件类型 git lfs track "*.pdf"
* 以上已经设置完毕， 其余的工作就是按照正常的 add , commit , push 流程就可以了。
 ## 7、window环境编译为linux二进制
 
 GOOS=linux GOARCH=amd64 go build

## 8、 第一次ssh连接省略yes提问

```
sshpass -p x ssh root@192.168.122.104  -o StrictHostKeyChecking=no
sshpass -p x ssh-copy-id root@192.168.122.104  -o StrictHostKeyChecking=no
```

## 9、 查看完整history

```
docker history image_name --no-trunc=true 
```

## 10、端口映射，用于被外部访问

```
添加：iptables -t nat -A PREROUTING -p tcp -m tcp --dport 2225 -j DNAT --to-destination 192.168.122.103:8080
删除：将"A"换为"D"
```

## 11、访问etcd

```
 export ETCDCTL_API=2
export ETCDCTL_API=3
以上两个语句分别将etcdctl访问的etcd版本调整为2或者3
获取etcd中存储的所有key：
etcd2：
etcdctl ls
etcd3：
etcdctl get / --prefix --keys-only
etcdctl --endpoints=https://192.168.122.111:2379 --cacert=/etc/kubernetes/ssl/etcd/ca.pem --key=/etc/kubernetes/ssl/etcd/node-node1-key.pem --cert=/etc/kubernetes/ssl/etcd/node-node1.pem get / --prefix --keys-only
```

## 12、禁用摄像头

```
sudo vi /etc/modprobe.d/blacklist.conf
##Disable webcam.
blacklist uvcvideo
```
然后重启电脑

## 13、 抓包
 
 ```
tcpdump -i eth0 src host hostname  对源为hostname的主机的通信数据包进行监视
tcpdump -i eth0 dst host hostname 监视所有送到主机hostname的数据包
```

## 14、 docker批量操作

```
docker rm `docker ps -a -q` 删除所有容器
docker rmi `docker images -q` 删除所有镜像
docker rmi `docker images | grep  "<none>" | awk '{print $3}' 删除选择的镜像
```

## 15、 node run dev 错误：vue-service-cli not found

确认为版本问题，具体要结合当前环境
```
npm i -g npm@5.0.0
npm install
npm run build
```

## 16、配置Systemd并启动etcd服务

```sudo apt install -y etcd

创建Etcd配置文件和数据目录：
sudo mkdir -p /var/lib/etcd/
sudo mkdir /etc/etcd

sudo vim /etc/systemd/system/etcd.service
[Unit]
Description=etcd key-value store
Documentation=https://github.com/etcd-io/etcd
After=network.target
[Service]
User=etcd
Type=notify
Environment=ETCD_DATA_DIR=/var/lib/etcd
Environment=ETCD_NAME=%m
ExecStart=/usr/local/bin/etcd
Restart=always
RestartSec=10s
LimitNOFILE=40000
[Install]
WantedBy=multi-user.target
重新加载systemd服务并在Ubuntu 18.04/Ubuntu 16.04系统上启动etcd：
sudo systemctl  daemon-reload
sudo systemctl  start etcd.service
```

## 17、ssh连接太慢

```
vi /etc/ssh/sshd_config   
查找GSSAPIAuthentication 赋值为no
查找UseDNS,赋值为 no(该项默认不启用的，要把前面的#删除掉)
重启systemctl restart sshd
```

## 18、npm的常见问题，npm和nodejs版本不匹配
```
remove old npm version and install new version npm
删除当前环境所有npm版本：
sudo apt --auto-remove purge npm
sudo apt --auto-remove purge nodejs
执行npm 和node命令再次确认，如果相应路径还是存在，手动删除：
sudo rm -rf /usr/local/bin/npm /usr/local/share/man/man1/node* /usr/local/lib/dtrace/node.d ~/.npm ~/.node-gyp /opt/local/bin/node opt/local/include/node /opt/local/lib/node_modules      
sudo rm -rf /usr/local/lib/node*  
sudo rm -rf /usr/local/include/node*                                                                         
sudo rm -rf /usr/local/bin/node*   

ubuntu安装新npm：
curl -sL https://deb.nodesource.com/setup_7.x | sudo -E bash -
sudo apt-get install -y nodejs
node install 命令执行的前排信息会显示与nodejs是否匹配

centos安装npm：
curl -sL https://rpm.nodesource.com/setup_6.x  |  bash  - 
yum install -y nodejs
npm install npm@latest -g
rm -rf node_modules/
npm install
npm run dev
npm run build
```

## 19、kubernetes命令
```
kubectl delete pod foo --grace-period=0 --force
```

## 20、命令执行创建deploy和service
```
 kubectl run nginx-deploy --image=nginx:1.14-alpine --port=80 --replicas=1    #创建一个nginx的应用，副本数为1
 kubectl expose deployment nginx-deploy --name=nginx --port=80 --target-port=80 --protocol=TCP　　#创建一个nginx的service
```

## 21、下载golang tools包
```
 go get github.com/golang/tools
```

## 22、kvm虚拟机扩容
```
qemu-img resize centos12.qcow2 +50G #增加硬盘大小
virsh edit vm-name  #查找cpu和mem修改成对应值
virsh  setvcpus  ansiblek8s12 8  #设置cpu核数，这是虚拟的
virsh setmem ansiblek8s12 4096M  #配置内存
```

## 23、ipables日志和链表操作
```
iptables -A INPUT  -j LOG --log-prefix "iptables"
dmesg | grep iptables
iptables -t nat -I OUTPUT 2 -j chain_name #在output链表的第二条增加链条引用
iptables -t nat -D OUTPUT 1  #删除链条OUTPUT的第一个引用
```

## 24、 git因仓库太大导致的下载问题

现象： error: RPC failed; curl 18 transfer closed with outstanding read data remaining
    fatal: The remote end hung up unexpectedly
    fatal: early EOF
    fatal: index-pack failed
 解决：git config --global http.postBuffer 524288000
 下载分支：git fetch origin release-1.13:release-1.13 --depth 1
 或者git clone -b v0.99 --depth 1 https://github.com/git/git.git

## 25、 安装nodejs
```
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh | bash
#终端会有提示说你可以重启终端或者执行以下语句使 nvm 可用，我尝试过重启，nvm 并没有配置好，所以还是需要将给出的提示命令全部复制执行一下
nvm --version  #证明安装成功
nvm install node
rm -rf node_modules #若出错，删除老的文件夹
npm install  
npm run dev #运行在本地环境
npm run build #用于生产环境
```

## 26、 查找最大目录
```
du -am | sort -nr | head -n 10
```

## 27、 更新mysql
```
 update host SET status=9 where id=39;
```
## 28、 helm本地仓库搭建（前提，在kubernetes集群中已安装helm）
```
helm repo default path: /root/.helm/repository/local
helm creat mychart     #创建默认nginx应用
helm serve --address 172.31.23.109:8879 --repo-path /root/.helm/repository/local  #启动server端
```

## 29、 win10 关闭Hyper-V
```
1、以管理员的身份运行“CMD”；

2、输入命令“bcdedit /copy {current} /d “Windows10 no Hyper-V”；

3、输入命令“bcdedit /set {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} hypervisorlaunchtype OFF”，然后重启电脑。
将第2步运行后的命令出现在{}里的序列号替换第3步{}里的“XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX”。

```

## 30、virtualbox共享文件配置
```
1、配置共享文件为固定分配，自动挂载，注意共享名与要被挂载的目录不要同名，否则，后续mount会出错
2、mount -t vboxsf share /home/zy/gocode/src
```
## 31、linux shdowsocks客户端配置及privoxy安装
```
sudo apt install shadowsocks
sslocal -s 11.22.33.44 -p 25003 -k 123456 -l 1080 -t 600 -m aes-256-cfb
sudo apt install privoxy
vim /etc/privoxy/config
 forward-socks5t   /               127.0.0.1:1080 .
 listen-address  127.0.0.1:1081
修改完配置启动 privoxy
```

## 31、ubuntu 修改网卡名称为eth0
```
vi /etc/default/grub
在GRUB_CMDLINE_LINUX=后面添加"net.ifnames=0 biosdevname=0"
grub-mkconfig -o /boot/grub/grub.cfg
vi /etc/network/interfaces
将对应网卡修改为eth0
```

## 32、安装ping命令
```
apt-get install iputils-ping
```

## 33、go get因网络原因下载不了go相关包,使用gom

```
go get github.com/gpmgo/gopm
gopm get -g -v golang.org/x/tools/cmd/goimports
go install golang.org/x/tools/cmd/goimports
```

## 34、git撤销第一次commit
```
git update-ref -d HEAD
```
## 35、批量删除Evicted pod
```
kubectl get pods -n p-dkgkj-pipeline | grep Evicted | awk '{print $1}' | xargs kubectl delete pod -n p-dkgkj-pipeline
```
## 36、校对时间
```
临时设置：
date -s "2019-9-26 10:14:00"
永久配置：
ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
 ntpdate cn.pool.ntp.org
 hwclock -w 然后重启，否则后续可能还会出问题
```

## 37、helm 使用阿里云仓库初始化
```
helm init --upgrade -i registry.cn-hangzhou.aliyuncs.com/google_containers/tiller:v2.9.0 --stable-repo-url https://kubernetes.oss-cn-hangzhou.aliyuncs.com/charts
```

## 38、helm 包在创建其他resource之前先创建crd
```
annotations:
    "helm.sh/hook": crd-install
```

## 39、docker无权限删除容器，查看日志： apparmor=“DENIED” operation=“signal” profile=“docker-default” pid=20728 comm=“docker-containe” requested_mask=“receive” denied_mask=“receive” signal=kill peer=“unconfined”
解决方法：
```
systemctl disable apparmor.service --now
Unload AppArmor profiles: sudo service apparmor teardown
```
## 40、查找docker日志
```
docker inspect --format='{{.LogPath}}' containername
```

## 41、证书pem转crt，key
```
 openssl rsa -in privkey.pem -out privkey.key  
 openssl x509 -in fullchain.pem -out fullchain.crt 
```
## 42、ubuntu public key error
```
现象：
W: GPG error: http://archive.ubuntukylin.com trusty Release: The following signatures couldn't be verified because the public key is not available: NO_PUBKEY D259B7555E1D3C58
解决方法：
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys D259B7555E1D3C58 
```

## 43、ununtu install doker-ce
```
apt-get update
apt-get install apt-transport-https ca-certificates curl software-properties-common -y 
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
apt-get update
apt-get install docker-ce -y
```

## 44、chrome https不能访问，需要clear chrome hsts
```
chrome://net-internals/#hsts
Query HSTS/PKP domain中输入域名，可以看到对应的信息
需要在Delete domain security policies中输入域名，清除域名信息
```

## 45、设置时区及ntp时间同步
```
apt install -y ntp 或 yum install -y ntp  //安装ntp
ntp server：
修改ntp.conf:    
添加 server 120.24.166.46   //aliyun ntp时间
server 127.127.1.0   //本地时间

ntp client:
修改ntp.conf:
添加 server ntp-server_ip

on ntp server or client all need to restart service：
systemctl restart ntp  #ubuntu重启ntp服务
systemctl enable ntpd
systemctl restart ntpd //centos重启ntp服务

//设置时区
date -R   //查看当前系统时区
timedatectl status  //查看系统时间方面的各种状态
timedatectl list-timezones # 列出所有时区
timedatectl set-local-rtc 1 # 将硬件时钟调整为与本地时钟一致, 0 为设置为 UTC 时间
timedatectl set-timezone Asia/Shanghai # 设置系统时区为上海
cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
```

## 46. 配置deployment command死循环
```
 containers:
      - command:
        - /bin/sh
        - -c
        - while true;do curl http://java:8081/harvestingResources/getIpRange;sleep
          6000;done
```

## 47. 配置go mod
```
export GOPROXY=https://goproxy.cn
```

## 48、做删除namespace时，kuberntes get all -n namespace_name 会看不到部分资源
```
无法显示rolebinging,secret资源，必须手动删除，才能再去删除namespace,
如果还是不行，只能etcdctl del 去删除相应的namespace及相关资源；
不过需要注意的是，确认真正删除的某个应用，只是确认其安装的namespace完全删除，可能
只对部分应用有用，比如采用rke去ha部署rancher，即使helm delete删除了应用，以及删除了cubepaas-namespace,
重新使用helm安装，还是会发现以前的user数据（可能不止user），因此还需要查找本不属于kubernetes原生的namespace,
将其他非原生的namespace完全删除
```

## 49、iptables基本操作
```
仅允许某些IP访问指定端口：
先禁用端口访问：iptables -I INPUT -p tcp --dport 8000 -j DROP
允许特定IP访问：iptables -I INPUT -s IP地址/32 -p tcp --dport 8000 -j ACCEPT
备份iptabes rules: iptables-save >/dd/iptables.bak  
恢复iptables rules: iptables-restore </dd/iptables.bak
iptables -A 添加的规则是添加在最后面。如针对INPUT链增加一条规则，接收从eth0口进入且源地址为192.168.0.0/16网段发往本机的数据
iptables -A INPUT -i eth0 -s 192.168.0.0/16 -j ACCEPT  
iptables -I 添加的规则默认添加至第一条
```

## 50、 ubuntu iptables没有设置systemctl启动
```
需要修改iptabels 表，并且重启电脑生效，但找不到对应路径，
最终找到在/etc/iptables/rules.v4文件里面,iptables-save > /etc/iptables/rules.v4保存当前配置，
然后重启电脑生效
```

## 51、centos7 内核3.10升级到4.4
```
#下载内核源
rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-3.el7.elrepo.noarch.rpm

#安装最新版本内核
yum --enablerepo=elrepo-kernel install -y kernel-lt

#查看可用内核
cat /boot/grub2/grub.cfg |grep menuentry

#设置开机从新内核启动
grub2-set-default "CentOS Linux (4.4.224-1.el7.elrepo.x86_64) 7 (Core)"

#查看内核启动项
grub2-editenv list

#重启系统使内核生效
reboot

查看内核版本是否生效
uname -r
```

## 52. ubuntu kubeadm安装指定版本k8s(转载链接https://zhuanlan.zhihu.com/p/111336681）
```
    更新源，设置国内下载镜像

apt-get update && apt-get install -y apt-transport-https
curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | apt-key add - 
cat <<EOF >/etc/apt/sources.list.d/kubernetes.list
deb https://mirrors.aliyun.com/kubernetes/apt/ kubernetes-xenial main
EOF  
apt-get update

指定版本安装

## 找到可用的版本
apt-cache madison kubeadm

## 指定版本
K_VER="1.17.3-00"
apt-get install -y kubelet=${K_VER}
apt-get install -y kubectl=${K_VER}
apt-get install -y kubeadm=${K_VER}

下载镜像脚本
#!/bin/bash

images=(k8s.gcr.io/kube-apiserver:v1.17.3
        k8s.gcr.io/kube-controller-manager:v1.17.3
        k8s.gcr.io/kube-scheduler:v1.17.3
        k8s.gcr.io/kube-proxy:v1.17.3
        k8s.gcr.io/pause:3.1
        k8s.gcr.io/etcd:3.4.3-0
        k8s.gcr.io/coredns:1.6.5)

for var in ${images[@]};do
        image=${var/k8s.gcr.io\//gcr.azk8s.cn\/google-containers/}
        docker pull ${image}
        docker tag ${image} ${var}
done

docker pull coredns/coredns:1.6.5
docker tag coredns/coredns:1.6.5 k8s.gcr.io/coredns:1.6.5

初始化Master节点（kubeadm init 命令）
kubeadm init --apiserver-advertise-address 192.168.5.6 --apiserver-bind-port 6443 --kubernetes-version v1.17.3 --pod-networ                                            k-cidr 10.244.0.0/16

 mkdir -p $HOME/.kube
  sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
  
安装flannel
wget https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml
kubectl apply -f kube-flannel.yml 

wget https://raw.githubusercontent.com/coreos/flannel/master/Documentation/k8s-manifests/kube-flannel-rbac.yml
kubectl apply -f kube-flannel-rbac.yml
  
```

## 53、指定docker版本安装
```
sudo apt-get update
sudo apt-get -y install apt-transport-https ca-certificates curl software-properties-common

curl -fsSL http://mirrors.aliyun.com/docker-ce/linux/ubuntu/gpg | sudo apt-key add -

sudo add-apt-repository "deb [arch=amd64] http://mirrors.aliyun.com/docker-ce/linux/ubuntu $(lsb_release -cs) stable"

sudo apt-get -y update
sudo apt-get -y install docker-ce
apt-cache madison docker-ce
apt-get install docker-ce={18.03.1~ce-0~ubuntu}可根据上面列出的选项选择
 
```

## 54、windows与linux使用xshell上传下载文件
```
yum install lrzsz -y
rz，sz是便是Linux/Unix同Windows进行ZModem文件传输的命令行工具。
运行命令rz，即是接收文件，xshell就会弹出文件选择对话框，选好文件之后关闭对话框，文件就会上传到linux里的当前目录。
运行命令sz file 就是发文件到windows上
```

## 55、 kubernetes1.11镜像下载
```
echo ""

echo "===================================================="

echo "Pull Kubernetes 1.11.1 Images from aliyuncs.com ..."

echo "===================================================="

echo ""

MY_REGISTRY=registry.cn-hangzhou.aliyuncs.com/openthings

## 拉取镜像

docker pull ${MY_REGISTRY}/k8s-gcr-io-kube-apiserver-amd64:v1.11.1

docker pull ${MY_REGISTRY}/k8s-gcr-io-kube-controller-manager-amd64:v1.11.1

docker pull ${MY_REGISTRY}/k8s-gcr-io-kube-scheduler-amd64:v1.11.1

docker pull ${MY_REGISTRY}/k8s-gcr-io-kube-proxy-amd64:v1.11.1

docker pull ${MY_REGISTRY}/k8s-gcr-io-etcd-amd64:3.2.18

docker pull ${MY_REGISTRY}/k8s-gcr-io-pause-amd64:3.1

docker pull ${MY_REGISTRY}/k8s-gcr-io-coredns:1.1.3

## 添加Tag

docker tag ${MY_REGISTRY}/k8s-gcr-io-kube-apiserver-amd64:v1.11.1 k8s.gcr.io/kube-apiserver-amd64:v1.11.1

docker tag ${MY_REGISTRY}/k8s-gcr-io-kube-scheduler-amd64:v1.11.1 k8s.gcr.io/kube-scheduler-amd64:v1.11.1

docker tag ${MY_REGISTRY}/k8s-gcr-io-kube-controller-manager-amd64:v1.11.1 k8s.gcr.io/kube-controller-manager-amd64:v1.11.1

docker tag ${MY_REGISTRY}/k8s-gcr-io-kube-proxy-amd64:v1.11.1 k8s.gcr.io/kube-proxy-amd64:v1.11.1

docker tag ${MY_REGISTRY}/k8s-gcr-io-etcd-amd64:3.2.18 k8s.gcr.io/etcd-amd64:3.2.18

docker tag ${MY_REGISTRY}/k8s-gcr-io-pause-amd64:3.1 k8s.gcr.io/pause-amd64:3.1

docker tag ${MY_REGISTRY}/k8s-gcr-io-coredns:1.1.3 k8s.gcr.io/coredns:1.1.3

echo ""

echo "===================================================="

echo "Pull Kubernetes 1.11.0 Images FINISHED."

echo "into registry.cn-hangzhou.aliyuncs.com/openthings, "

echo "by openthings@https://my.oschina.net/u/2306127."

echo "===================================================="

echo ""

docker tag k8s.gcr.io/pause-amd64:3.1 k8s.gcr.io/pause:3.1
```

## 56、journalctl日志
```
#指定服务和时间段获取日志
journalctl -u kubelet --since "2020-07-29" --until "2020-07-29 21:00"
```

## 57、ubuntu18.04 修改网卡名称为eth0
```
vim /etc/default/grub
GRUB_CMDLINE_LINUX=""  -> GRUB_CMDLINE_LINUX="net.ifnames=0 biosdevname=0"
grub-mkconfig -o /boot/grub/grub.cfg

vim /etc/netplan/xxxxx
经网卡名称改为eth0

```

## 58、进入docker容器网络空间
```
docker inspect -f '{{.State.Pid}}' df577abb762b
nsenter -t {{pid}} exec /bin/sh
```

## 59、根据overlay2目录下的id查找到对应的容器
```
docker ps -q | xargs docker inspect --format '{{.State.Pid}}, {{.Id}}, {{.Name}}, {{.GraphDriver.Data.WorkDir}}' | grep {id}
 docker inspect $(docker ps -qa) |  jq -r 'map([.Name, .GraphDriver.Data.MergedDir]) | .[] | "\(.[0])\t\(.[1])"'
```

## 60、根据进程id获取容器id
```
pstree -sg {pid}
docker ps -q | xargs docker inspect --format '{{.State.Pid}}, {{.Name}}' | grep {parent-pid}
```

## 61、生成自签名证书
```
openssl req -newkey rsa:2048 -nodes -keyout tls.key -x509 -days 365 -out tls.crt
kubectl create secret generic traefik-cert --from-file=tls.crt --from-file=tls.key -n kube-system
```

## 62、rancher证书过期导致重启失败
```
cd /var/lib/cubepaas/management-state/tls
mv token-node.crt token-node.crt.bak
mv localhost.crt localhost.crt.bak
删除数据面节点证书：
rm -rf /etc/kubernetes/ssl/*
```

## 63、ubuntu系统磁盘挂载点出现无法查看文件的情况，出现I/O input error，umount 挂载点也显示busy
```
进入ubuntu紧急模式，运行命令：
xfs_repair /dev/sdb{id} //这里注意是分区而不是整块磁盘
```
