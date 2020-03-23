## 1、部署主备环境，在主环境挂掉的情况下，能主动切换到备环境；而在主环境正常后，能够切换回来，下面以Nginx为例子

```
apt-get install keepalived //安装keepalived
apt-get install nginx  //安装nginx

下面大括号中是不要的，添加在这里是提醒自己，这是一个错误的示范，
只需要安装keeplived就足够了，虚ip还随着keeplived的启动状态，从主漂到备；
{
ifconfig ens33:0 172.16.130.150 netmask 255.255.255.0  //添加虚ip
若需要机器重启后生效，在/etc/network/interfaces文件中添加：

auto ens33:0  
iface ens33:0 inet static  
name Ethernet alias LAN card  
address 172.16.130.150
netmask 255.255.255.0
broadcast 172.16.130.255
network 172.16.130.0

/etc/init.d/networking restart  //重启网络
}


/etc/keepalived/keepalived.conf添加配置：
//主服务器
global_defs {
   notification_email {

   }
}

vrrp_script chk_nginx {
    script "/etc/keepalived/check_nginx.sh"
    interval 2                    # 每2s检查一次
    weight -5                     # 检测失败（脚本返回非0）则优先级减少5个值
    fall 3                        # 如果连续失败次数达到此值，则认为服务器已down
    rise 2                        # 如果连续成功次数达到此值，则认为服务器已up，但不修改优先级
}

vrrp_instance VI_1 {              # 实例名称
    state MASTER                  # 可以是MASTER或BACKUP，不过当其他节点keepalived启动时会自动将priority比较大的节点选举为MASTER
    interface ens33                # 节点固有IP（非VIP）的网卡，用来发VRRP包做心跳检测
    virtual_router_id 51          # 虚拟路由ID,取值在0-255之间,用来区分多个instance的VRRP组播,同一网段内ID不能重复;主备必须为一样
    priority 100                  # 权重，主服务器要比备服务器高
    advert_int 1                  # 检查间隔默认为1秒,即1秒进行一次master选举(可以认为是健康查检时间间隔)
    authentication {              # 认证区域,认证类型有PASS和HA（IPSEC）,推荐使用PASS(密码只识别前8位)
        auth_type PASS            # 默认是PASS认证
        auth_pass 1111            # PASS认证密码
    }
    virtual_ipaddress {
        172.16.130.150           # 虚拟VIP地址,允许多个,一行一个
    }
    track_script {                # 引用VRRP脚本，即在 vrrp_script 部分指定的名字。定期运行它们来改变优先级，并最终引发主备切换。
        chk_nginx          
    }                
}

//备服务器
global_defs {
   notification_email {

   }
}

vrrp_script chk_nginx {
    script "/etc/keepalived/check_nginx.sh"
    interval 2                    # 每2s检查一次
    weight -5                     # 检测失败（脚本返回非0）则优先级减少5个值
    fall 3                        # 如果连续失败次数达到此值，则认为服务器已down
    rise 2                        # 如果连续成功次数达到此值，则认为服务器已up，但不修改优先级
}

vrrp_instance VI_1 {              # 实例名称
    state BACKUP                  # 可以是MASTER或BACKUP，不过当其他节点keepalived启动时会自动将priority比较大的节点选举为MASTER
    interface ens33                # 节点固有IP（非VIP）的网卡，用来发VRRP包做心跳检测
    virtual_router_id 51          # 虚拟路由ID,取值在0-255之间,用来区分多个instance的VRRP组播,同一网段内ID不能重复;主备必须为一样
    priority 50                   # 权重，主服务器要比备服务器高
    advert_int 1                  # 检查间隔默认为1秒,即1秒进行一次master选举(可以认为是健康查检时间间隔)
    authentication {              # 认证区域,认证类型有PASS和HA（IPSEC）,推荐使用PASS(密码只识别前8位)
        auth_type PASS            # 默认是PASS认证
        auth_pass 1111            # PASS认证密码
    }
    virtual_ipaddress {
        172.16.130.150           # 虚拟VIP地址,允许多个,一行一个
    }
    track_script {                # 引用VRRP脚本，即在 vrrp_script 部分指定的名字。定期运行它们来改变优先级，并最终引发主备切换。
        chk_nginx          
    }                
}

// /etc/keepalived/check_nginx.sh（主备切换脚本，这里根据实际需要切换）（主备机器都要有）
#!/bin/bash  
#代码一定注意空格，逻辑就是：如果nginx进程不存在则启动nginx,如果nginx无法启动则kill掉keepalived所有进程  
A=`ps -C nginx --no-header | wc -l`  
if [ $A -eq 0 ];then  
  /etc/init.d/nginx start  
  sleep 3  
  if [ `ps -C nginx --no-header | wc -l` -eq 0 ];then  
    killall keepalived  
  fi  
fi  

//设置keepalived开机自启
update-rc.d keepalived defaults
```

## 2. keeplived容器版本
```
参考 https://github.com/osixia/docker-keepalived
modprobe ip_vs
//将主备配置文件放到/opt/keeplived目录下
docker run -itd  --name keepalived --cap-add=NET_ADMIN --net host -v /opt/keeplived/keeplived.conf:/container/service/keepalived/assets/keepalived.conf osixia/keepalived --copy-service
```
