# kubespray部署

## 环境

版本:kubespray v2.8.0
系统：centos7

## 准备

* 修改镜像脚本

#!/bin/bash
grc_image_files=(
./extra_playbooks/roles/download/defaults/main.yml
./extra_playbooks/roles/kubernetes-apps/ansible/defaults/main.yml
./roles/download/defaults/main.yml
./roles/kubernetes-apps/ansible/defaults/main.yml
./inventory/mycluster/group_vars/k8s-cluster/k8s-cluster.yml   //根据模板个人创建的host目录
)
 
 for file in ${grc_image_files[@]} ; do
 sed -i 's/gcr.io\/google[_-]containers/registry.cn-hangzhou.aliyuncs.com\/fantasticfee/g' $file
 done
 
 执行完此脚本，若还有gcr开头的镜像，就需要自己手动更换了。
 
* 修改文件roles/download/defaults/main.yml，由于网络原因，修改kubeadm和hyperkube的下载url：

  注意：下面两个url仅用于kubespray v2.8.0

kubeadm_download_url: "https://github.com/fantasticfee/ks-biniary-2.8.0/raw/master/kubeadm"
hyperkube_download_url: "https://github.com/fantasticfee/ks-biniary-2.8.0/raw/master/hyperkube"

## 执行部署

ansible-playbook -i inventory/cluster-2/hosts.ini cluster.yml -b -vvv

## 增加rr配置

* 1、hosts.ini增加主机

* 2、inventory/test/group_vars/all/all.yml文件里添加peers

peers:
  - router_id: "10.99.0.2"
    as: "64512"
  - router_id: "10.99.0.3"
    as: "64512"

inventory/test/group_vars/k8s-cluster/k8s-net-calico.yml
peer_with_router: true

## 问题

* 1、增加rr，calico使用bgp  router reflactor部署，其中kube_api_anonymous_auth在部署rr主机时总是false，看起来inventory中的
变量没有将default中的变量覆盖，导致做preinstall中verify任务时总是失败


  
