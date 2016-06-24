
#pragma once

#include <iostream>
using namespace std;
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <pthread.h>

#define SERVER_STRING "Server: jdbhttp/1.1\r\n"

const int SIZE=1024;
const int INT_TO_STR_SIZE=255;
const int METHOD_SIZE=255;
const int URL_SIZE=512;
const int TREATY_SIZE=512;
const int PATH_SIZE=512;
const int QUERY_SIZE=512;

void print_log(const char *str)
{
	if(str !=NULL)
		cout<<str<<endl;
}




