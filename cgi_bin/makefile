
FIND_PATH=-I/home/linux/code/project/face_identifitation/mysql -I/home/linux/code/project/face_identifitation/mysql/mysql_lib/include -L/home/linux/code/project/face_identifitation/mysql/mysql_lib/lib -lmysqlclient 

DATA_SRC=data_base_show.cpp
DATA_DES=data_base_show

SHOW_SRC=show.cpp
SHOW_DES=show
.PHONY:all
all:$(DATA_DES) $(SHOW_DES)

$(SHOW_DES):$(SHOW_SRC)
	g++ -o $@ $^ -g

$(DATA_DES):$(DATA_SRC) /home/linux/code/project/face_identifitation/mysql/mysql.h
	g++ -o $@ $^ -g  $(FIND_PATH) 




.PHONY:output
output:
	chmod 755 client.sh
	sh client.sh
	cp index.html ../htdocs
.PHONY:clean
clean:
	chmod 644 client.sh
	rm -rf $(SHOW_DES) $(DATA_DES)
