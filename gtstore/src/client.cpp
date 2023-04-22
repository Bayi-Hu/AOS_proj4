#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <cstdint>
#include "gtstore.hpp"
using namespace std;


void GTStoreClient::init(int id) {

    cout << "Inside GTStoreClient::init() for client " << id << "\n";
    // send message to the server to get the original table

    client_id = id;
}

string GTStoreClient::get(string key) {

//    int port = GTStoreClient::hash_key(key) + 5000;

    uint32_t hash = GTStoreClient::jenkins_hash(key.c_str(), key.length());
    int port = hash % 5 + 5000;

    cout<<"hash:"<<hash<<endl;
    cout<<"port:"<<port<<endl;

    // create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "Failed to create socket" << endl;
        return nullptr;
    }

    struct sockaddr_in send_addr;
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    send_addr.sin_port = htons(port);

    // send a message to the server
    string send_message = "1|" + key + "|";
    if (sendto(sockfd, send_message.c_str(), send_message.length(), 0, (struct sockaddr*) &send_addr, sizeof(send_addr)) < 0) {
        cerr << "Failed to send message" << endl;
        close(sockfd);
        return nullptr;
    }
    // recv the value from the storage node
    char buffer[1024];
    struct sockaddr_in recv_addr;
    socklen_t recv_addrlen = sizeof(recv_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &recv_addr, &recv_addrlen);
    if (n < 0) {
        cerr << "Failed to receive message" << endl;
    }
    string rec_message(buffer);
    stringstream ss(rec_message);
    string value;
    getline(ss, value, '|');
    return value;

}

uint32_t GTStoreClient::jenkins_hash(const void* key, size_t length){
    // determine which server to store based on the key.
    const uint8_t* data = static_cast<const uint8_t*>(key);
    uint32_t hash = 0;

    for (size_t i = 0; i < length; ++i)
    {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

bool GTStoreClient::put(string key, string value) {

    uint32_t hash = GTStoreClient::jenkins_hash(key.c_str(), key.length());
    int port = hash % 5 + 5000;

    cout<<"hash:"<<hash<<endl;
    cout<<"port:"<<port<<endl;
    // int port = GTStoreClient::hash_key(key) + 5000;
    // create a UDP socket

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "Failed to create socket" << endl;
        return 1;
    }

    // set up the server address and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);

    // send a message to the server
    string message = "0|" + key + "|" + value + "|";
    if (sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "Failed to send message" << endl;
        close(sockfd);
        return 1;
    }

    // recv a successful message from the storage node
    char buffer[1024];
    struct sockaddr_in recv_addr;
    socklen_t recv_addrlen = sizeof(recv_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &recv_addr, &recv_addrlen);
    if (n < 0) {
        cerr << "Failed to receive message" << endl;
    }

    string rec_message(buffer);
    stringstream ss(rec_message);
    string status;
    getline(ss, status, '|');
    cout<<status<<endl;

    return true;

}

void GTStoreClient::finalize() {
    cout << "Inside GTStoreClient::finalize() for client " << client_id << "\n";
}


int main(){

    GTStoreClient client;
    int client_id = 0;
    client.init(client_id);
    string key = "1234";
    string value = "HelloSihao";

    client.put(key, value);
    string rec_value = client.get(key);
    cout<<rec_value<<endl;

    client.finalize();

}