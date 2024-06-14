#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "guit.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;
using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client feature
namespace fs = std::experimental::filesystem;


void createRepoFiles(const string& repoName, const string& description) {
    // Definir la ruta del directorio
    string basePath = "C:\\Users\\DELL\\Desktop\\Repositorios";
    string repoPath = basePath + "\\" + repoName;

    // Crear el directorio del repositorio
    fs::create_directories(repoPath);

    // Crear el archivo info.txt y escribir en él
    ofstream infoFile(repoPath + "\\info.txt");
    if (infoFile.is_open()) {
        infoFile << "Repo Name: " << repoName << endl;
        infoFile << "Description: " << description << endl;
        infoFile.close();
    }
    else {
        cerr << "Error al crear info.txt" << endl;
    }

    // Crear el archivo .gitignore
    ofstream gitignoreFile(repoPath + "\\.gitignore");
    if (gitignoreFile.is_open()) {
        gitignoreFile.close();
    }
    else {
        cerr << "Error al crear .gitignore" << endl;
    }
}

int main(int argc, char* argv[]) {

    // Crear el cliente HTTP
    http_client client(U("https://localhost:7092"));

    // Construir la URI y comenzar la solicitud
    uri_builder builder(U("/InitRepo/"));



    // Crear el objeto JSON a enviar
    json::value postData;
    postData[U("repo_name")] = json::value::string(U("Proyecto 1"));
    postData[U("description")] = json::value::string(U("Primer proyecto"));



    // Hacer la solicitud POST
    pplx::task<void> requestTask = client.request(methods::POST, builder.to_string(), postData.serialize(), U("application/json"))
        .then([](http_response response) {
        // Manejar la respuesta
        if (response.status_code() == status_codes::OK) {
            std::wcout << U("POST exitoso. Código de estado: ") << response.status_code() << std::endl;
            return response.extract_json();
        }
        else {
            std::wcout << U("Error en la solicitud POST. Código de estado: ") << response.status_code() << std::endl;
            return pplx::task_from_result(json::value());
        }
            })
        .then([](pplx::task<json::value> previousTask) {
                try {
                    const json::value& v = previousTask.get();
                    std::wcout << U("Respuesta: ") << v.serialize() << std::endl;
                }
                catch (const http_exception& e) {
                    std::wcout << U("Error al obtener la respuesta JSON: ") << e.what() << std::endl;
                }
            });

            // Esperar a que la solicitud termine
            try {
                requestTask.wait();
            }
            catch (const std::exception& e) {
                std::wcout << U("Error: ") << e.what() << std::endl;
            }

            // Crear la carpeta y los archivos del repositorio
            createRepoFiles("Proyecto 1", "Primer proyecto");


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
                if (argc < 3) {
                    std::cerr << "No file provided for status.\n";
                    return 1;
                }
                std::string file = argv[2];
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