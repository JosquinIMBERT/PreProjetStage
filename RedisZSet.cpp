#include <cassandra.h>
#include <iostream>
#include <sw/redis++/redis++.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "RedisZSet.h"

using namespace sw::redis;
using namespace std;

namespace solution_zset {
    Redis redis = Redis("tcp://127.0.0.1:6379");
    CassCluster *cluster;
    CassSession *session;

    int main(int argc, char **argv) {
        //##### Initialisation des données de connexion #####
            const char *cassandra_host = "127.0.0.1", *redis_host = "127.0.0.1";
            int redis_port = 6379, cassandra_port = 9042;
            if (argc == 5) {
                redis_host = argv[1];
                redis_port = atoi(argv[2]);
                cassandra_host = argv[3];
                cassandra_port = atoi(argv[4]);
            }

        //##### Connexion à la base cassandra #####
            CassFuture *connect_future = NULL;
            cluster = cass_cluster_new();
            session = cass_session_new();
            // Add contact points
            cass_cluster_set_contact_points(cluster, cassandra_host);
            cass_cluster_set_port(cluster, cassandra_port);
            // Provide the cluster object as configuration to connect the session
            connect_future = cass_session_connect(session, cluster);
            if (cass_future_error_code(connect_future) != CASS_OK) {
                cerr << "Impossible de se connecter à la base Cassandra. Terminaison du programme." << endl;
                cass_future_free(connect_future);
                cass_cluster_free(cluster);
                cass_session_free(session);
                exit(1);
            }

        //##### Création des tables inexistantes #####
        /*    const CassSchemaMeta_ *schemaMeta = cass_session_get_schema_meta(session);
            const CassKeyspaceMeta_ * keyspaceMeta = cass_schema_meta_keyspace_by_name(schemaMeta, "BGP_KEYSPACE");
            if(keyspaceMeta==NULL) {
                const char *ks_query = "CREATE KEYSPACE BGP_KEYSPACE "
                                       " WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3}";
                CassStatement_ *ks_statement = cass_statement_new(ks_query, 0);
                CassFuture_ *ks_future = cass_session_execute(session, ks_statement);
                if(cass_future_error_code(ks_future) != CASS_OK) {
                    cout << "Impossible de créer le Keyspace" << endl;
                    const char *message;
                    size_t message_length;
                    cass_future_error_message(ks_future, &message, &message_length);
                    fprintf(stderr, "Unable to run query: '%.*s'\n", (int) message_length, message);
                }
                keyspaceMeta = cass_schema_meta_keyspace_by_name(schemaMeta, "BGP_KEYSPACE");
                cass_statement_free(ks_statement);
            }
            const CassTableMeta_ *tableMeta = cass_keyspace_meta_table_by_name(keyspaceMeta, "KEY_VALUE");
            if(!tableMeta) {
                const char *tab_query = "CREATE TABLE KEY_VALUE (\n"
                                        " key TEXT,\n"
                                        " value TEXT,\n"
                                        " timestamp INT,\n"
                                        " PRIMARY KEY (key)\n"
                                        " );";
                CassStatement_ *tab_statement = cass_statement_new(tab_query, 0);
                CassFuture_ *tab_future = cass_session_execute(session, tab_statement);
                if(cass_future_error_code(tab_future) != CASS_OK) {
                    cout << "Impossible de créer la Table" << endl;
                    const char *message;
                    size_t message_length;
                    cass_future_error_message(tab_future, &message, &message_length);
                    fprintf(stderr, "Unable to run query: '%.*s'\n", (int) message_length, message);
                }
                cass_statement_free(tab_statement);
            }
            cass_schema_meta_free(schemaMeta);*/

        try {
            //##### Connexion à la base Redis #####
                ConnectionOptions connection_options;
                connection_options.host = redis_host;
                connection_options.port = redis_port;
                redis = Redis(connection_options);

            //##### Ajout de données à la base Redis #####
            for (int i = 0; i < 500; i++) {
                string set_name = (i < 200) ? "set1" : ((i < 400) ? "set2" : "set3");
                //Pour l'instant, i est utilisé comme score. On utilisera le Timestamp fourni par BGP dans la version finale
                add_key_value(set_name, to_string(i), "val"+to_string(i), i);
            }
        } catch (const Error &e) {
            cout << e.what() << endl;
        }

        //##### Fin de la connexion à la base Cassandra #####
            CassFuture *close_future = cass_session_close(session);
            cass_future_wait(close_future);
            cass_future_free(close_future);

        //##### Libération des variables Cassandra #####
            cass_future_free(connect_future);
            cass_cluster_free(cluster);
            cass_session_free(session);

        return 0;
    }

    // Cette méthode ajoute un couple clé-valeur à la base Redis.
    // La clé est ajoutée, avec un timestamp, à un ensemble Redis.
    // Si la taille de l'ensemble dépasse la taille limite d'un ensemble, alors on déplace une donnée vers Cassandra
    void add_key_value(string set, string key, string value, int timestamp) {
        //##### Suppression des données en trop #####
        while(redis.zcount(set, UnboundedInterval<double>{}) >= MAX_SET_SIZE) {
            vector<string> data; //Recherche de la donnée à supprimer
            redis.zrange(set, 0, 0, inserter(data, data.begin()));
            if(data.size()>0) {
                //## Récupération de la donnée ##
                    const char* old_key = data.at(0).c_str();
                    Optional<basic_string<char>> optionnal_old_value = redis.get(old_key);
                    const char* old_value = (optionnal_old_value) ? optionnal_old_value->c_str() : "";
                    Optional<double> optionnal_old_timestamp = redis.zscore(set, old_key);
                    int old_timestamp = (optionnal_old_timestamp) ? *optionnal_old_timestamp : 0;
                    cout << "Suppression de : " << old_key << endl;
                //## Suppression de Redis ##
                    redis.zremrangebyrank(set, 0, 0); //Suppression de la clé dans l'ensemble Redis
                    redis.del(old_key); //Suppression du couple clé-valeur dans la base Redis

                //## Ajout à Cassandra ##
                    const char *query = "INSERT INTO BGP_KEYSPACE.KEY_VALUE (key, value, timestamp) VALUES (?,?,?)";
                    CassStatement *statement = cass_statement_new(query, 3);
                    cass_statement_bind_string(statement, 0, old_key);
                    cass_statement_bind_string(statement, 1, old_value);
                    cass_statement_bind_int(statement, 2, old_timestamp);
                    CassFuture *result_future = cass_session_execute(session, statement);
                    if (cass_future_error_code(result_future) == CASS_OK) {
                        cout << "La donnée (key:" << old_key
                             <<",value:" << old_value
                             <<",timestamp:" << old_timestamp
                             <<") a bien été transférée vers Cassandra." << endl;
                    } else {
                        const char *message;
                        size_t message_length;
                        cass_future_error_message(result_future, &message, &message_length);
                        fprintf(stderr, "Unable to run query: '%.*s'\n", (int) message_length, message);
                    }
            }
        }

        //##### Ajout de données #####
        redis.zadd(set, key, timestamp); //Ajout de la nouvelle clé à l'ensemble
        redis.set(key, value); //Ajout du nouveau couple clé-valeur dans la base
    }

    //Call the right cass_statement_bind_intX depending on the size of integer
    void cass_statement_bind_int(CassStatement_ *statement, size_t index, int integer) {
        cass_statement_bind_int32(statement, index, integer);
    }
}