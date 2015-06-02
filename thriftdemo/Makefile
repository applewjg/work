FLAGS=-g -O0 -msse4.2 -std=c++11 -Wall
BOOST_DIR = /usr/local/homebrew/include/boost 
THRIFT_DIR = /usr/local/homebrew/include/thrift
LIB_DIR = /usr/local/homebrew/lib  
GEN_SRC = ./gen-cpp/acsuser_types.cpp ./gen-cpp/acsuser_constants.cpp ./gen-cpp/UserService.cpp  
default: server client  
server: UserServer.cpp  
	g++ -o UserServer $(FLAGS) -I${THRIFT_DIR} -I${BOOST_DIR}  -I./gen-cpp -L${LIB_DIR} -lthrift UserServer.cpp ${GEN_SRC}  
client: UserClient.cpp  
	g++ -o UserClient $(FLAGS) -lm -pthread -lz -lssl -I${THRIFT_DIR} -I${BOOST_DIR}  -I./gen-cpp -L${LIB_DIR} -lthrift UserClient.cpp ${GEN_SRC}  
clean:  
	$(RM) -r UserServer UserClient 