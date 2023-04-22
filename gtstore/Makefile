CFLAGS  =
LFLAGS  =
CC      = g++ -std=c++11
RM      = /bin/rm -rf

TESTS = test_app manager storage
CLIENT_SRC = src/test_app.cpp src/client.cpp
LIB_SRC = src/gtfs.cpp

all: $(TESTS)

manager: src/manager.cpp
	$(CC) -Wall src/manager.cpp -o bin/manager

storage: src/storage.cpp
	$(CC) -Wall src/storage.cpp -o bin/storage

test_app: 
	$(CC) -Wall $(CLIENT_SRC) -o bin/test_app

clean:
	$(RM) *.o $(TESTS)

client:
	$(CC) -Wall -Wextra -pedantic src/client.cpp -o bin/client

start_service:
	$(CC) -Wall -Wextra -pedantic src/start_service.cpp src/storage.cpp src/manager.cpp -o bin/start_service

