#include "socket.hh"

#include <iostream>
#include <cstdlib>
#include <span>
#include <string>


void get_URL(const std::string &host, const std::string &path)
{
    std::cerr << "Function called: get_URL(" << host << ", " << path << ")\n";
    // TCPSocket sock{};
}

int main(int argc, char *argv[])
{
    try
    {
        auto args = std::span(argv, argc);

        if (argc != 3)
        {
            std::cerr << "Usage: " << args.front() << " HOST PATH\n";
            std::cerr << "\t Example: " << args.front() << " github.com/superbignut/Tcp-Ip-cpp\n";
            return EXIT_FAILURE;
        }

        const std::string host{args[1]};
        const std::string path{args[2]};

        get_URL(host, path);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Todo cerr span
