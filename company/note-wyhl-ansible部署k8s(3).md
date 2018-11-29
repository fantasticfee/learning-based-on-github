# 使用kubespray部署kubernetes

## 简介
      kubespray作为自动化部署k8s集群的一款工具，其本质是一堆ansible的role文件,底层通过ansible的方式自动化安装高可用k8s集群。

## 安装准备
* 1：部署的虚拟机采用centos7发行版本，需要升级内核，因为overlay2要求内核版本4.0以上：

      在 CentOS 7 上启用 ELRepo 仓库
       rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org
       rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
      列出可用的内核相关包：
       yum --disablerepo="*" --enablerepo="elrepo-kernel" list available
      安装内核：
       yum --enablerepo=elrepo-kernel install kernel-lt.x86_64
       配置启动项，选择新内核，然后重新启动：
       vim /etc/default/grub
       GRUB_DEFAULT=0   #<==save改为0
       grub2-mkconfig -o /boot/grub2/grub.cfg

* 2：安装太慢可以直接用pip阿里云加速：

      yum install epel-release –y

      yum install git python-pip -y

      pip install pip --upgrade -i http://mirrors.aliyun.com/pypi/simple/ --trusted-host mirrors.aliyun.com

      pip install --no-cache-dir ansible -i http://mirrors.aliyun.com/pypi/simple/ --trusted-host mirrors.aliyun.com

* 3：[下载最新的kubespray版本](https://github.com/kubernetes-sigs/kubespray)

## 安装步骤

* 1：安装依赖

      sudo pip install -r requirements.txt

* 2：修改inventory输入文档，修改hosts.ini文档

      cp -rfp inventory/sample inventory/mycluster

      vi inventory/mycluster/hosts.ini

      修改后的host文档如下：
      [all]
       node1 ansible_host=192.168.122.109   ip=192.168.122.109 etcd_member_name=etcd1
       node2 ansible_host=192.168.122.102   ip=192.168.122.102 
       node3 ansible_host=192.168.122.107   ip=192.168.122.107 etcd_member_name=etcd2
       node4 ansible_host=192.168.122.108  ip=192.168.122.108 etcd_member_name=etcd3

      [kube-master]
       node1
       node3

      [etcd]
       node1

      [kube-node]
       node2
       node3
       node4

      [k8s-cluster:children]
      kube-master
      kube-node

      由于被墙的原因，gcr开头的镜像都要修改，这里我替换了阿里云的镜像。
      需要修改的文件如下：
      kubespray/roles/kubernetes-apps/ansible/defaults/main.yml
      kubespray/roles/download/defaults/main.yml
      kubespray/extra_playbooks/roles/download/defaults/main.yml
      inventory/mycluster/group_vars/k8s-cluster/k8s-cluster.yml
      inventory/mycluster/group_vars/k8s-cluster/k8s-cluster.yml


* 3：执行部署

      ansible-playbook -i inventory/mycluster/hosts.ini --become --become-user=root cluster.yml

##问题

* 1： pip install -r requirements.txt执行出错

      错误信息：Cannot uninstall 'requests'. It is a distutils installed project and thus we cannot accurately determine 
      which files belong to it which would lead to only a partial uninstall.
      解决方法：pip install six --upgrade --ignore-installed six

* 2： 解决完第1个问题，继续执行pip install -r requirements.txt出错

      错误信息：ERROR! no action detected in task. This often indicates a misspelled module name, or incorrect module path.
      解决方法：requirements.txt安装时ansible-modules-hashivault安装出问题，需要执行下面的命令
      pip install ansible-modules-hashivault --upgrade --ignore-installed ansible-modules-hashivault

* 3： 在执行部署时，出现错误，docker启动失败

      问题现象：
      fatal: [node2]: FAILED! => {"changed": false, "msg": "Unable to start service docker: Job for docker.service failed because the control process exited with error code. See \"systemctl status docker.service\" and \"journalctl -xe\" for details.\n"}
      fatal: [node3]: FAILED! => {"changed": false, "msg": "Unable to start service docker: Job for docker.service failed because the control process exited with error code. See \"systemctl status docker.service\" and \"journalctl -xe\" for details.\n"}
      fatal: [node1]: FAILED! => {"changed": false, "msg": "Unable to start service docker: Job for docker.service failed because the control process exited with error code. See \"systemctl status docker.service\" and \"journalctl -xe\" for details.\n"}
      日志信息：
      Nov  9 02:47:42 x systemd: Cannot add dependency job for unit docker-storage-setup.service, ignoring: Unit not found.
      Nov  9 02:47:42 x systemd: Starting Docker Application Container Engine...
      Nov  9 02:47:42 x docker: unknown flag: --insecure-registry

      解决方法：安装docker的选项出了问题，kubespray安装dockers的默认driver是devicemapper,使能了docker-storage，而devicemapper是需要lvm分区的，
      由于新建的centos虚拟机默认没有，所有docker启动失败。修改kubespray的docker选项，devicemapper改为overlay2

* 4：经过第3个问题的修改，docker启动还是失败

      解决方法：尝试加入了一个新节点，发现新节点成功了，推测老节点之前安装过其他版本docker的原因，有残余配置没有卸载干净，导致docker安装一直失败，
      新节点部署，docker安装成功。

* 5：继续执行部署，出现apiserver一直没有启动的情况，现象一直wait apiserver timeout

      日志信息： Error: unknown flag: --endpoint-reconciler-type
      解决方法：reconciler是个新特性，查看hyperkube 的版本，还是老版本，更换新版本，apiserver果然恢复正常。


