## 1. 查看指定文件修改记录
```
git log -p [filename]
```

## 2. 导出patch
```
git format-patch -3 //从当前分支最新提交点往下共生成3个补丁
git format-patch -1 指定commit号 //生成指定commit号的补丁
```

## 3. git log操作
```
git log --grep=xxxx //列出包含相关字段的所有commit
git log -p filename //查看日志中对指定文件的修改
git log -S xxx --oneline // 列出所有commit，其中内容包括对xxx的修改
```
