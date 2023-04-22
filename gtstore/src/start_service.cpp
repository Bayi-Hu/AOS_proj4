#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "gtstore.hpp"
using namespace std;

void run_server(int port) {
    GTStoreStorage storage;
    cout<<"this is "<< port<<endl;
    storage.init(port);
}

int main() {
    int num_processes = 5;
    int num_replica = 1;

    for (int i = 0; i < num_processes; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << "Failed to create process" << std::endl;
            return 1;
        } else if (pid == 0) {
            int port = 5000 + i;
            run_server(port);
            return 0;
        }
    }

    // Wait for all child processes to finish
    int status;
    for (int i = 0; i < num_processes; ++i) {
        wait(&status);
    }

    return 0;
}