#include <iostream>
#include <string>
#include "guit.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No command provided.\n";
        help();
        return 1;
    }

    std::string command = argv[1];

    if (command == "help") {
        help();
    }
    else if (command == "init") {
        if (argc < 3) {
            std::cerr << "No repository name provided for init.\n";
            return 1;
        }
        std::string name = argv[2];
        initRepo(name);
    }
    else if (command == "add") {
        if (argc < 3) {
            std::cerr << "No file name or options provided for add.\n";
            return 1;
        }
        std::string options = argv[2];
        std::string name = (argc > 3) ? argv[3] : "";
        addFiles(options, name);
    }
    else if (command == "commit") {
        if (argc < 3) {
            std::cerr << "No commit message provided.\n";
            return 1;
        }
        std::string message = argv[2];
        commitChanges(message);
    }
    else if (command == "status") {
        std::string file = (argc > 2) ? argv[2] : "";
        showStatus(file);
    }
    else if (command == "rollback") {
        if (argc < 4) {
            std::cerr << "Not enough arguments provided for rollback.\n";
            return 1;
        }
        std::string file = argv[2];
        std::string commit = argv[3];
        rollbackFile(file, commit);
    }
    else if (command == "reset") {
        if (argc < 3) {
            std::cerr << "No file provided for reset.\n";
            return 1;
        }
        std::string file = argv[2];
        resetFile(file);
    }
    else if (command == "sync") {
        if (argc < 3) {
            std::cerr << "No file provided for sync.\n";
            return 1;
        }
        std::string file = argv[2];
        syncFile(file);
    }
    else {
        std::cerr << "Unknown command or incorrect usage.\n";
        help();
        return 1;
    }

    return 0;
}
