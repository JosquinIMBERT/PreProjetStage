#include <cassandra.h>
#include <iostream>
#include <sw/redis++/redis++.h>

#ifndef PREPROJETSTAGE_CONNECTEDTOOL_H
#define PREPROJETSTAGE_CONNECTEDTOOL_H


class ConnectedTool {
public:
    void init_connections(int argc, char **argv);
    void end_connections();

protected:
    sw::redis::Redis redis = sw::redis::Redis("tcp://127.0.0.1:6379");
    CassCluster *cluster;
    CassSession *session;

};


#endif //PREPROJETSTAGE_CONNECTEDTOOL_H
