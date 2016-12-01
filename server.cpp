#include "server.h"

Server::Server()
{
    stopped = false;
}

void Server::init()
{
    Logger::log("Server initializing ...");
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        std::cerr << "signal" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    port = SERVER_PORT;
    // getaddrinfo() to get a list of usable addresses
    //std::string host = "localhost";
std::string host = SERVER_IP;
    char service[NI_MAXSERV];
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
    // Work with IPV4/6
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV  ;
    // we could provide a host instead of nullptr
    if(getaddrinfo( nullptr,
                   port.c_str(),
                   &hints,
                   &result) != 0)
    {
        perror("getaddrinfo()");
        std::exit(EXIT_FAILURE);
    }
    listening_socket = create_socket();
    if(listening_socket < 0)
    {
        std::cerr << " a server socket couldn't be created."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    Logger::log("Server initializing ... done !");
}

int Server::create_socket()
{
    Logger::log("creating listening for this server ...");
    int socket_fd = -1;
    addrinfo *rp;
    int optval = 1;
    for( rp = result; rp != nullptr; rp = rp->ai_next)
    {
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(socket_fd == SOCKET_ERROR)
        {
            // on error we try the next address
            continue;
        }
        if(setsockopt(socket_fd, SOL_SOCKET,
                      SO_REUSEADDR,&optval,
                      sizeof(optval)) == SOCKET_ERROR)
        {
            perror("setsockopt");
            return SOCKET_ERROR;
        }
        if(bind(socket_fd,
                rp->ai_addr,
                rp->ai_addrlen) == 0)
        {
            break; // Success !
        }
        close(socket_fd);
    }
    if(rp == nullptr) // could not bind socket to any address of the list
    {
        std::cerr << "Fatal Error : couldn't find a suitable address" << std::endl;
        socket_fd = SOCKET_ERROR;
    }

    // enable socket connexions.
    // make it a socket server
    else if(listen(socket_fd, BACKLOG) == SOCKET_ERROR)
    {
        perror("listen: ");
        socket_fd = SOCKET_ERROR;
    }
    freeaddrinfo(rp);
    Logger::log("creating listening for this server ... done !");
    return socket_fd;
}

void Server::start()
{
    Logger::log("Server started ...");
    socklen_t addrlen;
    sockaddr_storage client_addr;
    //int client_socket_fd;
    addrlen = sizeof(sockaddr_storage);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    int *client_socket_fd = nullptr;
    std::thread userlist_updater{&Server::update_local_list, this};
    userlist_updater.detach();
    while(!stopped)
    {
        client_socket_fd = new int;
        Logger::log("waiting for new Connexion from client ");
        *client_socket_fd = accept(listening_socket, (sockaddr*)&client_addr, &addrlen);
        if(*client_socket_fd == -1)
        {
            perror("accept error");
            continue;
        }
        Logger::log("new connexion caught ...");
        if(getnameinfo((sockaddr*)&client_addr,addrlen, host, NI_MAXHOST, service,
                       NI_MAXSERV, 0) == 0)
        {
            std::thread worker{&Server::client_handler,this, *client_socket_fd};
            worker.detach();
            client_socket_fd = nullptr;
        }
    }
}

void Server::addClient(Client *client)
{
    while(client_shield.try_lock())
    {

    }
    clients.push_back(client);
    client_shield.unlock();

}


Client* Server::getClient(const std::string &username)
{

    Client *client = nullptr;
    for(Client* clt : clients)
    {
        if(clt->getUsername() == username)
        {
            client = clt;
            break;
        }

    }
    return client;
}

Client* Server::getClient(int uid)
{
    Client *client = nullptr;
    for(Client* clt : clients)
    {
        if(clt->getUid() == uid)
        {
            client = clt;
            break;
        }

    }
    return client;
}

void Server::update_userlist(const ControlInfo &info, int sender_uid)
{

}

void Server::update_local_list()
{
    char beat = 1;
    Logger::log(std::string(__FUNCTION__) + " started ...");
    while(!stopped)
    {
        std::this_thread::sleep_for(ms(5000));


        for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); )
        {
            if((*it)->isGone())
            {
                Logger::log((*it)->getUsername() + " left");
                clients.erase(it);
            }


            else
            {
                it++;
            }
        }


        /*
        auto it_new_end =
        std::remove_if(clients.begin(), clients.end(),
                       [](Client *client){
            if(client->isGone())
            {
                Logger::log(client->getUsername() + " left");
            }
            return client->isGone();
        });
        */
        //clients.erase(it_new_end, clients.end());
        std::this_thread::sleep_for(ms(5000));

        std::for_each(clients.begin(), clients.end(),
                      [&beat](Client* client){
            if(write(client->getSocket(), &beat, 1) < 0)
            {
                client->setGone();
            }
        });

    }

}


int Server::removeClient(Client *client)
{
    int ret = -1;
    if(client)
    {
        client_shield.lock();
        client->setGone();
        client_shield.unlock();
        ret = 0;
    }

    return ret;
}

void Server::hearbeat()
{
    /*
    Logger::log("Heartbeat for user uid "
                " started... ");
    char ok = 0;
    int ret = 0;
    Client *client = getClient(client_uid);
    if(client)
    {
        int fd = client->getSocket();
        while(!stopped)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if(write(fd, &ok, 1) < 0)
            {
                ret = -1;
                client->setConnected(false);
                client->setGone();
                Logger::log(client->getUsername() + " is gone");
                break;

            }
        }
    }
    */
}

NeighboorServer* Server::getServer(int pos)
{
    std::lock_guard<std::mutex> lock(server_shield);
    if(pos >= 0 && pos < clients.size())
    {
        return servers.at(pos);
    }
}


