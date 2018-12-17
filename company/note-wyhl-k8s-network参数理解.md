
## flannel：

  专门为K8S的跨主机通信提供的一种第三层网络方案，flannle相关的网络配置存储在etcd中，集群内的所有节点都运行flannel，
flannel有配置一个网络空间，该net分配给每个host一个subnet，subnet分配IP给pod。
选择哪个对外网卡与subnet绑定
1) 如果”–iface”和”—-iface-regex”都未指定时，则直接选取默认路由所使用的输出网卡
2) 如果”–iface”参数不为空，则依次遍历其中的各个实例，直到找到和该网卡名或IP匹配的实例为止
3) 如果”–iface-regex”参数不为空，操作方式和2)相同，唯一不同的是使用正则表达式去匹配
flannel_backend_type参数：vxlan,hosthw等。主要目的是如何转发包到目的pod ip对应的host   
  每种类型大概有什么区别？
    hostgw:监听到etcd的eventadd事件，根据添加的lease信息，直接添加一条路由，目的是subnet,网关是对应的host
    udp：监听到etcd的eventadd事件，flannel往数组中增添一条对应的lease记录，每次转发都查询这里记录
    vxlan: eventadd事件后，lease信息中多了flannel的mac地址，subnet与mac地址映射，mac地址与host映射
原理：将目的地址为非本机容器的流量送到相应主机：如果集群的主机都在一个子网内，就搞一条路由转发过去；若是不在一个子网内，就搞一条隧道转发过去。
flannel将上述过程进行了自动化。


## Calico：

  把每个操作系统的协议栈认为是一个路由器，然后把所有的容器认为是连在这个路由器上的网络终端，
  在路由器之间跑标准的路由协议——BGP的协议，然后让它们自己去学习这个网络拓扑该如何转发。
  监听etcd，每有一个新host或新pod，产生对应的虚拟网络设备。
  
  peer_with_router： enabling router peering will disable calico's default behavior ('node mesh'),只有在大规模部署或
  自定义整个拓扑网络时，才会选true
  nat_outgoing：能够访问ip pool规定网段以外的网址
  calico_pool_name：ip地址池名称，该资源是ip网段的集合
  global_as_num：bgp as 全局号
  calico_mtu：ipip模式中的mtu
    --ipip ：允许跨子网
    
## kube-router:

  为k8s提供了负载分担，取代kube-proxy,整个pod网络的联通。
  着重讲网络：使用bgp，每个host作为一个router，host中的pod看作终端。
  Kube-router监视Kubernetes API服务器以获取服务/端点的更新，并自动同步IPVS配置以反映所需的服务状态。 Kube-router使用IPVS伪装模式
Usage of ./kube-router:
      --advertise-cluster-ip                将该服务的集群IP添加到RIB，以便通告给BGP peers.
      --advertise-external-ip               将服务的外部IP添加到RIB，以便将其通告给BGP peers.
      --cleanup-config                      清理iptables规则，ipvs，ipset配置并退出.
      --cluster-asn uint                    集群节点运行iBGP的ASN编号.
      --cluster-cidr string                 群集中的CIDR范围。它被用来识别pods的范围.
      --config-sync-period duration         apiserver配置同步之间的延迟（例如“5s”，“1m”）。必须大于0.（默认1m0s）
      --enable-overlay                      当enable-overlay设置为true时，IP-in-IP隧道将用于跨不同子网中节点的pod-pod联网。如果设置为false，则不使用隧道，并且路由基础架构预计为不同子网中的节点之间的pod-pod联网路由流量（默认值为true）
      --enable-pod-egress                   从Pod到群集外的SNAT流量。 （默认为true）
      --hairpin-mode                        为每个服务端点添加iptable规则以支持流量管控.
  -h, --help                                打印使用信息.
      --hostname-override string            覆盖节点的NodeName。如果kube-router无法自动确定您的NodeName，请设置此项.
      --iptables-sync-period duration       iptables规则同步之间的延迟（例如'5s'，'1m'）。必须大于0.（默认1m0s）
      --ipvs-sync-period duration           ipvs config同步之间的延迟（例如'5s'，'1m'，'2h22m'）。必须大于0.（默认1m0s）
      --kubeconfig string                   具有授权信息的kubeconfig文件的路径（主位置由主标志设置）。
      --masquerade-all                      SNAT所有流量到群集IP /节点端口。
      --master string                       Kubernetes API服务器的地址（覆盖kubeconfig中的任何值）。
      --nodeport-bindon-all-ip              对于NodePort类型的服务，创建监听节点的所有IP的IPVS服务.
      --nodes-full-mesh                     集群中的每个节点都将建立与其他节点的BGP对等关系。 （默认为true）
      --peer-router-asns uintSlice          集群节点将向其通告集群ip和节点的pid cidr的BGP peers的ASN编号。 （默认[]）
      --peer-router-ips ipSlice             所有节点将对等的外部路由器的IP地址，并通告集群ip和pod cidr。 （默认[]）
      --peer-router-passwords stringSlice   用“--peer-router-ips”定义的BGP peers进行认证的密码。
      --routes-sync-period duration         路线更新与广播之间的延迟（例如“5s”，“1m”，“2h22m”）。必须大于0.（默认1m0s）
      --run-firewall                        启用网络策略 - 设置iptables为pod提供入口防火墙。 （默认为true）
      --run-router                          启用Pod网络 - 通过iBGP发布并学习到Pod的路由。 （默认为true）
      --run-service-proxy                   启用服务代理 - 为Kubernetes服务设置IPVS。 （默认为true）```
      
      
  ## etcd
  
    --auto-compaction-retention：：历史数据多长时间压缩一次
    --quota-backend-bytes：ETCDdb数据大小，默认是２G,当数据达到２G的时候就不允许写入，必须对历史数据进行压缩才能继续写入
    etcd_peer_client_auth:https使能
    etcd_metrics：模式分为基本和扩展
    etcd_memory_limit：运行内存，默认是4G，若没有4G，可自行设置
   
   
   ## all.yml: 
   access_ip:一般为master node ip,更复杂的情况可能代表不同的含义
   
   以下参数用于HA：
   apiserver_loadbalancer_domain_name: "elb.some.domain"  
    loadbalancer_apiserver:   
      address: 1.2.3.4
      port: 1234
      //用于外部LB
      
    loadbalancer_apiserver_localhost: true           //是否开启内部LB
    nginx_kube_apiserver_port: 8443         //本地内部负载均衡接口，内部LB仅接受客户端连接到本地主机
   
    
   
