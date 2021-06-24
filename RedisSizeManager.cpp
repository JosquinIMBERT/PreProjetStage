#include <cassandra.h>
#include <iostream>
#include <sw/redis++/redis++.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <signal.h>

#include "RedisSizeManager.h"

using namespace sw::redis;
using namespace std;


void RedisSizeManager::setSets(std::vector<std::string> sets) {
    this->sets = sets;
}

void RedisSizeManager::interrupt() {
    this->stop = 1;
}

void RedisSizeManager::start() {
    RedisSizeManager::start(0, NULL);
}

void RedisSizeManager::start(int argc, char **argv) {
    init_connections(argc, argv);

    int i = 0;
    while(!stop) {
        string set_name = sets.at(i);
        int nb_to_del = redis.zcount(set_name, UnboundedInterval<double>{}) - MAX_SET_SIZE;
        // Trouver les données à transférer
        vector<string> data; //Recherche de la donnée à supprimer
        if(nb_to_del>0) {
            redis.zrange(set_name, 0, nb_to_del, inserter(data, data.begin()));
            cout << "Suppression de " << nb_to_del << " éléments de l'ensemble "<< set_name << endl;
        }

        for(auto key : data) {
            // Récupérer la donnée à transférer
            const char *old_key = key.c_str();
            Optional<basic_string<char>> optionnal_old_value = redis.get(old_key);
            const char *old_value = (optionnal_old_value) ? optionnal_old_value->c_str() : "";
            Optional<double> optionnal_old_timestamp = redis.zscore(set_name, old_key);
            int old_timestamp = (optionnal_old_timestamp) ? *optionnal_old_timestamp : 0;

            //Supprimer la donnée de l'ensemble Redis
            redis.zremrangebyrank(set_name, 0, 0); //Suppression de la clé dans l'ensemble Redis
            redis.del(old_key); //Suppression du couple clé-valeur dans la base Redis

            //Ajouter la donnée à Cassandra
            const char *query = "INSERT INTO BGP_KEYSPACE.KEY_VALUE (key, value, timestamp) VALUES (?,?,?)";
            CassStatement *statement = cass_statement_new(query, 3);
            cass_statement_bind_string(statement, 0, old_key);
            cass_statement_bind_string(statement, 1, old_value);
            cass_statement_bind_int32(statement, 2, old_timestamp);
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

            //TODO Ajouter un indicateur de la suppression dans Redis


        }
        i = (i+1) % sets.size();
    }

    end_connections();
}