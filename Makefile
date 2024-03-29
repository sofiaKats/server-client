# Makefile for dataServer and remoteClient
SRC = ./src

# .o files will be stored here
BLD = ./obj

# Compiler options
CC = gcc
CFLAGS = -Wno-unused-parameter -g -pthread -w

# Executable file names
EXE_DATASERVER = ./dataServer
EXE_REMOTECLIENT = ./remoteClient

# .o files needed
COMMON_OBJS += $(SRC)/validation.o
OBJS_SERVER =  $(SRC)/server.o $(SRC)/directory.o $(SRC)/queue.o
OBJS_CLIENT =  $(SRC)/client.o $(SRC)/create_directory.o

# Build executables
all: $(EXE_DATASERVER) $(EXE_REMOTECLIENT)
	mkdir -p $(BLD)
	mv -f $(COMMON_OBJS) $(BLD)
	mv -f $(OBJS_SERVER) $(BLD)
	mv -f $(OBJS_CLIENT) $(BLD)

$(EXE_DATASERVER): $(OBJS_SERVER) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(OBJS_SERVER) $(COMMON_OBJS) -o $(EXE_DATASERVER)

$(EXE_REMOTECLIENT): $(OBJS_CLIENT) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(OBJS_CLIENT) $(COMMON_OBJS) -o $(EXE_REMOTECLIENT)

# Delete executable & object files
clean:
	rm -f $(EXE_DATASERVER)
	rm -f $(EXE_REMOTECLIENT)
	rm -rf $(BLD)

# Clean and compile
comp: clean all