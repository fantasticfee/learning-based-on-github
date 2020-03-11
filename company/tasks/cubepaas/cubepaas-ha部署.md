

机器准备：

三台机器用于部署外层k8s集群，最低配置2C4G，推荐4C8G

三台机器用于部署业务集群（内层k8s），最低配置4C8G

1、修改/etc/ssh/sshd_config允许root登陆

```
PermitRootLogin prohibit-password改为PermitRootLogin yes
service sshd restart
```



centos： 关闭防火墙

```
 systemctl stop firewalld.service #停止firewall 
 systemctl disable firewalld.service #禁止firewall开机启动 
 selinux关闭：setenforce 0 
```



2、安装docker-ce

```
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
```



3、在其中一台节点上执行到其他节点的免密码登陆并启动外层的k8s;

将rancher-cluster.yml中的节点IP改为对应部署节点；

命令：

```
ssh-keygen
ssh-copy-id -i ~/.ssh/id_rsa.pub root@ip
rke up --config ./rancher-cluster.yml
mkdir -p ~/.kube
cat kube_config_rancher-cluster.yml(由第三行执行成功生成) > ~/.kube/config
```



4、helm初始化

```plain
kubectl -n kube-system create serviceaccount tiller
kubectl create clusterrolebinding tiller \
  --clusterrole=cluster-admin \
  --serviceaccount=kube-system:tiller
helm init --service-account tiller -i registry.cn-hangzhou.aliyuncs.com/google_containers/tiller:v2.15.1 
```



5、cert-manger安装，用于给后续安装的Linkcloud分发证书

```plain
kubectl apply -f https://raw.githubusercontent.com/jetstack/cert-manager/release-0.9/deploy/manifests/00-crds.yaml
kubectl create namespace cert-manager
kubectl label namespace cert-manager certmanager.k8s.io/disable-validation=true
helm repo add jetstack https://charts.jetstack.io
helm repo update
helm install \
  --name cert-manager \
  --namespace cert-manager \
  --version v0.9.1 \
  jetstack/cert-manager
```

创建证书自签名的secret，如果用自签名，就不用安装cert-manager了（参考 [https://docs.rancher.cn/rancher2x/installation/helm-ha-install/online/tcp-l4/rancher-install.html#_2-1-%E4%BD%BF%E7%94%A8%E6%9D%83%E5%A8%81ca%E6%9C%BA%E6%9E%84%E9%A2%81%E5%8F%91%E7%9A%84](https://docs.rancher.cn/rancher2x/installation/helm-ha-install/online/tcp-l4/rancher-install.html#_2-1-使用权威ca机构颁发的) ）

使用链接中的脚本自动生成证书，然后执行下面的操作

```bash
# 指定配置文件
kubeconfig=xxx/xxx/xx.kubeconfig.yml
# 创建命名空间
kubectl --kubeconfig=$kubeconfig \
    create namespace cattle-system
# 服务证书和私钥密文
kubectl --kubeconfig=$kubeconfig \
    -n cattle-system create \
    secret tls tls-rancher-ingress \
    --cert=./tls.crt \
    --key=./tls.key
# ca证书密文
kubectl --kubeconfig=$kubeconfig \
    -n cattle-system create secret \
    generic tls-ca \
    --from-file=cacerts.pem

```

6、安装linkcloud

到charts目录执行：

```
helm install . --name cubepaas --namespace cubepaas-system --set hostname=netbank.cubepaas.com(自行更改为自己的域名) --set addLocal=false
```

自签名安装:

```bash
 helm install . --name cubepaas --namespace cubepaas-system --set hostname=netbank.cubepaas.com(自行更改为自己的域名) --set addLocal=false --set ingress.tls.source=secret --set privateCA=true
```



7、在阿里云的DNS云解析上添加域名和部署linkcloud节点的公网IP

8、步骤6若执行成功后，则可以正常访问域名 netbank.cubepaas.com（此处只是例子，修改为自己的域名);登陆 netbank.cubepaas.com后，根据界面创建业务集群。

