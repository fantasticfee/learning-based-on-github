## 1. 使用阿里云的windows server 2019作为rancher的worker节点，rancher采用最新稳定版本2.3.5
```
安装记录
问题：
1.rancher只支持docker ee,所以windows上需要安装docker ee，否则执行rancher agent命令会报错；
2.windows worker node注册不上,发现第二个启动的rancher agent会异常结束，仔细查看每个已经启动的container，都有错误日志输出；确认为
阿里云与机房内的机器网络不通,导致windows node注册不上,虽然显示正常,node active了,但在windows
上启动的容器还是有相同的错误,还是有一个rancher agent异常结束(这里可能是正常的),这里需要记下,后续碰到,可能有问题;
3.在windows上docker pull非常慢,由于使用的是脚本方式安装的docker ee,没有图形界面配置registry mirror用于镜像加速,
所以速度非常慢
```

## 2. 阿里云windows server 2019 安装docker ee过程
```
参考：https://docs.docker.com/ee/docker-ee/windows/docker-ee/
这里需要注意：对于不同的windows版本，下面的命令不一定能够执行
Invoke-WebRequest -UseBasicParsing -OutFile docker-19.03.5.zip https://download.docker.com/components/engine/windows-server/19.03/docker-19.03.5.zip
# Stop Docker service
Stop-Service docker

# Extract the archive.
Expand-Archive docker-19.03.5.zip -DestinationPath $Env:ProgramFiles -Force

# Clean up the zip file.
Remove-Item -Force docker-19.03.5.zip

# Install Docker. This requires rebooting.
$null = Install-WindowsFeature -Name containers

# Add Docker to the path for the current session.
$env:path += ";$env:ProgramFiles\docker"

# Optionally, modify PATH to persist across sessions.
$newPath = "$env:ProgramFiles\docker;" +
[Environment]::GetEnvironmentVariable("PATH",
[EnvironmentVariableTarget]::Machine)

[Environment]::SetEnvironmentVariable("PATH", $newPath,
[EnvironmentVariableTarget]::Machine)

# Register the Docker daemon as a service.
dockerd --register-service

# Start the Docker service.
Start-Service docker
```

## 3. rancher windows问题记录
```
1. 开启监控，部署在window node的exporter未知原因无法启动，windows节点的cpu较高，
变化从50%~100%；
2. 试图规避1问题，重启windows机器，发现exporter启动成功，cpu也不是很高，但exporter日志中还是显示service失败了，
node指标无法获取，
```
