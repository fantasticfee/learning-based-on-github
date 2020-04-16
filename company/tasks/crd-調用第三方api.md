##本任務主要是做一個簡單的crd action,調用dockerhub 的api;
## 1.遇到的一些坑:
```
    1.沒有去查dockerhub的代碼倉庫,直接根據當前postman調用的api的调用结果,
    在代碼中的结构体构造根据调用结果来猜成员,这个是自己粗心大意了,以为廷简单的,但每次
    出现unmarshal失败,去修改结构体,都需要在type中重新go generate,然后在主rancher库中trash,特别浪费时间,因为这个点
    的不断尝试,浪费了些时间;  以后如果能够找到调用api的代码仓库,还是要去找的,这样方便了后面的调用;
    2.type仓库中对于struct中定义为空struct和interface{}类型的成员,执行go generate会报错,建议定义一个新类型,
    比如"type aa struct {}",aa这个就是空结构体,再把aa变为某个struct的成员,这样,go generate就不会报错了;
    3. 给crd action设置output后,在最终把结构返回给前端,也就是调用response.Write,这里的不能直接返回struct,
    需要将struct转为map[string]interface{},[]interface{}等类似空接口,不然还是会报错;
    ```
