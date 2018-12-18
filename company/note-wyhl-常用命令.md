## 1、给git账户生产ssh key

       ssh-keygen -t rsa -C "youremail@example.com"
       登录github。打开setting->SSH keys，点击右上角 New SSH key，
       把生成好的公钥id_rsa.pub放进 key输入框中，再为当前的key起一个title来区分每个key。

## 2、linux挂载windows共享文件

       问题：[root@x home]# mount -t cifs -o username=zy,password=1qaz2WSX# \\10.93.0.247\ezk8s ./windows
       mount: wrong fs type, bad option, bad superblock on \10.93.0.247ezk8s,
              missing codepage or helper program, or other error
              (for several filesystems (e.g. nfs, cifs) you might
              need a /sbin/mount.<type> helper program)

              In some cases useful info is found in syslog - try
              dmesg | tail or so.
       解决方法：yum install cifs-utils

       重新执行mount命令，遇到问题：
         mount.cifs: bad UNC (\10.93.0.247ezk8s)
  
## 3、mysql
       问题：启动beego二进制程序，出现error：
       register db Ping `default`, Error 1044: Access denied for user 'test'@'%' to database 'ezk8s'
       解决：
       在mysql服务器上执行：
       GRANT ALL PRIVILEGES ON *.* TO 'test'@'%';

       改密码：update user set password=password("123") where user="root";
       FLUSH PRIVILEGES;(刷新权限，任何涉及权限的操作都要执行此步来生效）

## 4、修改密码错误
       问题：
       [root@linux1 /]# passwd Oracle
       Changing password for user oracle.
       New password: 
       Retype new password: 
       passwd: Authentication token manipulation error
       解决方法：
       [root@linux1 /]# chattr -i /etc/passwd
       [root@linux1 /]# chattr -i /etc/shadow

       [root@linux1 /]# passwd oracle
       Changing password for user oracle.
       New password: 
       Retype new password: 
       passwd: all authentication tokens updated successfully.

       [root@linux1 /]# chattr +i /etc/shadow
       [root@linux1 /]# chattr +i /etc/passwd

## 5、vim没有权限保存修改

       :w !sudo tee %
## 6、git上传超过100M文件

Git LFS的官方网址： https://git-lfs.github.com/
*先安装 Git LFS 的客户端，然后在将要push的仓库里重新打开一个bash命令行： 
*只需设置1次 LFS : git lfs install
*跟踪你要push的大文件的文件或指定文件类型 git lfs track "*.pdf"
*以上已经设置完毕， 其余的工作就是按照正常的 add , commit , push 流程就可以了。
