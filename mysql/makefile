WORK_PATH=$(shell pwd)

CC=g++
INCLUDE= -I${WORK_PATH}/mysql_lib/include -I.
LIB_PATH= -L${WORK_PATH}/mysql_lib/lib
SRC=mysql.cpp
BIN=mysql

LIB_NAME=-lmysqlclient

${BIN}:${SRC}
	${CC} -o $@ $^ ${INCLUDE} ${LIB_PATH} $(LIB_NAME) -g -D_DEBUG_

.PHONY:clean
clean:
	rm -f ${BIN}



