#include "gtstore.hpp"

void single_set_get(int client_id) {
		cout << "Testing single set-get for GTStore by client " << client_id << ".\n";

		GTStoreClient client;
		client.init(client_id);

		string key = to_string(client_id);
		vector<string> value;
		value.push_back("phone");
		value.push_back("phone_case");

		client.put(key, value);
		client.get(key);

		client.finalize();
}

//void multiple_set_get(int client_id)

int main(int argc, char **argv) {
		string test = string(argv[1]);
		int client_id = atoi(argv[2]);
//		cout<<"client id:"<<client_id<<endl;

		string test1 = "single_set_get";
		if (string(argv[1]) ==  test1) {
			single_set_get(client_id);
		}
		string test2 = "multiple_set_get";
		if (string(argv[1]) == test1) {
//		    single_set_get(client_id);
		}

		return 1;
}
