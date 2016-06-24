CLIENT_DES=client/client_http.cpp
CLIENT_SRC=client_http
SERVER_DES=server/server_http.cpp
SERVER_SRC=server_http

#mysql file
#WORK_PATH=$(shell pwd)
#
#CC=g++
#INCLUDE= -I${WORK_PATH}/mysql/mysql_lib/include -I.
#LIB_PATH= -L${WORK_PATH}/mysql/mysql_lib/lib
#MYSQL_SRC=mysql/mysql.cpp
#MYSQL_BIN=mysql
#
#LIB_NAME=-lmysqlclient

FIND_PATH=-I/home/linux/code/project/face_identifitation/mysql -I/home/linux/code/project/face_identifitation/mysql/mysql_lib/include -L/home/linux/code/project/face_identifitation/mysql/mysql_lib/lib -lmysqlclient

flag=-lpthread

.PHONY:all
all:$(CLIENT_SRC) $(SERVER_SRC) $(MYSQL_BIN)

$(CLIENT_SRC):$(CLIENT_DES)
	g++ -o $@ $^ -g $(flag)
$(SERVER_SRC):$(SERVER_DES) /home/linux/code/project/face_identifitation/mysql/mysql.h
	g++ -o $@ $^ -g  $(FIND_PATH) $(flag)


.PHONY:output
output:
	mkdir output
	mv $(CLIENT_SRC) ./output
	mv $(SERVER_SRC) ./output
	cp ./server_sh.sh ./output
	cp ./htdocs ./output -r
	chmod 755 server_sh.sh
	chmod 755 client_sh.sh
	sh server_sh.sh


.PHONY:clean
clean:
	chmod 644 ./server_sh.sh
	chmod 644 ./client_sh.sh
	rm -rf output
