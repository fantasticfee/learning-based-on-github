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
