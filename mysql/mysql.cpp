
#include "mysql.h"

int main()
{

	string name="abcd";
	string number="123456";
	string begin="12:23:45";
	string end="12:34:67";
	string total="23";

	string db_user="Mysql";   //数据库名称
	string user_name="root"; //登陆数据库的用户
	string user_passwd="root";// 数据库的密码
	string host="127.0.0.1";      //数据库主机的ip
	
	string header[ROW];
	string out_str[1024][ROW];
	int line=0;

	Mysql sql(host,user_name,user_passwd,db_user);
	//sql.set_data_base(HOST_IP,db_user,passwd);
	cout<<"构造函数成功"<<endl;
	bool flag=sql.inter_link();
	if(flag)
		cout<<"link success"<<endl;
	else
		cout<<"link falied"<<endl;
	flag=sql.insert(name,number,begin,end,total);
	if(flag)
		cout<<"insert success"<<endl;
	else
		cout<<"insert falied"<<endl;
	flag=sql.show_table(header,out_str,&line);
	if(flag)
		cout<<"show table success"<<endl;
	else
		cout<<"show table failed"<<end;
	for(int i=0; i<ROW; ++i)
		cout<<header[i]<<"\t";
	cout<<endl;
	for(int i=0; i<line; ++i)
	{
		for(int j=0; j<ROW; ++j)
			cout<<out_str[i][j]<<"\t";
		cout<<endl;
	}

	return 0;
}
