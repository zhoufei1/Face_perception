
SRC=server/server.cpp
DES=server

FLAG=-g
$(DES):$(SRC)
	g++ -o $@ $^ $(FLAG)
	
.PHONY:clean
clean:
	rm -rf server


