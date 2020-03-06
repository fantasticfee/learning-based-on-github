## 1. 使用阿里云的windows server 2019作为rancher的worker节点，rancher采用最新稳定版本2.3.5
```
问题：
rancher只支持docker ee,所以windows上需要安装docker ee，否则执行rancher agent命令会报错；
第二个启动的rancher agent会异常结束，仔细查看每个已经启动的container，都有错误日志输出；
```
