#include <cassandra.h>
#include <iostream>
#include <sw/redis++/redis++.h>
#include <thread>
#include <unistd.h>

using namespace sw::redis;
using namespace std;

const string EXTENSION = "_key";
const int EXTENSION_LENGTH = EXTENSION.size();

Redis redis = Redis("tcp://127.0.0.1:6379");

//Extract the label at the end of the channel name (last field after delimiter ':')
string extract_label(string msg) {
    string delimiter = ":";
    size_t pos = msg.find_last_of(delimiter);
    string label = msg.substr(pos+1, msg.size()-pos);
    return label;
}


//Handler for key expiration / eviction
void listenKeyspaceEvents() {
    try {
        // Create a Subscriber.
        auto sub = redis.subscriber();

        // Set callback functions.
        sub.on_pmessage([](std::string pattern, std::string channel, std::string msg) {
            if(msg=="expired"){
                string label = extract_label(channel);
                label = label.substr(0, label.size()-EXTENSION_LENGTH); //On supprime l'extension "_key"
                auto val = redis.get(label);
                redis.del(label);
                if (val) { //TODO Stocker la données dans la base cassandra
                    cout << msg << " : " << label << "=>" << *val << endl;
                } else { //Une donnée est perdue
                    cout << "The key " << label << " doesn't exist on Redis server" << endl;
                }
            } else {

            }
        });

        // Subscribe to channels and patterns.
        sub.psubscribe("__key*__:*");

        // Wait for pmesssages.
        while (true) {
            try {
                sub.consume();
            } catch (const Error &err) {
                cout << "Consume error" << endl;
            }
        }
    } catch (const Error &e) {
        cout << e.what() << endl;
    }
}

int main(int argc, char **argv) {
    const char *cassandra_host = "127.0.0.1", *redis_host = "127.0.0.1";
    int redis_port = 6379, cassandra_port = 9042;
    if(argc==5) {
        redis_host = argv[1];
        redis_port = atoi(argv[2]);
        cassandra_host = argv[3];
        cassandra_port = atoi(argv[4]);
    }


    cout << "#############  TEST REDIS  #############" << endl;

    try {
        //Connexion à la base
        ConnectionOptions connection_options;
        connection_options.host = redis_host;
        connection_options.port = redis_port;
        redis = Redis(connection_options);

        //Launching thread
        thread keyspaceListener(listenKeyspaceEvents);

        //Ecritures dans la base (et lecture instantanée)
        for(int i=0; i<500; i++) {
            string key = to_string(i);
            string keyWithTTL = key + EXTENSION;
            redis.setex(keyWithTTL, 1, "");
            redis.set(key, "val"+key);
            auto val = redis.get(key);
            if (val) {
                cout << "Successfully wrote ('" << key << "','" << *val <<"') on Redis server." << endl;
            } else {
                cout << "Key doesn't exist" << endl;
            }
            usleep(1000005);
            //sleep(1);
        }
    } catch (const Error &e) {
        cout << e.what() << endl;
    }











    cout << endl << endl << "############# TEST CASSANDRA #############" << endl;
    CassFuture* connect_future = NULL;
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();

    /* Add contact points */
    cass_cluster_set_contact_points(cluster, cassandra_host);
    cass_cluster_set_port(cluster, cassandra_port);

    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect(session, cluster);

    if (cass_future_error_code(connect_future) == CASS_OK) {
        CassFuture* close_future = NULL;

        /* Build statement and execute query */
        const char* query = "SELECT release_version FROM system.local";
        CassStatement* statement = cass_statement_new(query, 0);

        CassFuture* result_future = cass_session_execute(session, statement);

        if (cass_future_error_code(result_future) == CASS_OK) {
            /* Retrieve result set and get the first row */
            const CassResult* result = cass_future_get_result(result_future);
            const CassRow* row = cass_result_first_row(result);

            if (row) {
                const CassValue* value = cass_row_get_column_by_name(row, "release_version");

                const char* release_version;
                size_t release_version_length;
                cass_value_get_string(value, &release_version, &release_version_length);
                printf("release_version: '%.*s'\n", (int)release_version_length, release_version);
            }

            cass_result_free(result);
        } else {
            /* Handle error */
            const char* message;
            size_t message_length;
            cass_future_error_message(result_future, &message, &message_length);
            fprintf(stderr, "Unable to run query: '%.*s'\n", (int)message_length, message);
        }

        cass_statement_free(statement);
        cass_future_free(result_future);

        /* Close the session */
        close_future = cass_session_close(session);
        cass_future_wait(close_future);
        cass_future_free(close_future);
    } else {
        /* Handle error */
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future, &message, &message_length);
        fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
    }

    cass_future_free(connect_future);
    cass_cluster_free(cluster);
    cass_session_free(session);


    return 0;
}
