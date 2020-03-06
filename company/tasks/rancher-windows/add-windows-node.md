## 1. 使用阿里云的windows server 2019作为rancher的worker节点，rancher采用最新稳定版本2.3.5
```
安装记录
问题：
1.rancher只支持docker ee,所以windows上需要安装docker ee，否则执行rancher agent命令会报错；
2.windows worker node注册不上,发现第二个启动的rancher agent会异常结束，仔细查看每个已经启动的container，都有错误日志输出；确认为
阿里云与机房内的机器网络不通,导致windows node注册不上,虽然显示正常,node active了,但在windows
上启动的容器还是有相同的错误,还是有一个rancher agent异常结束(这里可能是正常的),这里需要记下,后续碰到,可能有问题;
3.在windows上docker pull非常慢,由于使用的是脚本方式安装的docker ee,没有图形界面配置registry mirror用于镜像加速,
所以速度非常慢
```
