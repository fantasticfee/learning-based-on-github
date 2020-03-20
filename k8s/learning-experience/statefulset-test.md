## 以mysql为例,创建一个典型的主从mysql集群
```
具体场景:
1.是一个“主从复制”（Maser-Slave Replication）的 MySQL 集群；
2.有 1 个主节点（Master）；
3.有多个从节点（Slave）；
4.从节点需要能水平扩展；
5.所有的写操作，只能在主节点上执行；
```

需要解决的三个问题:
1.基于同一个statefulset启动三个不同的pod,怎么区分主从?
2.从pod如何从主pod中拷贝数据来备份?
3.在slave pod启动之前需要执行初始化sql语句,怎么做?
