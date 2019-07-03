## 1、linux mounts: path /var/lib/rancher is mounted on / but it is not a shared mount Error: failed to start containers: kubelet
```
解决方法：
 mount -o bind / /
 mount --make-shared /
```
