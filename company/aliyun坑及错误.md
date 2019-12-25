## 1、调用aliyun SDK，出现SDK.CanNotResolveEndpoint错误
````
原因：access key 和 secret错误
````
## 2、使用flannel 的ali-vpc模式，有个坑，部署完后，flannel pod报错不能访问aliyun地址
````
解决方法：设置flannel的dns，因为默认dns策略是clusterFirst，但集群网络本来就有flannel配置的，所以就变成“鸡生蛋”的问题，这里需要配置dns为阿里云的dns servier
dnsPolicy: None
      dnsConfig:
        nameservers:
        - 223.6.6.6
````
