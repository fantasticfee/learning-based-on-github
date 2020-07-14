## 1. 删除指定行
```
sed -i '/balance/d' user.yaml 
```

## 2、在指定字符行后面插入多行
```
sed -i -e '/apiVersion\:\ management.cubepaas.com\/v3/a\  balance\:\n    free\: 0\n    recharge\: 0\n    total\: 0' user.yaml
```
