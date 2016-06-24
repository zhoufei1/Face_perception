#include "comm/comm.h"

#include <string>
#include <time.h>

int main()
{
	char *str="zhoufei";
	string pstr=str;
	string tmp;

	tmp+=pstr;
	cout<<pstr<<endl;
	cout<<tmp<<endl;
	return 0;
}

/*
struct node
{
	int data;
	node(int k):data(k)
	{}
};

int main()
{
	vector<node> v;
	node *p=new node(10);
	v.push_back(*p);
	delete p;
	cout<<v[0].data<<endl;
	return 0;
}
/*
int main()
{
	char str[1024];
	fgets(str,sizeof(str),0);
	//fgets(str,sizeof(str),stdin);
	cout<<"get str :"<<str<<endl;
	return 0;
}
/*
int main()
{
	string name;
	const char *str="abcd";
	name=str;
	//strcpy(name.c_str(),str);
	cout<<name<<endl;
	return 0;
}
/*
int main()
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	fgets(buf,sizeof(buf),stdin);
	int i=0;

	while(buf[i] !='\n')
	{
		cout<<buf[i++]<<endl;
	}
	cout<<"OVER"<<endl;
	return 0;
}
/*
int main()
{
	char str[]="abcd";
	cout<<*(str+3)<<endl;
	char *p=str+2;
	cout<<*p<<endl;
	return 0;
}
/*
void int_to_string(int key,char *str)
{
	assert(str !=NULL);
	cout<<"int_to str key :"<<key<<endl;
	sprintf(str,"%d",key);
}
int main()
{
	double a=3.6;
	int b=9;
	int c=a+b;
	char str[3];
	int key=12;
	string t;
	string flag=":";
	memset(str,'\0',sizeof(str));
	int_to_string(key,str);
	printf("%s\n",str);
	t+=str;
	cout<<"1 t is "<<t<<endl;
	t+=flag;
	cout<<"2 t is "<<t<<endl;
//	cout<<c<<endl;
	return 0;
}

/*
//char *get_str()
string get_time(struct tm *&get_t)
{
	char tmp[1024];
	//string k;
	memset(tmp,'\0',sizeof(tmp));
	time_t t=time(NULL);
	//strftime((&k.c_str()),sizeof(k.c_str()),"%Y.%m.%d  %A 第 %j天",localtime(&t));
	strftime(tmp,sizeof(tmp),"%Y.%m.%d %X %A",localtime(&t));
	get_t=localtime(&t);
	//string str=tmp;
//	cout<<tmp<<endl;
	//return str;
	return tmp;
}
time_t get_time1(const char *str)
{
	struct tm t;
	sscanf(str,"%d%d%d%d%d%d%d",&(t.tm_year),&(t.tm_mon),\
			&(t.tm_mday),&(t.tm_hour),&(t.tm_min),&(t.tm_sec),&(t.tm_wday));
	t.tm_year-=1900;
	t.tm_mon--;
	t.tm_isdst=-1;
	//printf("%d%d%d%d%d%d%d%d",t.tm_year,t.tm_mon,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec,t.tm_wday);
	cout<<"***************"<<endl;
	time_t tmp=mktime(&t);
	return tmp;
}
void  str_to_time(const string &str)
{
	const char *tmp=str.c_str();
	//time_t t=get_time(tmp);
//	cout<<"time is :"<<t<<endl;
//	return t;
}
int main()
{
	struct tm *p;
	time_t start=time_t();
	time_t end=0;
	string str=get_time(p);
	cout<<"str is : "<<str<<endl;
	start=mktime(p);
	//str_to_time(str);
	sleep(5);
	string key=get_time(p);
	cout<<"key is : "<<key<<endl;
	end=mktime(p);
	//str_to_time(key);
	double s=difftime(end,start);
	cout<<"s is :"<<s<<endl;
//	char *tmp=get_str();
//	cout<<tmp<<endl;
	return 0;
}










/*
		void get_string(const char *buf,int n,string &name,string &id)
		{
			assert(buf !=NULL && n>0);
			char *str=new char[n+1];
			char *cur=str;
			while(*buf !='\0')
			{
				if(*buf==' ' || *(buf+1)=='\0')
				{
					if(*(buf+1)=='\0')
					{
						*cur=*buf;
						*(cur+1)='\0';
						id=str;
						return ;
					}
					*cur='\0';
					name=str;//
					cur=str;
					buf++;
				}
				*cur++=*buf++;
			}
			delete []str;
		}
void get_string1(string &name,string &id)
{
	char *str=new char[1024];
	cin>>str;
	name=str;
	cout<<"*****"<<endl;
	char *p=str;
	cin>>p;
	id=p;
	//delete []str;
}
int main()
{
	string name;
	string id;

	const char *buf="zhoufei 130612";
	int len=strlen(buf);
	get_string(buf,len,name,id);
//	get_string(name,id);
	cout<<name<<"\n"<<id<<endl;
	//time_t a,b;
	//cout<<"please enter a :";
	//getchar();

	//a=time(NULL);
	//sleep(3);
	//cout<<"sleep 3 s please enter a :";
	//b=time(NULL);

	//cout<<"计时器时间："<<difftime(b,a)<<endl;
	return 0;
}


/*
int get_str(char *buf,int size)
{
	assert(buf !=NULL && size >0);
	cout<<"please enter :";
	fgets(buf,SIZE,stdin);
	int len=strlen(buf);
	buf[len-1]='\0';
	return len-1;
}
int main()
{
	string st;
	//cin>>st;
	//fgets(st.c_str(),SIZE,stdin);
	//cout<<st<<endl;
	//const char *str="abcd";
	//cout<<"str len is :"<<strlen(str)<<endl;
	char buf[SIZE];
	//memset(buf,'\0',sizeof(buf));
	int len=get_str(buf,SIZE);
	cout<<"len is : "<<len<<endl;
	cout<<"buf is : "<<buf<<endl;
	cout<<"*************"<<endl;
	return 0;
}*/
