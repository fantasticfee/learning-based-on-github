## 1.sdwan java-grpc在调用kubeapps接口安装应用时报403没有权限的问题?

```
java-grpc对应的sa没有足够的权限;
检查sa,知道权限即对应namespace的默认secret;
检查java-grpc相关的clusterrole,找到java-admin,但没有对应namespace的clusterrolebinding,
创建对应namespace的clusterrolebinding,解决问题
```

## 2. java-grpc 调用kubeapps安装granafa时报502的错误,而其他应用能够正常安装
```
确认为kubeapps的cpu和内存太小,将kubeapps和cpu扩为原来的一倍,问题解决
```

## 3. 前端访问grafana出现404的现象,查看日志,发现grafana日志报302错误,然后开发方认为是环境问题,
证据是程序在另一个环境都能够正常运行
