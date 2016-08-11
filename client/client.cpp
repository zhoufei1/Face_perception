
#include "client.h"


int main(int argc,char *argv[])
{
	if(argc <3)
	{
		print_log("argc < 3");
		return -1;
	}
	const char *ip=argv[1];
	short int port=atoi(argv[2]);
	cout<<"ip is :"<<ip<<"  port is :"<<port<<endl;

	struct sockaddr_in client;
	int sock=get_sock(ip,port,client);
	if(-1==sock)
	{
		print_log("sock is -1");
		return -1;
	}
	cout<<"sock success :"<<sock<<endl;
	while(1)
	{
		int flag=connect(sock,(struct sockaddr*)&client,sizeof(client));
		if(flag <0)
		{
			print_log("sock connect failed");
			close(sock);
			return -1;
		}//else
		while(1)  //当用while时，server端也需要用while
		{
			char buf[SIZE];
			memset(buf,'\0',sizeof(buf));

			int len=get_str(buf,sizeof(buf)-1);
			cout<<"client len is : "<<len<<"   cleint :"<<buf<<endl;
			if(len <0 || len >=SIZE)
			{
				print_log("fget failed");
				return -1;
			}
			if(strncasecmp(buf,"quit",4)==0)
			{
				cout<<"本次链接断开"<<endl;
				break;
			}
			send(sock,buf,len,0);
		}
	}
	return 0;
}








