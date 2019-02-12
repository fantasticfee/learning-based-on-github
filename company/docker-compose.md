```
version: "3"
services:
  mysql:
    image: mysql:5.7
    container_name: mysql
    restart: always
    network_mode: "host"
    volumes:
      - /data/mysql/data:/var/lib/mysql
      - /data/mysql/logs:/logs
      - /mysql/config/my.cnf:/etc/my.cnf
      - /mysql/init:/docker-entrypoint-initdb.d/
    environment:
      MYSQL_ROOT_PASSWORD: "123456"
      MYSQL_DATABASE: ezk8s
  ezk8s:
    image: registry.cn-hangzhou.aliyuncs.com/fantasticfee/ezk8s:0.4.16.14
    container_name: ezk8s
    restart: always
    network_mode: "host"
    command:
      - /bin/sh
      - -c
      - |
        ./ezk8s orm syncdb
        ./ezk8s
    depends_on:
      - mysql
    volumes:
      - /data/ezk8s/conf:/usr/kubespray/conf
      - /data/ezk8s/inventory:/usr/kubespray/inventory
      - /data/ezk8s/log/kubespray:/usr/kubespray/log
      - /root/.ssh:/root/.ssh
  etcd:
    image: registry.cn-hangzhou.aliyuncs.com/fantasticfee/coreos-etcd:v3.2.24
    container_name: etcd
restart: always
    network_mode: "host"
  cm:
    image: registry.cn-hangzhou.aliyuncs.com/licheng/cm:0.1
    container_name: cm
    restart: always
    network_mode: "host"
    command: '--etcd=http://localhost:2379'
    depends_on:
      - etcd  
  cloud:  
    image: registry.cn-hangzhou.aliyuncs.com/linkcloud/private-cloud:0.1
    container_name: cloud
    restart: always
    ports:  
      - "8080:8080"
  manager_web:
    image: registry.cn-hangzhou.aliyuncs.com/fantasticfee/iview-admin:1.1
    container_name: manager_web
    restart: always
    network_mode: "host"
    depends_on:
      - cm    
      - ezk8s 
      - cloud 
```
