#include <cassandra.h>
#include <iostream>
#include <sw/redis++/redis++.h>
#include <thread>
#include <unistd.h>
#include <ctime>
#include <set>
#include <vector>

//Related header file
#include "RedisSubdivision.h"

using namespace sw::redis;
using namespace std;

namespace solution_subdivision {
    vector<set<int>> sets;
    int cpt = 0;

    Redis redis = Redis("tcp://127.0.0.1:6379");

    int main(int argc, char **argv) {
        const char *cassandra_host = "127.0.0.1", *redis_host = "127.0.0.1";
        int redis_port = 6379, cassandra_port = 9042;
        if (argc == 5) {
            redis_host = argv[1];
            redis_port = atoi(argv[2]);
            cassandra_host = argv[3];
            cassandra_port = atoi(argv[4]);
        }

        try {
            //Connexion à la base
            ConnectionOptions connection_options;
            connection_options.host = redis_host;
            connection_options.port = redis_port;
            redis = Redis(connection_options);

            //Création des ensembles
            for(int i=0; i<3; i++) {
                add_set(i);
            }

            //Ecritures dans la base (et lecture instantanée)
            for (int i = 0; i < 500; i++) {
                string key = to_string(i);
                string keyWithTTL = key + EXTENSION;

                if (i < 200) {
                    add_key_to_set(0);
                } else if (i < 400) {
                    add_key_to_set(1);
                } else {
                    add_key_to_set(2);
                }
            }
            print_sets();
        } catch (const Error &e) {
            cout << e.what() << endl;
        }

        return 0;
    }

    void add_set(int set_id) {
        set<int> new_set;
        sets.push_back(new_set);
    }

    int add_key_to_set(int set_id) {
        if(sets.at(set_id).size()>MAX_SET_SIZE) {

        }
        sets.at(set_id).insert(cpt);
        return cpt++;
    }

    void print_sets() {
        for(int i=0; i<sets.size(); i++) {
            cout << "Set("<<i<<") - "<<sets.at(i).size()<<" elements : [";
            for(time_t time : sets.at(i)) {
                cout << time << " ";
            }
            cout << "]" << endl;
        }
    }

    vector<string> split(char delimiter, string src) {
        vector<string> res{};
        size_t pos;

        while ((pos = src.find(delimiter)) != string::npos) {
            res.push_back(src.substr(0, pos));
            src.erase(0, pos + 1);
        }

        return res;
    }
}