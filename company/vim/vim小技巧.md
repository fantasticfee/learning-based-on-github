## 一、 vim与系统剪切板的复制与粘贴
*1、查看vim版本是否支持clipboard
```
vim --version | grep "clipboard"
```
结果中clipboard前面有一个小小的减号，说明不支持。

*2、如果不支持，安装vim-gnome
sudo apt-get install vim-gnome

*3、重复执行第1步的命令，可以看到已经支持了，然后打开vim，选中要复制的段落，执行
命令"+y,就把相应的内容复制到系统剪切版了

*4. 插件安装时需要走代理,直接执行下面命令,然后执行PlugInstall类似插件安装命令,速度会快很多
    ALL_PROXY='socks5://xx.xx.xx.xx:1080' vim
