
#include <iostream>
using namespace std;
#include <string>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
const int SIZE=1024;

void recv_str(const char *str)//val=zhoufei&val2=123
{
	assert(str !=NULL);
	char *cur=(char*)str;
	char *name=NULL;
	char *id=NULL;
	while(*cur !='\0')
	{
		if(*cur=='=' && name==NULL)
		{
			cur++;
			name=cur;
			continue;
		}
		if(*cur=='&')
			*cur='\0';
		if(*cur=='=' && name !=NULL)
		{
			id=cur+1;
			break;
		}
		cur++;
	}
	cout<<"<p> name is :%s </p>"<<name<<endl;
	cout<<"<p> id   is :%s </p>"<<id<<endl;
}
int main()
{
	
//	string name=getenv("REQUEST_METHOD");
//	cout<<name<<endl;
	char method[SIZE];
	char query_str[SIZE];
	char data_str[SIZE];
	memset(method,'\0',sizeof(method));
	memset(query_str,'\0',sizeof(query_str));
	memset(data_str,'\0',sizeof(data_str));

	cout<<"<html>"<<endl;
	cout<<"<head> come on show world </head>"<<endl;
	cout<<"<body>"<<endl;

	strcpy(method,getenv("REQUEST_STRING"));
	if(strcasecmp(method,"GET")==0)
	{
		strcpy(query_str,getenv("QUERY_STRING"));
		recv_str(query_str);
	}else if(strcasecmp(method,"POST")==0)
	{
		int content_len=atoi(getenv("CONTENT_LENGTH"));
		int i=0;
		for(; i<content_len; ++i)
			read(0,&data_str[i],1);
		data_str[i]='\0';
		recv_str(data_str);
	}else
		cout<<"error"<<endl;

	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;

	return 0;
}



