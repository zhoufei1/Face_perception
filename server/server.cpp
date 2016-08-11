
#include "server.h"


int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		print_log("please enter ip port");
		exit(1);
	}

	const char *ip=argv[1];
	int port=atoi(argv[2]);
	int sock=get_sock(ip,port);

	epoll_event events[MAX_EVENT];
	//需不需要初始化
	int epoll_fd=epoll_create(5);
	assert(epoll_fd !=-1);
	addfd(epoll_fd,sock,true);
	while(1)
	{
		int ret=epoll_wait(epoll_fd,events,MAX_EVENT,-1);
		if(ret <0)
		{
			print_log("epoll wait failed");
			break;
		}
		print_log("epoll wait success");
//#ifdef DEBUG
		epoll_et(events,ret,epoll_fd,sock);
//#else
		//epoll_lt(events,ret,epoll_fd,sock);
//#endif
	}
	close(sock);
	return 0;
}