void Server::removeServer(int pos)
{
    std::lock_guard<std::mutex> lock(server_shield);
    if(pos >= 0 && pos < servers.size())
    {
        std::vector<NeighboorServer*>::iterator it = servers.begin() + pos;
        servers.erase(it);
    }
}


int Server::sendToClient(int client_uid, void *data, int n)
{

     int count = 0;
     for(Client *client : clients)
     {
         if(client->getUid() == client_uid)
         {
             count = write(client->getSocket(), data, n);
             if(count < 0)
             {
                 perror("SendToClient : ");
                 close(client->getSocket());
             }
             else if(count < n)
             {
                Logger::log("Not all data could be sent");
             }
             else
             {
                 Logger::log("all data could be sent");
             }
             break;
         }
     }
     return count;
}

void Server::client_handler(int socket_fd)
{
    Client client{socket_fd};
    addClient(&client);
    int count = 0;
    char buffer[BUFFER_SIZE] = {0};
    while(!client.isGone())
    {
        count = read(socket_fd, buffer, BUFFER_SIZE);
        if(count < 0)
        {
            client.setGone();
        }
        else
        {
            if (count > 0)
            {
                print_raw_data(buffer, count);
                if(decode_and_process(buffer,
                                   client.getUid()) == -1)
                {
                     client.setGone();
                }
            }
        }
    }
    auto it = std::find(clients.begin(), clients.end(),&client);
    if(it != clients.end())
    {
        Logger::log((*it)->getUsername() + " left");
        clients.erase(it);
    }
    close(socket_fd);
    Logger::log("leaving client handler ...");
}

int Server::decode_and_process(void *data, int sender_uid)
{

    char *ptr = (char*)data;
    LogInOut log;
    Message msg;
    flat_header header;
    int ret = 0;
    memcpy(&header.value, data, sizeof(Header));
    switch(header.header.type)
    {
    case LOGINOUT:
        log = Serialization::Serialize<LogInOut>::deserialize(data);
         ret = process_loginout(log, sender_uid);
        break;
    case MSG:
        msg = Serialization::Serialize<Message>::deserialize(data);
        ret = process_message(std::string(msg.receiver),
                              data, header.header.length
                              + sizeof(Header)
                              + (2*STR_LEN));
        break;
    case CONTROLINFO:
        ret = process_controlInfo_request(data, sender_uid);

        break;        
    }

    ptr = nullptr;
    return ret;
}

void Server::print_raw_data(char *data, int size) const
{
    Logger::log("Printing raw data");
    std::ofstream file;
    file.open("raw_data.log", std::ios::app);

    if(file.is_open())
    {
        if(data)
        {
            for(int i = 0; i < size; i++)
            {
                if(data[i] != 0)
                {
                    file << data[i];
                }
            }
            file << '\n';
        }
    }
    file.close();
}


int Server::process_loginout(LogInOut &log, int sender_uid)
{
    int ret;

    switch (log.header.flags) {
    case (SYN):
        ret = updateClient(std::string(log.username), sender_uid);
        break;
    case (SYN | FIN):
        ret = removeClient(getClient(sender_uid));
    default:
        break;
    }
    return ret;
}


int Server::process_message(const std::string &receiver,
                            void *data, int len)
{
    Client *client = nullptr;
    int count = 1;
    for(Client* clt : clients)
    {
        if(clt->getUsername() == receiver)
        {
            client = clt;
            break;
        }
    }
    if(client)
    {
        count = write(client->getSocket(), data, len);
        if(count < 0)
        {
            perror(__FUNCTION__);

        }
        else
        {
            count = 0;
        }

    }
    // the receiver is may be available from the the server
    else
    {
        Logger::log("user " + receiver + " not found on this server");
        // send message to the next server
        count = 0;
    }
    return count;
}


int Server::process_get_request(int sender_uid)
{
    int ret = 0;
    std::vector<std::string> users;
    for(Client *client : clients)
    {
        if(!client->isGone())
            users.push_back(client->getUsername());
    }
    ControlInfo info = create_controlInfo(users);
    void *data = Serialization::Serialize<ControlInfo>::serialize(info);
    int size =sizeof(Header) + (sizeof(Entry) * info.header.length);
    ret = write(getClient(sender_uid)->getSocket(),
              data,
              size);
    Logger::log("Userlist sent to " +
                getClient(sender_uid)->getUsername());
    return ret;
}

int Server::process_controlInfo_request(void *data,int sender_uid)
{
    Header header;
    int ret = 0;
    if(data)
    {
        memcpy((char*)&header, data, 4);
        if(header.flags == GET) // a user made the request
        {
            ret = process_get_request(sender_uid);
        }
        else if(header.flags == 0) // a server has sent this userlist
        {
            ControlInfo info =
                    Serialization::Serialize<ControlInfo>::deserialize(data);
            update_userlist(info, sender_uid);
        }
    }
    return ret;
}

int Server::updateClient(const std::string &username, int uid)
{
    flat_header flat;

    flat.header.length = 0;
    flat.header.version = VERSION;
    flat.header.type = LOGINOUT;
    flat.header.flags = SYN | ACK;
    Client *client = getClient(username);
    if(client)
    {
        flat.header.flags |= DUP ;
    }
    else
    {
       client = getClient(uid);
       client->setUsername(username);
    }
    int count = write(getClient(uid)->getSocket(), (void*)&flat.header, 4);
    if(count > 0)
        Logger::log(std::string(__FUNCTION__)
                    + " reply sent to client "
                    + username);
    client = nullptr;
    return count;
}
