#############################################################################
# Makefile for building: ChatServer
# Generated by qmake (3.0) (Qt 5.5.1)
# Project:  ChatServer.pro
# Template: app
# Command: /usr/lib/x86_64-linux-gnu/qt5/bin/qmake -spec linux-g++-64 CONFIG+=debug CONFIG+=qml_debug -o Makefile ChatServer.pro
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_QML_DEBUG
CFLAGS        = -m64 -pipe -g -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -m64 -pipe -g -std=c++0x -Wall -W -fPIC $(DEFINES)
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = ChatServer1.0.0
DISTDIR = /home/cyrildz/Project/ChatServer/.tmp/ChatServer1.0.0
LINK          = g++
LFLAGS        = -m64
LIBS          = $(SUBLIBS) -pthread 
AR            = ar cqs
RANLIB        = 
SED           = sed
STRIP         = strip

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		client.cpp \
		server.cpp \
		protocol.cpp \
		logger.cpp 
OBJECTS       = main.o \
		client.o \
		server.o \
		protocol.o \
		logger.o
DIST          = client.h \
		common.h \
		server.h \
		protocol.h \
		serialization.h \
		logger.h \
		inputargreader.h main.cpp \
		client.cpp \
		server.cpp \
		protocol.cpp \
		logger.cpp
QMAKE_TARGET  = ChatServer
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = ChatServer


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)




all: $(TARGET)

dist: distdir FORCE
	(cd `dirname $(DISTDIR)` && $(TAR) $(DISTNAME).tar $(DISTNAME) && $(COMPRESS) $(DISTNAME).tar) && $(MOVE) `dirname $(DISTDIR)`/$(DISTNAME).tar.gz . && $(DEL_FILE) -r $(DISTDIR)

distdir: FORCE
	@test -d $(DISTDIR) || mkdir -p $(DISTDIR)
	$(COPY_FILE) --parents $(DIST) $(DISTDIR)/


clean: compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean 
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


####### Sub-libraries

check: first

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.cpp server.h \
		common.h \
		client.h \
		serialization.h \
		protocol.h \
		logger.h
	$(CXX) -c $(CXXFLAGS) -o main.o main.cpp

client.o: client.cpp client.h
	$(CXX) -c $(CXXFLAGS)  -o client.o client.cpp

server.o: server.cpp server.h \
		common.h \
		client.h \
		serialization.h \
		protocol.h \
		logger.h
	$(CXX) -c $(CXXFLAGS)  -o server.o server.cpp

protocol.o: protocol.cpp protocol.h
	$(CXX) -c $(CXXFLAGS)  -o protocol.o protocol.cpp

logger.o: logger.cpp logger.h
	$(CXX) -c $(CXXFLAGS)  -o logger.o logger.cpp

####### Install

install:  FORCE

uninstall:  FORCE

FORCE:

