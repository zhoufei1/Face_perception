
SRC=server.cpp
DES=server

#debug=DEBUG

FLAG=-g
$(DES):$(SRC)
	g++ -o $@ $^ $(FLAG) -lpthread
	
.PHONY:output
output:
	cp server ../
.PHONY:clean
clean:
	rm -rf server


