FLAGS=-g -O0 -msse4.2 -std=c++11 -Wall
#BOOST_DIR = /usr/local/homebrew/include/boost
THRIFT_DIR = /usr/local/include/thrift
LIB_DIR = /usr/local/lib
GEN_SRC = ./gen-cpp/acsuser_types.cpp ./gen-cpp/acsuser_constants.cpp ./gen-cpp/UserService.cpp
default: server client
server: UserServer.cpp
	g++ -o UserServer UserServer.cpp ${GEN_SRC} $(FLAGS)  -I${THRIFT_DIR} -I./gen-cpp -L${LIB_DIR} -lthrift
client: UserClient.cpp
	g++ -o UserClient UserClient.cpp ${GEN_SRC} $(FLAGS) -lm -pthread -lz -lssl -I${THRIFT_DIR} -I./gen-cpp -L${LIB_DIR} -lthrift
clean:
	$(RM) -r UserServer UserClient
