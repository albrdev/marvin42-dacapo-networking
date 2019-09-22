APP_BIN		:= m42net

CC_C		:= gcc
CC_CPP		:= g++
CC_FLAGS	:= -pedantic -Wall -Wextra -Wconversion -std=c++11
CC_LIBS		:= -lm -lserialport

FLAGS_DBG	:= -g -Wno-unused-parameter
FLAGS_RLS	:= -DNDEBUG

DIR_INC		:= inc
DIR_SRC		:= src
DIR_OBJ		:= obj
DIR_BIN		:= bin
DIR_SL		:= shared_libraries

CMD_CP		:= cp -f
CMD_RM		:= rm -f
CMD_MKDIR	:= mkdir -p
CMD_PRINT	:= @printf

SRCS_C		:= $(wildcard $(DIR_SRC)/*.c)
OBJS_C		:= $(patsubst $(DIR_SRC)/%.c,$(DIR_OBJ)/%.o,$(SRCS_C))
SRCS_CPP	:= $(wildcard $(DIR_SRC)/*.cpp)
OBJS_CPP	:= $(patsubst $(DIR_SRC)/%.cpp,$(DIR_OBJ)/%.o,$(SRCS_CPP))
SRCS_SLC	:= $(DIR_SL)/crc.c $(DIR_SL)/packet.c $(DIR_SL)/custom_packets.c
OBJS_SLC	:= $(patsubst $(DIR_SL)/%.c,$(DIR_OBJ)/%.o,$(SRCS_SLC))
SRCS_SLCPP	:= $(DIR_SL)/generic.cpp
OBJS_SLCPP	:= $(patsubst $(DIR_SL)/%.cpp,$(DIR_OBJ)/%.o,$(SRCS_SLCPP))
OBJS		:= $(OBJS_C) $(OBJS_CPP) $(OBJS_SLC) $(OBJS_SLCPP)

TRG_BIN		:= $(DIR_BIN)/$(APP_BIN)

BIN_SERVER1	:= $(APP_BIN)_tcpsrv
BIN_SERVER2	:= $(APP_BIN)_udpsrv
BIN_CLIENT1	:= $(APP_BIN)_tcpcli
BIN_CLIENT2	:= $(APP_BIN)_udpcli

TRG_SERVER	:= $(DIR_BIN)/$(BIN_SERVER1) $(DIR_BIN)/$(BIN_SERVER2)
TRG_CLIENT	:= $(DIR_BIN)/$(BIN_CLIENT1) $(DIR_BIN)/$(BIN_CLIENT2)

TRG_BINS	:= $(TRG_SERVER) $(TRG_CLIENT)

.PHONY: all
all: debug

.PHONY: release
release: CC_FLAGS += $(FLAGS_RLS)
release: mkdirs $(TRG_BINS)

.PHONY: debug
debug: CC_FLAGS += $(FLAGS_DBG)
debug: mkdirs $(TRG_BINS)

.PHONY: clean
clean:
	$(CMD_RM) $(DIR_OBJ)/*.o

.PHONY: distclean
distclean: clean
	$(CMD_RM) $(TRG_BINS) $(patsubst %,%.exe,$(TRG_BINS))

.PHONY: test
test:
	$(CMD_PRINT) "\n"

.PHONY: run
run: $(TRG_BIN)
	./$< $(args)

.PHONY: mkdirs
mkdirs: $(DIR_OBJ) $(DIR_BIN)

$(DIR_OBJ):
	$(CMD_MKDIR) $@

$(DIR_BIN):
	$(CMD_MKDIR) $@

# Main
$(DIR_BIN)/$(BIN_SERVER1): $(OBJS) $(DIR_OBJ)/tcpserver_main.o
	$(CC_CPP) $(CC_LIBS) $^ -o $@

$(DIR_BIN)/$(BIN_SERVER2): $(OBJS) $(DIR_OBJ)/udpserver_main.o
	$(CC_CPP) $(CC_LIBS) $^ -o $@

$(DIR_BIN)/$(BIN_CLIENT1): $(OBJS) $(DIR_OBJ)/tcpclient_main.o
	$(CC_CPP) $(CC_LIBS) $^ -o $@

$(DIR_BIN)/$(BIN_CLIENT2): $(OBJS) $(DIR_OBJ)/udpclient_main.o
	$(CC_CPP) $(CC_LIBS) $^ -o $@

$(DIR_OBJ)/tcpclient_main.o: tcpclient_main.cpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -I$(DIR_SL) -c $< -o $@

$(DIR_OBJ)/udpclient_main.o: udpclient_main.cpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -I$(DIR_SL) -c $< -o $@

$(DIR_OBJ)/tcpserver_main.o: tcpserver_main.cpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -I$(DIR_SL) -c $< -o $@

$(DIR_OBJ)/udpserver_main.o: udpserver_main.cpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -I$(DIR_SL) -c $< -o $@

# External/Shared
$(DIR_OBJ)/custom_packets.o: $(DIR_SL)/custom_packets.c $(DIR_SL)/custom_packets.h
	$(CC_C) $(CC_FLAGS) -I$(DIR_SL) -c $< -o $@

$(DIR_OBJ)/packet.o: $(DIR_SL)/packet.c $(DIR_SL)/packet.h
	$(CC_C) $(CC_FLAGS) -I$(DIR_SL) -c $< -o $@

$(DIR_OBJ)/crc.o: $(DIR_SL)/crc.c $(DIR_SL)/crc.h
	$(CC_C) $(CC_FLAGS) -I$(DIR_SL) -c $< -o $@

$(DIR_OBJ)/generic.o: $(DIR_SL)/generic.cpp $(DIR_SL)/generic.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_SL) -c $< -o $@

# Units
$(DIR_OBJ)/ClientOptions.o: $(DIR_SRC)/ClientOptions.cpp $(DIR_INC)/ClientOptions.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/ServerOptions.o: $(DIR_SRC)/ServerOptions.cpp $(DIR_INC)/ServerOptions.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Options.o: $(DIR_SRC)/Options.cpp $(DIR_INC)/Options.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SerialPort.o: $(DIR_SRC)/SerialPort.cpp $(DIR_INC)/SerialPort.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/UDPClient.o: $(DIR_SRC)/UDPClient.cpp $(DIR_INC)/UDPClient.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/UDPServer.o: $(DIR_SRC)/UDPServer.cpp $(DIR_INC)/UDPServer.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/TCPClient.o: $(DIR_SRC)/TCPClient.cpp $(DIR_INC)/TCPClient.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/TCPServer.o: $(DIR_SRC)/TCPServer.cpp $(DIR_INC)/TCPServer.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Client.o: $(DIR_SRC)/Client.cpp $(DIR_INC)/Client.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Server.o: $(DIR_SRC)/Server.cpp $(DIR_INC)/Server.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Socket.o: $(DIR_SRC)/Socket.cpp $(DIR_INC)/Socket.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -D__USE_POSIX -c $< -o $@

$(DIR_OBJ)/IPAuthority.o: $(DIR_SRC)/IPAuthority.cpp $(DIR_INC)/IPAuthority.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/IError.o: $(DIR_SRC)/IError.cpp $(DIR_INC)/IError.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/ErrorInfo.o: $(DIR_SRC)/ErrorInfo.cpp $(DIR_INC)/ErrorInfo.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Exception.o: $(DIR_SRC)/Exception.cpp $(DIR_INC)/Exception.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
