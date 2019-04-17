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
```npm i -g npm@5.0.0
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

## 18、centos7 install npm
```
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
