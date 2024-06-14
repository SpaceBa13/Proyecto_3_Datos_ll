#include <iostream>
#include <string>
#include "guit.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;
using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client features


// Función para manejar la respuesta HTTP
void get_id_repo(http_response response, string& ID_Repo) {
    if (response.status_code() == status_codes::OK) {
        wcout << U("POST exitoso. Código de estado: ") << response.status_code() << endl;
        auto json_task = response.extract_json();
        try {
            json::value v = json_task.get();
            // Extraer el valor de la etiqueta "ID_Repo" y almacenarlo en la variable ID_Repo
            if (v.has_field(U("id"))) {
                ID_Repo = utility::conversions::to_utf8string(v.at(U("id")).as_string());
                wcout << U("Repositorio iniciado con existo, su id es: ") << v.at(U("id")).as_string() << endl;
            }
            else {
                wcout << U("La respuesta no contiene la etiqueta 'ID_Repo'.") << endl;
            }
        }
        catch (const http_exception& e) {
            std::wcout << U("Error al obtener la respuesta JSON: ") << e.what() << endl;
        }
    }
    else {
        std::wcout << U("Error en la solicitud POST. Código de estado: ") << response.status_code() << endl;
    }
}

void init_repo_in_server(string &id_repo, string repo_name, string description) {
    // Crear el cliente HTTP
    http_client client(U("https://localhost:7092"));

    // Construir la URI y comenzar la solicitud
    uri_builder builder(U("/InitRepo/"));

    // Crear el objeto JSON a enviar
    json::value init_repo_model;
    init_repo_model[U("repo_name")] = json::value::string(utility::conversions::to_string_t(repo_name));
    init_repo_model[U("description")] = json::value::string(utility::conversions::to_string_t(description));

    // Hacer la solicitud POST
    pplx::task<void> requestTask = client.request(methods::POST, builder.to_string(), init_repo_model.serialize(), U("application/json"))
        .then([&id_repo](http_response response) { // Captura por referencia
        get_id_repo(response, id_repo);
            });

    // Esperar a que la solicitud termine
    try {
        requestTask.wait();
    }
    catch (const std::exception& e) {
        std::wcout << U("Error: ") << e.what() << std::endl;
    }

}



int main(int argc, char* argv[]) {

    string id_repo;
    string repo_name = "Repo1";
    string description = "Ola putos";

    init_repo_in_server(id_repo, repo_name, description);

    cout << id_repo << endl;















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
