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
```
在没有理清sdwan业务逻辑的前提下,给予sdwan定位问题:
因为需要确认是不是环境那里配置问题导致的,所以对比了dev(正常的)和test(有问题)相关组件的配置,
两个环境的java-grpc和grafana的配置完全相同,但不知道是什么原因,在test环境中从java-grpc获取的
cookie访问grafana为401,而test正常;

同事定位问题策略:
既然前端访问grafana出错,直接在grafana的机器上抓包,对比前端和使用curl命令请求,发现前端发出的请求没有带上
cookie,然后curl模仿前端发送一模一样的请求,返回结果是200,但页面显示有错误,再抓包发现请求的iframe不对,
将iframe换成重定向的url,curl请求就能正常工作,结果还是怀疑前端有问题,查看浏览器记录的cookie对应的域名,发现域名不对,
后来发现是java-major配置的域名错误导致的,修改后,可以正常访问;

业务逻辑:
get cookie: web-admin(前端)-----> java-major ------> java-grpc ----> grafana
访问grafana dashboard: 浏览器 ------> ingress(grafana) -------> grafana

虽然根据同事定位解决问题了,但发现在另外一个namespace下同样的grafana,前端访问grafana还是失败,而此失败的namespace
除了grafana不共用,其他组件都是共用的,这就令人费解了.
```
