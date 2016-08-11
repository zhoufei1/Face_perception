
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

const int SIZE=1024;

inline void print_log(const char *str)
{
	assert(str);
	cout<<str<<endl;
}
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
