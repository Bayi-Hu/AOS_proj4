#ifndef GTSTORE
#define GTSTORE

#include <string>
#include <cstdio>
#include <map>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_KEY_BYTE_PER_REQUEST 20
#define MAX_VALUE_BYTE_PER_REQUEST 1000

using namespace std;

typedef vector<string> val_t;

class GTStoreClient {
		private:
				int client_id;
//				string value;
		public:
				void init(int id);
				void finalize();
				string get(string key);
				bool put(string key, string value);
				uint32_t jenkins_hash(const void* key, size_t length);
};

class GTStoreManager {
		public:
				void init();
};

class GTStoreStorage {
		public:
				void init(int port);
	            int port;
	            map<string, string> dict;
};

#endif