
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
using namespace std;
#include <pthread.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sendfile.h>

const int MAX_EVENT=1024;
const int BUF_SIZE=128;
const int SIZE=128;
const int METHOD_SIZE=128;
const int PATH_SIZE=128;
const int QUERY_SIZE=128;
const int URL_SIZE=128;
#define SERVER_STRING "Server: jdbhttp/1.1\r\n"


#define DEBUG

//打印log
inline void print_log(const char *str)
{
	assert(str !=NULL);
#ifdef DEBUG
	cout<<str<<endl;
#else
	NULL;
#endif
}

int get_line(int sock,char *buf,int len)
{
	assert(buf !=NULL && len >0);
	int i=0;
	char ch='\0';
	int num=-1;
	while(i <len-1 && ch !='\n')
	{
		num=recv(sock,&ch,1,0);
		if(ch=='\0' || ch=='\n' || ch=='\r')
		{
			num=recv(sock,&ch,1,0);
			break;
		}
		if(num >0)
			buf[i++]=ch;
	}
	if(ch=='\0')
		buf[i++]='\0';
	else
	{
		buf[i++]='\n';
		buf[i]='\0';
	}
	return i;
}
int get_comm(const char *buf,char *str)
{
	int i=0;
	int j=0;
	while(buf[j]==' ')
		j++;
	while(buf[j] !=' ')
	{
		if(buf[j]=='\0')
			break;
		str[i++]=buf[j++];
	}
	return i;
}
int get_method(const char *buf,char *method)
{
	return get_comm(buf,method);
}
int get_url(const char *buf,char *url)
{
	return get_comm(buf,url);
}
void get_method_path_url(const char *buf,char *method,char *path,char *url,int &cgi)
{
	assert(buf);
	assert(method);
	assert(path);
	assert(url);
	int len=get_method(buf,method);

	len+=get_url(buf+len+1,url);

	if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
		return;//not fuond
	if(strcmp(method,"POST")==0)
		cgi=1;
	char *cur=url;
	if(strcmp(method,"GET")==0)
	{
		while(*cur !='?' && *cur !='\0')
			cur++;
		if(*cur=='?')
		{
			*cur='\0';
			cur++;
			cgi=1;
		}
		else
			NULL;
	}
	sprintf(path,"htdocs%s",url);
	if(path[strlen(path)-1]=='/')
		strcat(path,"index.html");
}
void send_header(int sock,char *path,int size)
{
	int fd=open(path,O_RDONLY);
	char buf[SIZE];
	print_log("header begin");
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	send(sock,buf,strlen(buf),0);
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,SERVER_STRING);
	send(sock,buf,strlen(buf),0);
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,"COntent-Type:text/html\r\n");
	send(sock,buf,strlen(buf),0);

	memset(buf,'\0',sizeof(buf));
	strcpy(buf,"\r\n");
	send(sock,buf,strlen(buf),0);
	if(sendfile(sock,fd,NULL,size) <0)
		print_log("sendfile failed");
	else
		print_log("sendfile success");
}
void exec_get_file(int sock,char *path,int size)
{
	char buf[SIZE];
	memset(buf,'\0',sizeof(buf));
	int num=1;
	while(num >0 && strcmp(buf,"\n"))
		num=get_line(sock,buf,SIZE);

	FILE *file=fopen(path,"r");
	if(NULL==file)
		print_log("fopen failed");
	else
		send_header(sock,path,size);
	fclose(file);
}
void exec_get(int sock,char *path)
{
	struct stat st;
	if(stat(path,&st)==-1)
		print_log("stat failed");
	else
	{
		if((st.st_mode & S_IFMT)== S_IFDIR)
		{
			print_log("st.st_mode success");
			strcat(path,"/index.html");
		}
		exec_get_file(sock,path,st.st_size);
	}
}
void* accept_request(void *argc)
{
	pthread_detach(pthread_self());
	int sock=(int)argc;
	char buf[SIZE];
	memset(buf,'\0',sizeof(buf));
	int n=0;
	int cgi=0;

	char method[METHOD_SIZE];
	memset(method,'\0',sizeof(method));

	char url[URL_SIZE];
	memset(url,'\0',sizeof(url));

	char path[PATH_SIZE];
	memset(path,'\0',sizeof(path));

	char query[QUERY_SIZE];
	memset(query,'\0',sizeof(query));

	if((n=get_line(sock,buf,sizeof(buf)) !=0))
	{
		//分别提取出方法、资源、路径、全路径
		get_method_path_url(buf,method,path,url,cgi);
		if(!cgi)
		{
			print_log("GET METHOD");
			exec_get(sock,path);
			close(sock);
		}else
		{
			print_log("POST METHOD");
			//exec_cgi(sock,method,path,query);
			close(sock);
		}
		memset(buf,'\0',sizeof(buf));
	}
}
//获取sock端口号
int get_sock(const char *ip,int port)
{
	print_log("get sock");
	struct sockaddr_in adderss;
	bzero(&adderss,sizeof(sockaddr_in));
	adderss.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&adderss.sin_addr);
	adderss.sin_port=htons(port);
	//创建sock
	int sock=socket(AF_INET,SOCK_STREAM,0);
	assert(sock >=0);
	//绑定端口
	int ret=bind(sock,(struct sockaddr*)&adderss,sizeof(struct sockaddr));
	assert(ret !=-1);
	//监听端口
	ret=listen(sock,5);
	assert(ret !=-1);
	return sock;
}
//设置sock为非阻塞版本
int setnonblocking(int sock)
{
	print_log("set no blocking");
	int old_option=fcntl(sock,F_GETFL);
	int new_option=old_option | O_NONBLOCK;
	fcntl(sock,F_SETFL,new_option);
	return old_option;
}
//添加触发事件
void addfd(int epoll_fd,int sock,bool flag)
{
	print_log("add fd event");
	epoll_event event;
	event.data.fd=sock;
	event.events=EPOLLIN;
	if(flag)
		event.events |=EPOLLET;
	//EPOLL_CTL_ADD ：添加事件
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sock,&event);
	//设置成非阻塞版本
	setnonblocking(sock);
}
//LT模式
void epoll_lt(epoll_event *events,int number,int epollfd,int listen_sock)
{
	print_log("add epoll LT");
	char buf[BUF_SIZE];
	static int connfd=0;
	for(int i=0; i<number; ++i)
	{
		int sockfd=events[i].data.fd;
		if(sockfd==listen_sock)
		{
			struct sockaddr_in client_address;
			socklen_t client_len=sizeof(client_address);
			print_log("ET begin accept!");
			connfd=accept(listen_sock,(struct sockaddr*)&client_address,&client_len);
			print_log("accept epoll LT");
			//对lt禁止用et模式
			addfd(epollfd,connfd,false);
		}else if(events[i].events & EPOLLIN)
		{
			//只要socket读缓存中还没有未读出的数据，这段代码被执行
			//事件只触发一次
			print_log("event trigger once");
			memset(buf,'\0',sizeof(BUF_SIZE));
			int ret=recv(sockfd,buf,BUF_SIZE,0);
			if(ret <=0)
			{
				//表示读缓存中的数据已经被读完
				print_log("ret < 0 epoll LT");
				close(sockfd);
				continue;
			}
			cout<<"get "<<ret<<" bytes of content : "<<buf<<endl;
		}else
		{
			cout<<"something else happend"<<endl;
		}
	}
}
//ET模式
void epoll_et(epoll_event *event,int number,int epollfd,int listen_sock)
{
	print_log("add epoll ET");
	char buf[BUF_SIZE];
	static int connfd=0;
	for(int i=0; i<number; ++i)
	{
		int sockfd=event[i].data.fd;
		if(sockfd ==listen_sock)
		{
			struct sockaddr_in client_address;
			socklen_t len=sizeof(client_address);
			print_log(" ET begin accept!");
			connfd=accept(sockfd,(struct sockaddr*)&client_address,&len);
			//cout<<"connfd :"<<connfd<<"sockfd :"<<sockfd<<endl;
			print_log("accept epoll ET");
			addfd(epollfd,connfd,true);
		}else if(event[i].events & EPOLLIN)
		{
#ifdef DEBUG
			pthread_t pid;
			if(pthread_create(&pid,NULL,accept_request,(void*)connfd) !=0)
			{
				print_log("create pthread failed");
				return;
			}
#else
			//因为ET模式是只触发一次，只要有数据触发
			//用while将缓存里的数据全部读出
			print_log("event trigger once");
			while(1)
			{
				memset(buf,'\0',sizeof(buf));
				int ret=recv(sockfd,buf,sizeof(buf)-1,0);
				if(ret <0)
				{
					//对于非阻塞的I/O，下面的条件成立表示数据已经全部被读出
					//此后，epoll就能再次触发sockfd上的EPOLLIN事件
					print_log("ret <0  epoll ET");
					if((errno ==EAGAIN) || (errno==EWOULDBLOCK))
					{
						print_log("read later");
						break;
					}
					close(sockfd);
					break;
				}else if(ret ==0)
				{
					print_log("ret==0 epoll ET");
					close(sockfd);
					break;
				}else
				{
					cout<<"get "<<ret<<" bytes of content : "<<buf<<endl;
				}
			}
#endif
		}else
		{
			cout<<"something else happend"<<endl;
		}
	}
}














