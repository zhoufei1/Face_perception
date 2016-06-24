
#include "server_http.h"

int main(int argc,char *argv[])
{
	if(argc < 2)
		return -1;

//	const char *str="8080";
//	int port=atoi(str);
	int port=atoi(argv[1]);
	int server_sock=GetSock(port);
	if(server_sock ==-1)
	{
		print_log("main get sock failed");
		return -1;
	}

	cout<<"server sock is : "<<server_sock<<"   port is :"<<port<<endl;
	struct sockaddr_in client_name;
	bzero(&client_name,sizeof(client_name));
	socklen_t len=sizeof(client_name);


	int count=1;
	while(1)
	{
		//等待client的链接
		cout<<" 【 第  "<<count++<<"  个client 链接请求 】"<<endl;
		int client_sock=accept(server_sock,(struct sockaddr*)&client_name,&len);
		if(client_sock==-1)
		{
			print_log("accept client sock failed");
			return -1;
		}
//		cout<<" 【 链接成功 】 "<<client_sock<<endl;
		//创建线程执行client 的请求
		//char buf[SIZE];
		//memset(buf,'\0',sizeof(buf));
		//recv(client_sock,buf,sizeof(buf),0);
		//cout<<"***** recv :"<<buf;
		pthread_t pid;
		if(pthread_create(&pid,NULL,accept_request,(void*)(client_sock)) !=0)
		{
			print_log("create pthread falied");
			return -1;
		}
	}
	pthread_mutex_destroy(&mutex);
	close(server_sock);
	return 0;
}
