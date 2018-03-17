#include "mediaserver.h"
#include <QTcpServer>

MediaServer::MediaServer()
    : server(new QTcpServer)
{

}
