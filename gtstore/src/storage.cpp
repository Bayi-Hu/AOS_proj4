#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cryptopp/sha3.h>
#include <cryptopp/cryptlib.h>
#include "gtstore.hpp"


void GTStoreStorage::init(int port) {

	cout << "Inside GTStoreStorage::init()\n";
	this->port = port;

	// create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
    }

    // set up the address and port to bind to
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->port);

    // bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(sockfd);
    }

    while (true) {
        // receive a message
        char buffer[1024];
        struct sockaddr_in recv_addr;
        socklen_t recv_addrlen = sizeof(recv_addr);
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &recv_addr, &recv_addrlen);
        if (n < 0) {
            continue;
        }

        string message(buffer);
        stringstream ss(message);
        string type, key, value;
        getline(ss, type, '|');
        getline(ss, key, '|');
        getline(ss, value, '|');

        int message_type = std::stoi(type);

        // Process the message
        cout << "Received message: type=" << type << ", key=" << key << ", value=" << value << endl;

        // handle the message based on its type
        const char* response = "";
        switch (message_type) {
            case 0:
                cout<<"Store the key/value"<<endl;
                this->dict[key] = value;
                response = "Successfully|";
                break;

            case 1:
                cout<<"Get the value based on key"<<endl;
                value = this->dict[key];
                response = (value + string("|")).c_str();
                break;

            case 2: // checking status
                cout<<"Message type 2"<<endl;
                break;
            default:
                // invalid message type
                response = "Invalid message type";
                break;
        }

        // send a response
        if (sendto(sockfd, response, strlen(response), 0, (struct sockaddr*) &recv_addr, recv_addrlen) < 0) {
            std::cerr << "Failed to send response" << std::endl;
            continue;
        }
    }

    // close the socket
    close(sockfd);
}