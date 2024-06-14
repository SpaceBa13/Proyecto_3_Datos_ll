#include <iostream>
#include <string>
#include "guit.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client features
using namespace std;



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

void init_repo_in_server(string& id_repo, string repo_name, string description) {
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



// Funci�n para eliminar espacios en blanco al inicio y al final de una cadena
string noespacios(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}



void printUsage() {


    cout << "Available commands:\n";
    cout << "  help\n";
    cout << "  init <repository_name>\n";
    cout << "  add <-A | filename>\n";
    cout << "  commit <message>\n";
    cout << "  status <filename>\n";
    cout << "  rollback <filename> <commit_id>\n";
    cout << "  reset <filename>\n";
    cout << "  sync <filename>\n";
    cout << "Enter command (type 'exit' to quit): ";
}

   

int main(int argc, char* argv[]) {
    bool exitProgram = false;

    while (!exitProgram) {
        printUsage();
        string input;
        getline(cin, input); // Leer la entrada del usuario

        //eliminar espacios en blanco al inicio y al final
        input = noespacios(input);

        // Analizar el comando ingresado
        if (input == "help") {
            help();
        }
        else if (input.find("init ") == 0) {
            string name = input.substr(5); // Obtener el nombre del repositorio
            initRepo(name);
        }
        else if (input.find("add ") == 0) {
            string args = input.substr(4); // Obtener argumentos para add
            size_t spacePos = args.find(' ');
            string options = args.substr(0, spacePos);
            string name = (spacePos != string::npos) ? args.substr(spacePos + 1) : "";
            addFiles(options, name);
        }
        else if (input.find("commit ") == 0) {
            string message = input.substr(7); // Obtener el mensaje del commit
            commitChanges(message);
        }
        else if (input.find("status ") == 0) {
            string file = input.substr(7); // Obtener el nombre del archivo para status
            showStatus(file);
        }
        else if (input.find("rollback ") == 0) {
            string args = input.substr(9); // Obtener argumentos para rollback
            size_t spacePos = args.find(' ');
            string file = args.substr(0, spacePos);
            string commit = (spacePos != string::npos) ? args.substr(spacePos + 1) : "";
            rollbackFile(file, commit);
        }
        else if (input.find("reset ") == 0) {
            string file = input.substr(6); // Obtener el nombre del archivo para reset
            resetFile(file);
        }
        else if (input.find("sync ") == 0) {
            string file = input.substr(5); // Obtener el nombre del archivo para sync
            syncFile(file);
        }
        else if (input == "exit") {
            exitProgram = true;
        }
        else {
            cerr << "Unknown command or incorrect usage.\n";
        }
    }

    // Mensaje al final antes de cerrar
    cout << "\nPresiona Enter para cerrar la consola...";
    cin.get(); // Espera a que el usuario presione Enter

    return 0;
}
