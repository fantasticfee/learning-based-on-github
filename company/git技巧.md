## 1. 查看指定文件修改记录
```
git log -p [filename]
```

## 2. 导出patch
```
git format-patch -3 //从当前分支最新提交点往下共生成3个补丁
git format-patch -1 指定commit号 //生成指定commit号的补丁
```
