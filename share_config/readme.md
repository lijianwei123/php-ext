主要目的是  方便管理一些跨服务器的配置   比如公用的数据库配置

配置项分两级,  存储在mysql中
     section_name
     item_name

开发思路:
	PHP扩展端：

		get_share_config("db_veryeast", "user");
		
		如果在php全局内存中找不到，就会请用源数据源，并写入全局内存中
	     

		del_share_config("db_veryeast", "user");
		删除共享配置  主要是有改动的时候，通过nginx调用这个函数，删除过时的配置
	

	nginx扩展端：
		
		写入一个模块，调用php中的del_share_config

	

	普通管理端：
		 
		 这个就比较简单了,用php实现就好了，



