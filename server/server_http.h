

//没有处理异常情况
#include "../comm/comm.h"
#include "../mysql/mysql.h"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

struct user_node
{
	string _name; //姓名
	string _id;   //编号
	string arrive_time; //签到时间
	string leave_time; //离开时间
	int sec;   //秒
	int min;   //分
	int hour;  //时
	double data; //天
	time_t  start_time;  //记录签到时间的time
	time_t  end_time;    //记录离开时间的time，为了算时间差
	user_node()
	{}
	user_node(const string &name,const string &id,const string &ar_time,\
			const string lev_time,double d=0.0)
		:_name(name),_id(id)
		 ,arrive_time(ar_time) ,leave_time(lev_time)
		 ,sec(0),min(0),hour(0),data(d)
		 ,start_time(time_t()),end_time(time_t())
	{}
};

class server_http
{
	private:
		size_t size;
		vector<user_node > v;
	public:
		server_http(size_t sz=0)
			:size(sz)
		{}
		void show()
		{
			cout<<"姓名\t编号\t签到时间\t\t\t离开时间\t\t\t计时间"<<endl;
			vector<user_node>::iterator cur=v.begin();
			for(; cur !=v.end(); ++cur)
			{
				cout<<cur->_name<<"\t"<<cur->_id<<"\t";
				cout<<cur->arrive_time<<"\t"<<cur->leave_time;
				cout<<"\t"<<cur->data<<"天"<<cur->hour<<"时";
				cout<<cur->min<<"分"<<cur->sec<<"秒"<<endl;
			}
		}
	private:
		//将字符串中姓名和编号分离出来
		void get_string(const char *query,int n,string &name,string &id)
		{
			cout<<"get string begin :"<<query<<endl;
			assert(query !=NULL && n>0);
			char *tmp=new char[n+1];
			int i=0;
			memset(tmp,'\0',sizeof(tmp));

			pthread_mutex_lock(&mutex);
			while(*query++ !='=' && *query !='\0')
				NULL;
			while(*query !='&' && *query !='\0')
				tmp[i++]=*query++;
			pthread_mutex_unlock(&mutex);

			tmp[i]='\0';
			name=tmp;
			pthread_mutex_lock(&mutex);
			while(*query++ !='=' && *query !='\0')
				NULL;
			pthread_mutex_unlock(&mutex);
			id=query;
			cout<<"name:"<<name<<"  id:"<<id<<endl;
			delete []tmp;
		}
	protected:
		//得到当前时间
		string get_time(struct tm *&T)
		{
			char  buf[SIZE];
			memset(buf,'\0',sizeof(buf));
			time_t _t=time(NULL);
		
			pthread_mutex_lock(&mutex);
			strftime(buf,sizeof(buf),"%Y.%m.%d %X %A",(T=localtime(&_t)));
			pthread_mutex_unlock(&mutex);
			return buf;
		}
		//查找id，为了在插入的时候进行不重复插入
		vector<user_node>::iterator find_id(user_node *head)
		{
			vector<user_node>::iterator cur=v.begin();
			while(cur !=v.end())
			{
				if(!strcmp((cur->_id).c_str(),head->_id.c_str()) &&\
						!strcmp((cur->_name).c_str(),(head->_name).c_str()))
				{
					//cout<<"已经存在:"<<cur->_id<<"  "<<cur->_name<<endl;
					return cur;
				}
				cur++;
			}
			pthread_mutex_lock(&mutex);
			size++;
			pthread_mutex_unlock(&mutex);
			return v.end();
		}
		//计算出上班时间函数
		void calculat_work_time(vector<user_node>::iterator cur)
		{
			cur->sec+=difftime(cur->end_time,cur->start_time);//计算时间差函数
			//cout<<"cur sec :"<<cur->sec<<endl;
			while(cur->sec >=60)
			{
				cur->min+=1;
				if(cur->min >=60)
				{
					cur->hour+=1;
					cur->min-=60;
				}
				if(cur->hour >=8)
				{
					cur->data+=1;
					cur->hour-=8;
				}
				cur->sec-=60;
			}
		}
	private:
		//将整数转化成字符串
		void int_to_str(int key,char *buf)
		{
			assert(buf !=NULL);
			sprintf(buf,"%d",key);
		}
		string int_to_string(vector<user_node>::iterator prev)
		{
			vector<user_node>::iterator cur=prev;

			char buf[INT_TO_STR_SIZE];
			memset(buf,'\0',sizeof(buf));
			//分别对 天、时、分、秒进行整数转化成字符串
			//所以只需要进行四次调用int_to_str进搞定
			string tmp;
			string flag=":";
			int_to_str((int)cur->data,buf);
		
			pthread_mutex_lock(&mutex);
			tmp+=buf;
			tmp+=flag;
			pthread_mutex_unlock(&mutex);
			
			memset(buf,'\0',sizeof(buf));
			int_to_str(cur->hour,buf);
			
			pthread_mutex_lock(&mutex);
			tmp+=buf;
			tmp+=flag;
			pthread_mutex_unlock(&mutex);
			
			memset(buf,'\0',sizeof(buf));
			int_to_str(cur->min,buf);
			
			pthread_mutex_lock(&mutex);
			tmp+=buf;
			tmp+=flag;
			pthread_mutex_unlock(&mutex);
			
			memset(buf,'\0',sizeof(buf));
			int_to_str(cur->sec,buf);
			
			pthread_mutex_lock(&mutex);
			tmp+=buf;
			pthread_mutex_unlock(&mutex);
			return tmp;
		}
		string get_total_string(vector<user_node>::iterator cur)
		{
			string tmp;
			string flag=" ";
			pthread_mutex_lock(&mutex);
			tmp+=cur->_name;
			tmp+=flag;
			tmp+=cur->_id;
			tmp+=flag;
			tmp+=cur->arrive_time;
			tmp+=flag;
			tmp+=cur->leave_time;
			tmp+=flag;
			pthread_mutex_unlock(&mutex);
			return tmp;
		}
	private:
		int get_line(int sock,char *buf,int size)
		{
			assert(buf !=NULL && size >0);
			int i=0;
			char ch='\0';
			int num=-1;
			while(i <size-1 && ch !='\n')
			{
				 num=recv(sock,&ch,1,0);
				 if(ch=='\0' || ch=='\n' || ch=='\r')
				 {
					 //当ch=='\r'的时候，就必须再读一次，不然最后就有两个\n
				 	num=recv(sock,&ch,1,0);
					//cout<<"get line ch is :"<<ch;
					 break;
				 }
				if(num >0)
				{
					buf[i++]=ch;
				}
			}
			if(ch=='\0')
				buf[i]='\0';
			else
			{
				buf[i++]='\n';
				buf[i]='\0';
			}
		//	cout<<"get line :"<<buf;
			return i;
		}
	private:
		void send_header(int client,const char *path,size_t size)
		{
			int fd=open(path,O_RDONLY);
			char buf[SIZE];
			cout<<"header"<<endl;
			
			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"HTTP/1.0 200 OK\r\n");
			send(client,buf,strlen(buf),0);

