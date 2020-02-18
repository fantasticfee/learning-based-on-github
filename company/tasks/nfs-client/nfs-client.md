## 1、修改nfs client deployment
```
在deployment中将nfs server和path修改为当前nfs服务器的地址和路径
kubectl apply -f rbac.yaml
kubectl apply -f deployment.yaml
kubectl apply -f storageclass.yaml
```
