
#ifndef GAME__H
    #define GAME__H

    #include "network/server.hpp"
    #include "game/party.hpp"
    #include "game/rules.hpp"
    #include "json.hpp"

    #include <string>
    #include <memory>
    #include <vector>

namespace Game {

    class Game {
    private:
        Party               *party;
        Network::Server     *server;    //TCP client/server management

        int dispatch_msg(std::vector<nlohmann::json*> *datas,
            nlohmann::json *data);

    public:
        Game(Network::Server *server);
        virtual ~Game();

        //Pseudo state pattern
        void start();
    };

}
#endif

