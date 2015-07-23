PWD = $(shell pwd)
KERNEL_SRC = /usr/src/linux-source/
PROJECT_ROOT = ../
CXX = g++
CXXFLAGS = -g -W -Wall
MAKE = make 
CLEAN = make clean -i

LINK = g++
LINKFLAGS = -rdynamic

LIBS = 

BUILD_TARGETS = main

SOURCE　:=　$( wildcard　*.c )　$( wildcard　*.cpp ) 
OBJS　:=　$( patsubst　%.c, %.o, $(patsubst　%.cpp, %.o, $(SOURCE)) ) 
DEPS　:=　$( patsubst　%.o, %.d, $(OBJS) ) 

all: $(BUILD_TARGETS) output

.PHONY: all main clean output

SUBDIRS = $( addprefix ../, $(ALL_PATH) )
TEST_SUBDIRS = ../unittest

main:
	@for i in ${SUBDIRS}; { \
	       	${MAKE} -C $$i -j8 bc; \
		if [ $$? -ne 0 ];then\
			echo "++++++++Make '$$dir' failed++++++++";\
			exit $$?;\
		fi \
	} ; \
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp
	$(LINK) $(LINKFLAGS) -o $@ main.o $(LIBS)

clean:
	@(for i in ${SUBDIRS}; do (cd $$i; pwd; ${MAKE} $@); done)
	rm -rf $(BUILD_TARGETS) *.o output

output: $(BUILD_TARGETS)
	mkdir -p output
	mkdir -p output/bin
	cp -rf $(BUILD_TARGETS) output/bin

