
#include "../comm/comm.h"

int  get_str(char *buf,int size)
{
	assert(buf !=NULL && size >0);
	cout<<"please enter : ";
	fgets(buf,size,stdin);
	int len=strlen(buf);
	buf[len-1]='\0';
	return len;
}
int get_sock(const char *ip,short int port,struct sockaddr_in &client)
{
	bzero(&client,sizeof(client));
	client.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&client.sin_addr);
	client.sin_port=htons(port);

	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock <0)
	{
		print_log("create sock failed");
		return -1;
	}
	return sock;
}
