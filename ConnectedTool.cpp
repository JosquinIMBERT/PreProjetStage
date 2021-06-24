#include <cassandra.h>
#include <iostream>
#include <sw/redis++/redis++.h>

#include "ConnectedTool.h"

using namespace sw::redis;
using namespace std;

void ConnectedTool::init_connections(int argc, char **argv) {
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
    cass_future_free(connect_future);

    //##### Connexion à la base Redis #####
    try {
        ConnectionOptions connection_options;
        connection_options.host = redis_host;
        connection_options.port = redis_port;
        redis = Redis(connection_options);
    } catch (const Error &e) {
        cerr << e.what() << endl;
    }
}

void ConnectedTool::end_connections() {
    //##### Fin de la connexion à la base Cassandra #####
    CassFuture *close_future = cass_session_close(session);
    cass_future_wait(close_future);
    cass_future_free(close_future);

    //##### Libération des variables Cassandra #####
    cass_cluster_free(cluster);
    cass_session_free(session);
}