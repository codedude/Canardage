
#include "utils.hpp"
#include "network/server.hpp"
#include "game/game.hpp"

#include <stdexcept>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>

void handle(int signal)
{
    if(signal != SIGINT)
        return ;
}

int main(int argc, char **argv)
{
    if(argc < 4) {
        log_warn("Usage : ./canardage IP PORT MODE");
        exit(1);
    }
    signal(SIGWINCH, handle);

    //Init random generator
    uint_least64_t random = 0;
    ssize_t result = time(NULL);
    int randomData = open("/dev/random", O_RDONLY);
    if(randomData > 0) {
        result = read(randomData, &random, sizeof(uint_least64_t));
        if(result < 0)
            result = time(NULL);
        close(randomData);
    }
    srand((unsigned) random);

	Network::Server *server = nullptr;
	try {
		server = new Network::Server(argv[1], argv[2], 6, argv[3]);
	}catch(const std::invalid_argument& e) {
        log_errs("Error Server init : %s", e.what());
    }

    Game::Game game(server);
    game.start();

	delete server;

	return 0;
}

