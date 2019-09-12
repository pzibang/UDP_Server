# UDP_Server
/*
  *第一次提交的代码是存在bug的,下面的代码让我感到疑惑，在整个代码中，出现配置IP的地方就只有这里，但是
  *如果是服务器能够接受任意客户端访问的时候我们可以理解，这个IP是服务器的IP地址；另外一种情况是，我们要指定
  *客户端的地址才能进行访问，我找了许多博客，或者文章，发现基本的UDP服务器都是以以下下的形式进行初始化的：
	sockaddr_in serverAddr;										          //声明客户端地址信息结构体
  memset(&serverAddr, 0, sizeof(serverAddr));					//清空
  
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(CLIENT_PORT);					  //端口
	//serverAddr.sin_addr.s_addr =htonl(INADDR_ANY);			//任意客户端连接
	serverAddr.sin_addr.s_addr =  inet_addr("192.168.234.1");	//本机IP
   //绑定套接字和地址
	   if(bind(sock, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("bind error\n");
		return -1;
	}

	//接收客户端请求
	while(1)
	{
		sockaddr_in clntAddr;  //客户端地址信息
		memset(&clntAddr, 0, sizeof(sockaddr_in));	//清除客户端信息
		int nSize = sizeof(clntAddr);

		strLen = recvfrom(sock, revBuffer,  BUF_SIZE, 0,(struct sockaddr*) &clntAddr, &nSize);
		if(strLen<0)
		{
			perror("recvfrom");
			return 3;
		}
		else
		{
			revBuffer[strLen] = 0;
			printf("#server echo#  %s\n",revBuffer);	//打印接收的内容
			if(strcmp(revBuffer,"quit") == 0)			//接收到"quit"字符串退出程序
				break;
		}
	}
  
  *但是这样的初始化并不是规范的serverAddr和clntAddr两个sockaddr_in类型的结构体是用来存放server和client的信息的，
  *如果想要server接收指定的client地址的时候，这个地址又在哪里设置？这样总是让人心生疑惑，而产生这样疑惑的原因还是对网络
  *底层结构缺乏了解导致的，这个时候我明白了，UDP通信的时候，这个客户端的IP在初始化的时候是不需要的，而且初始化
  *也没有效果：
  sockaddr_in serverAddr;										//声明客户端地址信息结构体
	memset(&serverAddr, 0, sizeof(serverAddr));					//清空

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(CLIENT_PORT);					//端口
	serverAddr.sin_addr.s_addr =  inet_addr("192.168.234.1");	//本机IP
		
	sockaddr_in clntAddr;										          //客户端地址信息
	clntAddr.sin_port = htons(CLIENT_PORT);						//端口
	clntAddr.sin_addr.s_addr =  inet_addr("192.168.234.12");	//客户端IP
	int nSize = sizeof(clntAddr);
  
  *我想通过以上操作到达server接收指定IP的client，但即使这样初始化也是没有效果的，任意的UDP客户端都能进行访问这个服务器，而
  *当不同的客户端进行连接的时候，这个IP都会变成当前的IP。
  
    sockaddr_in serverAddr;										//声明客户端地址信息结构体
	memset(&serverAddr, 0, sizeof(serverAddr));					//清空

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(CLIENT_PORT);					//端口
	serverAddr.sin_addr.s_addr =  inet_addr("192.168.234.1");	//本机IP
		
	sockaddr_in clntAddr;										//客户端地址信息
	clntAddr.sin_port = htons(CLIENT_PORT);						//端口
	//clntAddr.sin_addr.s_addr =  inet_addr("192.168.234.12");	//客户端IP
	clntAddr.sin_addr.s_addr =htonl(INADDR_ANY);			//地址
	int nSize = sizeof(clntAddr);


	//绑定套接字和地址
	if(bind(sock, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("bind error\n");
		return -1;
	}

	//接收客户端请求
	while(1)
	{
		strLen = recvfrom(sock, revBuffer,  BUF_SIZE, 0,(struct sockaddr*) &clntAddr, &nSize);
		if(0 == strcmp(inet_ntoa(clntAddr.sin_addr), "192.168.234.12"))
		{			
			if(strLen<0)
			{
				perror("recvfrom");
				return 3;
			}
			else if(strLen == 0)
			{
				printf("server closed\n");
			}
			else
			{
				revBuffer[strLen] = 0;
				printf(" received from %s   :  %s\n", inet_ntoa(clntAddr.sin_addr),revBuffer);	//打印接收的内容

				if(strcmp(revBuffer,"quit") == 0)			//接收到"quit"字符串退出程序
					break;
			}
		}
		else
		{
			printf("received %s is not destination IP\n", inet_ntoa(clntAddr.sin_addr));
			break;
		}

	}

  *目前只想到通过判断IP的方式处理非指定IP



*/
