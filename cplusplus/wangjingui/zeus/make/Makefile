PROJECT_ROOT = ../
WORKROOT = ../../../

CXX = g++
CXXFLAGS = -g -pipe -W -Wall -Wreorder -Wswitch -Wpointer-arith -Wconversion -Wformat
LINK = g++
LINKFLAGS = -rdynamic

SE_DIR = $(WORKROOT)/ps/se
LIB2_DIR = $(WORKROOT)/lib2-64
PUBLIC_DIR = $(WORKROOT)/public
THIRD_DIR = $(WORKROOT)/third-64

INCLUDES = -I./ -I../ \
		   -I$(PUBLIC_DIR)/configure/output/include \
		   -I$(LIB2_DIR)/bsl/include \
		   -I$(LIB2_DIR)/ccode/include \
		   -I$(LIB2_DIR)/ullib/include \
		   -I$(LIB2_DIR)/dict/include \
		   -I$(SE_DIR)/conf-manager/output/include

LIBS = -L$(SE_DIR)/conf-manager/output/lib/ -lconfmgr \
	   -L$(PUBLIC_DIR)/configure/output/lib -lconfig \
	   -L$(PUBLIC_DIR)/ependingpool/output -lependingpool \
	   -L$(PUBLIC_DIR)/spreg/output/lib -lspreg \
	   -L$(THIRD_DIR)/pcre/lib  -lpcre \
	   -L$(LIB2_DIR)/bsl/lib -lbsl \
	   -L$(LIB2_DIR)/others-ex/lib -lullib_ex \
       -L$(LIB2_DIR)/ccode/lib -lulccode \
	   -L$(LIB2_DIR)/dict/lib -luldict \
	   -L$(LIB2_DIR)/ullib/lib -lullib \
	   -L$(SE_DIR)/gs/output/lib -lgs \
	   -lpthread -lcrypto -lm

BUILD_TARGETS = zeus

all: $(BUILD_TARGETS) output

.PHONY : all zeus output

zeus:
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp $(INCLUDES)
	$(LINK) $(LINKFLAGS) -o $@ main.o $(LIBS)
