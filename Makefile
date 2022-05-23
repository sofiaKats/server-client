# Makefile for dataServer and remoteClient
SRC = ./src

# .o files will be stored here
BLD = ./obj

# Compiler options
CC = gcc
CFLAGS = -Wno-unused-parameter -g

# Executable file names
EXE_DATASERVER = ./dataServer
EXE_REMOTECLIENT = ./remoteClient

# .o files needed
OBJS_SERVER =  $(SRC)/server.o
OBJS_CLIENT =  $(SRC)/client.o

# Build executables
all: $(EXE_DATASERVER) $(EXE_REMOTECLIENT)
	mkdir -p $(BLD)
	mv -f $(OBJS_SERVER) $(BLD)
	mv -f $(OBJS_CLIENT) $(BLD)

$(EXE_DATASERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) $(OBJS_SERVER) -o $(EXE_DATASERVER)

$(EXE_REMOTECLIENT): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $(EXE_REMOTECLIENT)

# Delete executable & object files
clean:
	rm -f $(EXE_DATASERVER)
	rm -f $(EXE_REMOTECLIENT)
	rm -rf $(BLD)

# Clean and compile
comp: clean all