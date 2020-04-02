## 1.使用nsenter和tcpdump抓包 
```
docker inspect --format "{{.State.Pid}}" <container id/name> //显示容器进程id
nsenter -n -t <container pid>  //进入容器网络空间
tcpdump -i eth0 tcp and port 80 -vvv  //抓包查看
```
