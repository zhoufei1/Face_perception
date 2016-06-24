
#pragma once

#include <iostream>
#include <string>
#include "../mysql/mysql.h"
using namespace std;

const int SIZE=1024;
//const int ROW=5;
//const int ROW=4;

int main()
{
	string header[ROW];
	string show_data[SIZE][ROW];
	int line=0;
	cout<<"<html>"<<endl;
	cout<<"<head>"<<endl;
	cout<<"<p> wecome to linux data base show world </p>"<<endl;
	cout<<"</head>"<<endl;
	cout<<"<body>"<<endl;
	Mysql mysql;
	if(mysql.inter_link());
//		cout<<"link base success"<<endl;
	else;
//		cout<<"link base failed"<<endl;
	mysql.show_table(header,show_data,&line);

	cout<<"<table border=\"1\">"<<endl;
	cout<<"<tr>"<<endl;

	for(int i=0; i<ROW; ++i)
		cout<<"<th>"<<header[i]<<"</th>"<<endl;
	cout<<"</th>"<<endl;
	for(int i=0; i<line; ++i)
	{
		cout<<"<tr>"<<endl;
		for(int j=0; j<ROW; ++j)
		{
			cout<<"<td>"<<show_data[i][j]<<"</td>"<<endl;
		}
		cout<<"</tr>"<<endl;
	}
	cout<<"</table>"<<endl;
	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;
	return 0;
}







