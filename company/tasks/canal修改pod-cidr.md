canal 原理：是flannel和calico的结合，采用flannel的overlay vxlan的网络结构，基于底层宿主机网络虚拟化出二层网络，
使跨节点pod通信；网络策略policy采用calico；
组成：
使用daemonset方式部署pod，给每个节点进行网络初始化;
pod中container组成：
install-cni: Using CNI config template from CNI_NETWORK_CONFIG environment variable. 
根据CNI config Created CNI config 10-canal.conflist
kube-flannel： 创建flannel.1设备，（待补）
calico-node：（待补）

canal配置：
```
cat /etc/cni/net.d/10-canal.conflist 
{
  "name": "k8s-pod-network",
  "cniVersion": "0.3.0",
  "plugins": [
    {
      "type": "calico",
      "log_level": "WARNING",
      "datastore_type": "kubernetes",
      "nodename": "idc-hcaas001",
      "ipam": {
        "type": "host-local",
        "subnet": "usePodCidr"
      },
      "container_settings": {
        "allow_ip_forwarding": true
      },
      "policy": {
          "type": "k8s"
      },
      "kubernetes": {
          "kubeconfig": "/etc/kubernetes/ssl/kubecfg-kube-node.yaml"
      }
    },
    {
      "type": "portmap",
      "snat": true,
      "capabilities": {"portMappings": true}
    }
  ]
}
```

pod ip地址分配：flannel默认采用kube-subnet-mgr mode分配ip，根据上面的配置文件也可知，完全根据Kubernetes设置的
pod cidr来分配ip；kubernetes根据controller-manager参数cluster_cidr和node-cidr-mask-size共同决定pod的网段
参考：
https://github.com/coreos/flannel/blob/master/Documentation/running.md
步骤

1. 修改controller-manager启动参数，目的是增加pod网段范围，不需要修改cluster_cidr,只需要添加node-cidr-mask-size；（前提--allocate-node-cidrs=true）；
2. 修改node的podcidr字段，因为canal采用的是flannel的kube-subnet-mgr来管理分配ip，实际上是在节点加入集群时根据第一步中controller-manager
的两个字段设置node 的podcidr字段，而在第一步修改之前加入的node，因为子网段不同，所以也需要修改node的podcidr字段，
