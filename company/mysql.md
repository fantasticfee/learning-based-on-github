# mysql外键设置失败原因：
  1.  两个字段的类型或者大小不严格匹配。例如，如果一个是int(10)，那么外键也必须设置成int(10)，而不是int(11)，也不能是tinyint。另外，你还必须确定两个字段是否一个为 signed，而另一个又是unsigned（即：无符号），这两字段必须严格地一致匹配，更多关于signed和unsigned的信息，请参阅：http://www.verysimple.com/blog/?p=57
  2. 试图设置外键的字段没有建立起索引，或者不是一个primary key（主键）。如果其中一个不是primary key的话，你必须先为它创建一个索引。
  3. 其中一个或者两个表是MyISAM引擎的表。若想要使用外键约束，表必须是InnoDB引擎（实际上，如果两个表都是MyISAM 引擎的，这个错误根本不会发生，但也不会产生外键，只会建立索引）你需要检查表的引擎类型。
  4. 外键的名字不能重复。你应该检查你的数据库以确保外健名字是唯一的，或者你在键名后面加上几个随机的字符以测试是否是这个原因。
  5. 你可能设置了ON DELETE SET NULL，但是相关的键的字段又设置成了NOTS NULL值。你可能通过修改cascade的属性值或者把字段属性设置成allow null来解决。
  6. 请确定你的Charset和Collate选项在表级和字段级上的一致。
  7. 你可能设置为外键设置了一个默认值，如default=0。
  8. ALTER声明中有语法错误
 
  转载：https://blog.csdn.net/wangpeng047/article/details/19624351 
  
  ## 1、mysql不设置密码
  ```
  注意在需要同版本MySQL之间迁移数据的情况下，使用之间拷贝目录的方式，需要记住当前数据库的root密码，因为root密码是写进数据里面的
  mysql的后缀为cnf的配置文件配置如下：
      [mysqld]
    skip-host-cache(可以忽略）
    skip-name-resolve（可以忽略）
    skip-grant-tables
  ```
  
  ## 2、mysql 5.7 修改root密码
  ```
  update user set authentication_string=password('a@123') where user='root';
  update user set password=password(“123”) where user=”root”;
  ```
  
  ## 3、wordpress(包括mariadb）迁移数据
 ```
 原以为直接把数据目录拷贝过来就可以，没想到出错了，报错现象，mariadb直接无法启动，日志报错：
 table xxxxx doesn't exist in engine
 将ib_logfile*文件移出数据目录，可以正常启动，并自动生成ib_logfile*文件，可以推测ibdata1文件是
 真正的数据文件；
 wordpress前端界面无法修改文件，报错wordpress.table_name not exist,
 使用mysqlcheck -u root -p database_name;发现部分table可以部分不行，区别是文件权限不同，
 需要进入mariadb数据wordpress目录，对想以table文件从原来的-rw-r-r--修改为 -rw-rw-r--，即执行
 chmod 660 table文件；然后重新启动pod就可以正常运行了
 ```
