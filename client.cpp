#include "client.h"

int Client::client_uid = 0;
Client::Client(int fd)
{
    uid = ++client_uid;
    setSocket(fd);
    connected = false;
    gone = false;
}
void Client::setSocket(int fd)
{
    this->fd = fd;
}

int Client::getSocket()const
{
    return fd;
}

int Client::getUid()const
{
    return uid;
}

std::string Client::getUsername()const
{
    return username;
}

void Client::setUsername(const std::string &username)
{
    this->username = username;
}

bool Client::isConnected()const
{
    return connected;
}

void Client::setConnected(bool flag)
{
    connected = flag;
}

bool Client::isGone()const
{
    return gone;
}

void Client::setGone()
{
    gone = true;
}
