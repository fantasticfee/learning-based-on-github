## 1、修改nfs client deployment
```
用到的文件来自于git clone https://github.com/kubernetes-incubator/external-storage.git的nfs-client目录：
在deployment中将nfs server和path修改为当前nfs服务器的地址和路径
kubectl apply -f rbac.yaml
kubectl apply -f deployment.yaml
kubectl apply -f storageclass.yaml
```