			memset(buf,'\0',sizeof(buf));
			strcpy(buf,SERVER_STRING);
			send(client,buf,strlen(buf),0);

			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"Content-Type:text/html\r\n");
			send(client,buf,strlen(buf),0);

			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"\r\n");
			send(client,buf,strlen(buf),0);

			if(sendfile(client,fd,NULL,size)< 0)
				cout<<"sendfile failed"<<endl;
			else
				cout<<"sendfile success"<<endl;
		}
		void exec_get_file(int client,const char *path,size_t size)
		{
			char buf[SIZE];
			memset(buf,'\0',sizeof(buf));
			int num=1;
			//recv(client,buf,sizeof(buf),0);
			//cout<<"***** recv :"<<buf;
			while(num >0 && strcmp(buf,"\n"))
			{
				num=get_line(client,buf,sizeof(buf));
			//	cout<<" while get line :"<<buf;
			}
			FILE *file=fopen(path,"r");
			if(file==NULL)
				cout<<"file fopen filed"<<endl;
			else
			{
				send_header(client,path,size);
			}
			fclose(file);
		}
	private:
		int get_comm(const char *buf,char *str)
		{
			assert(str !=NULL);
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
		int get_treaty(const char *buf,char *treaty)
		{
			return get_comm(buf,treaty);
		}
		//提取出各个方法和资源
		void get_path_method_query(const char *buf,int *cgi,char *path,\
				char *method,char *url,char *query,char *treaty)
		{
			assert(buf !=NULL);
			assert(path !=NULL);
			assert(method !=NULL);
			assert(url !=NULL);
			assert(query !=NULL);
			assert(treaty !=NULL);
			
			int len=get_method(buf,method);
			//cout<<"method len :"<<len<<endl;

			len+=get_url(buf+len+1,url);
			//cout<<"url len :"<<len<<endl;

			len+=get_treaty(buf+len+1,treaty);
			//cout<<"treaty len :"<<len<<endl;
	
			if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
				;//not_find_method();
			if(strcmp(method,"POST")==0)
				*cgi=1;
			char *cur=url;
			if(strcmp(method,"GET")==0)
			{
			//	cout<<"strcmp method is GET"<<endl;
				while(*cur !='?' && *cur !='\0')
					cur++;
				if(*cur=='?')
				{
					*cur='\0';
					cur++;
					*cgi=1;
				}
			}
			strcpy(query,cur);
			sprintf(path,"htdocs%s",url);
			if(path[strlen(path)-1]=='/')
			{
				strcat(path,"index.html");
			}

			show_resource(method,url,treaty);
			cout<<"query :"<<query<<endl;
			cout<<"path :"<<path<<endl;
		}
		void exec_get(int client,char *path)//解析资源
		{
			struct stat st;
			if(stat(path,&st)==-1)
			{
				cout<<"stat failed"<<endl;
				///////////
			}else
			{
				//cout<<"stat success"<<endl;
				if((st.st_mode & S_IFMT)==S_IFDIR)
				{
					cout<<"st.st_mode success"<<endl;
					strcat(path,"/index.html");
				}
				exec_get_file(client,path,st.st_size);
				//cout<<"OVER"<<endl;
			}
			//show_resource(method,url,treaty);
		}
		void show_resource(const char *method,const char *url,const char *treaty)
		{
			cout<<"method : "<<method<<endl;
			cout<<"url    : "<<url<<endl;
			cout<<"treaty : "<<treaty<<endl;
		}
	private:
		void clear_head(int client)
		{
			char buf[SIZE];
			memset(buf,'\0',sizeof(buf));
			int num=0;
			do
			{
				num=get_line(client,buf,sizeof(buf));
			}while(num >0 && strcasecmp(buf,"\n") !=0);
		}
		void exec_cgi(int client,const char *path,const char *method,const char *query)
		{
			cout<<"exec cgi [methos]:"<<method<<"  [path]:"<<path<<"  [query]:"<<query<<endl;
			char buf[SIZE];
			int numchar=0;
			int content_length=0;
			//定义两个管道pipe
			int cgi_input[2]={0,0};
			int cgi_output[2]={0,0};

			//首先对方法判断
			if(strcmp(method,"GET")==0)
			{
				clear_head(client);
				cout<<" [ clear header success ]"<<endl;
			}
			else//POST
			{
				const char *str="Content-Length:";
				int len=strlen(str);
				do
				{
					memset(buf,'\0',sizeof(buf));
					numchar=get_line(client,buf,sizeof(buf));
					if(strncmp(buf,str,len)==0)
						content_length=atoi(&buf[len+1]);
				}while(numchar >0 && strcmp(buf,"\n"));
				cout<<"content lenght is :"<<content_length<<endl;
				if(content_length==-1)
				{
					cout<<"content length is -1"<<endl;
					return ;
				}
			}
			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"HTTP/1.1 200 OK\r\n\r\n");
			send(client,buf,strlen(buf),0);
			
			memset(buf,'\0',sizeof(buf));
			strcpy(buf,SERVER_STRING);
			send(client,buf,strlen(buf),0);

			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"Content-Type:text/html\r\n");
			send(client,buf,strlen(buf),0);

			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"\r\n");
			send(client,buf,strlen(buf),0);

			cout<<" [ send success ]"<<endl;
			//先创建两个管道
			if(pipe(cgi_input)==-1)
			{
				cout<<"pipe cgi input failed"<<endl;
			}
			if(pipe(cgi_output)==-1)
			{
				close(cgi_input[0]);
				close(cgi_input[1]);
				cout<<"pipe cgi output failed"<<endl;
			}
			cout<<"[ pipe create success ]"<<endl;
			//创建进程fork
			pid_t id=fork();
			if(id <0)
			{
				close(cgi_input[0]);
				close(cgi_input[1]);
				close(cgi_output[0]);
				close(cgi_output[1]);
				exit(1);
			}else if(id==0)//child
			{
				cout<<" [ enter child ]"<<endl;
				//首先关闭cgi_input写端，再关闭cgi_output读端
				close(cgi_input[1]);
				close(cgi_output[0]);

				//将标准读重定向到cgi_input读端
				dup2(cgi_input[0],0);
				//将标准输出重定向到cgi_output写端
				dup2(cgi_output[1],1);

				char query_env[QUERY_SIZE];
				memset(query_env,'\0',sizeof(query_env));
				
				char method_env[METHOD_SIZE];
				memset(method_env,'\0',sizeof(method_env));
				
				char content_len_env[METHOD_SIZE];
				memset(content_len_env,'\0',sizeof(content_len_env));

				sprintf(method_env,"REQUEST_METHOD:%s",method);
				putenv(method_env);
		//		cout<<"method env:"<<method_env<<endl;

				if(strcmp(method,"GET")==0)
				{
					sprintf(query_env,"QUERY_STRING:%s",query);
					putenv(query_env);
					cout<<" GET  query env :"<<query_env<<endl;
				}else//POST
				{
					sprintf(content_len_env,"CONTENT-LENGTH:%d",content_length);
					putenv(content_len_env);
					cout<<" POST  content len env :"<<content_len_env<<endl;
				}
				execl(path,path,NULL);
				exit(1);
			}else//father
			{
				cout<<" [ enter father ] "<<endl;
				close(cgi_input[0]);
				close(cgi_output[1]);

				char ch;
				//如果是POST方法分时候需要处理
				//如果是GET时，直接进行读取和发送
				if(strcasecmp(method,"POST")==0)
				{
					cout<<"POST METHOD begin..."<<endl;
					for(int i=0; i<content_length; ++i)
					{
						recv(client,&ch,1,0);
						write(cgi_input[1],&ch,1);
					}
					cout<<"POST METHOD end..."<<endl;
				}
				while(read(cgi_output[0],&ch,1)>0)
					send(client,&ch,1,0);
				cout<<"[ read and send success ]"<<endl;
				close(cgi_input[1]);
				close(cgi_output[0]);

				//防止僵尸进程的产生
				waitpid(id,NULL,0);
			}
		}
		void exec_post(int client,const char *path,const char *method,const char *query)
		{
			//先把姓名和卡号用vector数组保持
			cout<<"exec post [methos]:"<<method<<"  [path]:"<<path<<"  [query]:"<<query<<endl;
			//第一步，先将数据保存到vector，然后在保存链接数据库，将数据送如数据库中
			send_vector(query);
			//第二步，执行cgi函数
			exec_cgi(client,path,method,query);
		}
		void send_vector(const char *query)
		{
			cout<<"send vector begin :"<<query<<endl;
			string name;
			string id;
			int len=strlen(query);
			cout<<"len is :"<<len<<endl;
			//提取出姓名和卡号
			get_string(query,len,name,id);
			
			cout<<"get struct tm will begin"<<endl;
			struct tm *T;
			string local_time=get_time(T);
			user_node *tmp=buynode(name,id);
			
			//cout<<"tmp :"<<tmp->_name<<" "<<tmp->_id<<endl;
			vector<user_node>::iterator prev=find_id(tmp);
			//注释：加锁的时候一定要注意，如果内部函数已经加了锁
			//      那么外面调用它的函数一定不要再加锁
			//      如果再加很有可能造成死锁
			//cout<<"find name:"<<prev->_name<<endl;
			if(prev !=v.end())//已经存在
			{
				//pthread_mutex_lock(&mutex);
				
				prev->end_time=mktime(T);
				//因为之前签到已经做了标记，
				//所以只要再一次签到将被当成是离开时间记录
				prev->leave_time=local_time;

				//cout<<"prev->leave_time: "<<prev->leave_time<<endl;
				calculat_work_time(prev);//计算出上班时间函数
				string total_string=get_total_string(prev);

				//total_time是计算的时间差
				string total_time=int_to_string(prev);
				total_string+=total_time;
				//pthread_mutex_unlock(&mutex);

			//	prev->total_str=total_string;
				cout<<"**************"<<endl;
				cout<<"total :"<<total_string<<endl;
				cout<<"**************"<<endl;
				//定义mysql数据库的临时变量，将数据插入到数据库中
				//Mysql my;
				pthread_mutex_lock(&mutex);
				//if(my.inter_link())
				//	if(my.insert(prev->_name,prev->_id,\
								prev->arrive_time,\
								prev->leave_time,total_time));
				pthread_mutex_unlock(&mutex);
				//cout<<"mysql insert success"<<endl;
				//cout<<"prev_total :"<<prev->total_str<<endl;
			}else
			{
				pthread_mutex_lock(&mutex);
				//记录当前的time_t 为了在计算时间差的时候用
				tmp->start_time=mktime(T);
				//记录签到时间
				tmp->arrive_time=local_time;
				//将数据存入vector数组中
				v.push_back(*tmp);
				pthread_mutex_unlock(&mutex);
				delete tmp;
			}
			show();
		}
	public:
		//2线程处理请求
		void Accept(int server_sock)
		{
			char buf[SIZE];
			memset(buf,'\0',sizeof(buf));
			int n=-1;
			int cgi=0;
			int sock=server_sock;
			
			char method[METHOD_SIZE];//请求方法
			memset(method,'\0',sizeof(method));
			
			char url[URL_SIZE];//资源
			memset(url,'\0',sizeof(url));
			
			char path[PATH_SIZE];//路径
			memset(path,'\0',sizeof(path));
			
			char query[QUERY_SIZE];//路径
			memset(query,'\0',sizeof(query));

			char treaty[TREATY_SIZE];//协议  
			memset(treaty,'\0',sizeof(treaty));
			
			//while((n=recv(sock,buf,SIZE,0)) !=-1)
			////读出整个浏览器中所有字符（包括：头，资源，主机，协议号，还有后面等等的所有资源）
			//cout<<"线程处理请求"<<endl;
			//当用while时，对应client也需要用while
			//while((n=get_line(sock,buf,sizeof(buf))) !=-1)//只读出第一行，后面的消息不管
			if((n=get_line(sock,buf,sizeof(buf))) !=-1)//只读出第一行，后面的消息不管
			{
				cout<<"线程中得到消息 :"<<buf;
				//将第一行中的方法、路径、资源、协议分离出来
				get_path_method_query(buf,&cgi,path,method,url,query,treaty);

				if(!cgi) //不执行cgi 即GET方法
				{
					cout<<"exec GET"<<endl;
					exec_get(sock,path);//解析资源
					close(sock);//必须断开这个链接，为了刷新sock里缓存的数据
					//cout<<"exec get over"<<endl;
				}else //执行cgi  即POST方法
				{
					cout<<"exec POST"<<endl;
					exec_post(sock,path,method,query);
					close(sock);//必须断开这个链接，为了刷新sock里缓存的数据
					//cout<<"post over"<<endl;
				}
				memset(buf,'\0',sizeof(buf));
			}
		}

	public:
		//1实现得到server sock
		int get_sock(int port)
		{
			//server端初始化
			struct sockaddr_in server_name;
			bzero(&server_name,sizeof(server_name));
			server_name.sin_family=AF_INET;
			server_name.sin_port=htons(port);
			server_name.sin_addr.s_addr=htonl(INADDR_ANY);//随机
		
			//创建sock
			int server_sock=socket(AF_INET,SOCK_STREAM,0);
			if(-1 == server_sock)
			{
				print_log("create server sock failed");
				return -1;
			}
			//cout<<"server sock is :"<<server_sock<<endl;
			//端口复用
			int opt=1;
			setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt,sizeof(opt));
			//cout<<"端口复用成功"<<endl;
		
			//Bind 绑定端口号
			if(bind(server_sock,(struct sockaddr*)&server_name,sizeof(server_name)) <0)
			{
				print_log("bind failed");
				return -1;
			}
			//listen 监听端口号
			if(listen(server_sock,5)<0)
			{
				print_log("listen failed");
				return -1;
			}
			//cout<<"监听成功"<<endl;
			return server_sock;
		}
	protected:
		user_node* buynode(const string &name,const string &id,\
				const string &first="",const string &last="",int d=0)
		{
			user_node *node=new user_node(name,id,first,last,d);
			return node;
		}

}server_class;


//server_http server_class;
int GetSock(int port)
{
	return server_class.get_sock(port);
}
void* accept_request(void *arg)
{
	pthread_detach(pthread_self());//线程分离
	int sock=(int)arg;
	//cout<<"accept request sock is :"<<sock<<endl;
	server_class.Accept(sock);
	return NULL;
}



