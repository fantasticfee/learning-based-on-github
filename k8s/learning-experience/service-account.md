service account作为pod访问apiserver的认证:

当pod没有指定service account时,admission controller会自动给pod创建名称为default的service account,而
token controller会listwathch service account,从而创建对应的token,生成的token存放于pod中的目录
/var/run/secrets/kubernetes.io/serviceaccount/token;ca证书存放于/var/run/secrets/kubernetes.io/serviceaccount/ca.crt;

小测试:

```
kubectl run -i --tty --rm curl-tns --image=radial/busyboxplus:curl
 curl https://kubernetes:8443/api  //会显示因为没有证书访问失败
 cd /var/run/secrets/kubernetes.io/serviceaccount
 CA_CERT=/var/run/secrets/kubernetes.io/serviceaccount/ca.crt
TOKEN=$(cat /var/run/secrets/kubernetes.io/serviceaccount/token)
NAMESPACE=$(cat /var/run/secrets/kubernetes.io/serviceaccount/namespace)
curl --cacert $CA_CERT -H "Authorization: Bearer $TOKEN" "https://kubernetes/api/v1/namespaces/$NAMESPACE/services/" //报403错误

//将service account和role联系起来
kubectl create rolebinding default-view \
  --clusterrole=view \
  --serviceaccount=default:default \
  --namespace=default
//重复执行上述curl操作,可以显示正常
```
这个测试说明service account本身是没有权限的,需要rolebinding或clusterrolebinding才能赋予其相应的权限;
这里也可以认识到认证和鉴权的区别,小测试中报403错误,说明认证已经通过,但鉴权失败;
