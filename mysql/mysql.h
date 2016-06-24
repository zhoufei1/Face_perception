#pragma once

#include "mysql_lib/include/mysql.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

const int ROW=4;
//const int ROW=5;
#define HOST_IP "127.0.0.1"
const string  USER="root";

class Mysql
{
	private:
		MYSQL *mysql_base;
		MYSQL_RES *res;

		string host;   //主机ip
		string user_name; //用户名称
		string user_db;   //数据库名称
		string passwd; //登陆数据库密码
		
		string name;   //姓名
		string sub_number; //注册的用户号
		string begin_time; //签到时间
		string end_time;  //离开时间
		string work_time; //一天上班时间
	private:
		void Insert_base(const string &Name,const string &Sub_number,\
				const string &Begin,const string &End,const string &total_time)
		{
			name=Name;
			sub_number=Sub_number;
			begin_time=Begin;
			end_time=End;
			work_time=total_time;
		}
	public:
		bool insert(const string &Name,const string &Sub_number,const string &Begin,\
				const string &End,const string &total_time)
		{
			Insert_base(Name,Sub_number,Begin,End,total_time);

			//string tmp="INSERT INTO mysql_table(Name,Sub_number,Begin,End,total_time) values";
			string tmp="INSERT INTO mytest values";
			//string tmp="INSERT INTO mysql_table values";
			tmp+="('";
			tmp+=Name;
			tmp+="','";
			tmp+=Sub_number;
			tmp+="','";
			tmp+=Begin;
			tmp+="','";
			tmp+=End;
			tmp+="','";
			tmp+=total_time;
			tmp+="');";

			//cout<<"方法："<<tmp<<endl;
			if(mysql_query(mysql_base,tmp.c_str())==0)
			{
			//	cout<<"mysql insert success"<<endl;
				return true;
			}
			else
			{
				//cout<<"mysql insert failed"<<endl;
				return false;
			}
		}
	public:
		Mysql(const string &Host=HOST_IP,const string &User_name="root",\
			const string &Passwd="root",const string &User_db="mytest")
		{
			mysql_base=mysql_init(NULL);//初始化数据库
			res=NULL;
			host=Host;
			user_name=User_name;
			passwd=Passwd;
			user_db=User_db;
		}
		~Mysql()
		{
			close_connect();
			if(res !=NULL)
			{
				free(res);
				res=NULL;
			}
		}
		void close_connect()
		{
			mysql_close(mysql_base);
		}
		bool show_table(string name[],string out_str[1024][ROW],int *line)
		{
			string mytmp="SELECT * FROM mytest";
			//string mytmp="SELECT * FROM mysql_table";
			if(mysql_query(mysql_base,mytmp.c_str())==0);
		//		cout<<"select * from mysql success"<<endl;
			else;
			//	cout<<"select * from mysql falied"<<endl;
			res=mysql_store_result(mysql_base);
			int Line=mysql_num_rows(res);  //行
			int List=mysql_num_fields(res);//列
			*line=Line;
			MYSQL_FIELD *fd=NULL;
			int i=0;
			while(fd=mysql_fetch_field(res))
				name[i++]=fd->name;
			for(int index=0; index <Line; ++index)
			{
				MYSQL_ROW row=mysql_fetch_row(res);
				if(row)
				{
					for(int start=0; start <List; ++start)
						out_str[index][start]=row[start];
				}
			}
			return true;
		}
	public:
		bool inter_link()
		{
			if(mysql_real_connect(mysql_base,host.c_str(),user_name.c_str(),\
			    passwd.c_str(),user_db.c_str(),3306,NULL,0)==NULL)
				return false;
			else
				return true;
		}
};
