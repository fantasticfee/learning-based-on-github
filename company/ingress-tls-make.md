## 1、初始化CA
```
前提：安装cfssl
mkdir cfssl
cd cfssl
cfssl print-defaults config > ca-config.json
cfssl print-defaults csr > ca-csr.json
```

## 2、Configure CA options
```
edit the ca-config.json file and rename www profile into server
其中的C改为CN，ST和L改为Shanghai
cfssl gencert -initca ca-csr.json | cfssljson -bare ca - //生成ca.csr,ca.pem,ca-key.pem
```

## 3、Generate server certificate
```
cfssl print-defaults csr > server.json
修改server.json中的C、ST、L字段，值同上
cfssl gencert -ca=ca.pem -ca-key=ca-key.pem -config=ca-config.json -profile=server server.json | cfssljson -bare server //生成文件 - server-key.pem - server.csr - server.pem
```

## 4、Create Kubernetes secret
```
kubectl create secret tls tls-secret --key ./server-key.pem --cert server.pem //默认default下生成tls-secret的secret
```
