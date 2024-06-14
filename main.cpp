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
using namespace utility::conversions;

//Funcion para obtener el tiempo actual
std::string get_current_datetime() {
    // Obtener el tiempo actual
    auto now = std::chrono::system_clock::now();

    // Convertir el tiempo actual a tiempo local
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    // Crear una estructura tm a partir del tiempo local
    std::tm time_info;
    localtime_s(&time_info, &current_time); // Para Windows

    // Formatear la fecha y hora según el formato deseado
    char buffer[20]; // Suficiente para "YYYY-MM-DDTHH:MM:SS" + el carácter nulo
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &time_info);

    return std::string(buffer);
}


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

// Función para manejar la respuesta HTTP
void get_id_commit(http_response response, string& id_commit) {
    if (response.status_code() == status_codes::OK) {
        wcout << U("POST exitoso. Código de estado: ") << response.status_code() << endl;
        auto json_task = response.extract_json();
        try {
            json::value v = json_task.get();
            // Extraer el valor de la etiqueta "ID_Repo" y almacenarlo en la variable ID_Repo
            if (v.has_field(U("id"))) {
                id_commit = utility::conversions::to_utf8string(v.at(U("id")).as_string());
                wcout << U("Commit realizado con existo, su id es: ") << v.at(U("id")).as_string() << endl;
            }
            else {
                wcout << U("La respuesta no contiene la etiqueta 'id'.") << endl;
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
    init_repo_model[U("repo_name")] = json::value::string(to_string_t(repo_name));
    init_repo_model[U("description")] = json::value::string(to_string_t(description));

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

void commit_in_server(json::value& commit, string &id_commit) {
    // Crear el cliente HTTP
    http_client client(U("https://localhost:7092"));

    // Construir la URI y comenzar la solicitud
    uri_builder builder(U("/Commit/"));

    // Hacer la solicitud POST
    pplx::task<void> requestTask = client.request(methods::POST, builder.to_string(), commit.serialize(), U("application/json"))
        .then([&id_commit](http_response response) {
        get_id_commit(response, id_commit);
            });

    // Esperar a que la solicitud termine
    try {
        requestTask.wait();
    }
    catch (const std::exception& e) {
        std::wcout << U("Error: ") << e.what() << std::endl;
    }
}

// Función para manejar la respuesta HTTP
void get_changes_rollback(http_response response, json::value& changes) {
    if (response.status_code() == status_codes::OK) {
        wcout << U("POST exitoso. Código de estado: ") << response.status_code() << endl;
        auto json_task = response.extract_json();
        try {
            json::value changes_from_server = json_task.get();
            // Asignar el JSON completo de la respuesta a 'changes'
            changes = changes_from_server;
            std::wcout << U("Rollback realizado con exito, cambios realizados: ") << changes_from_server << std::endl;
        }
        catch (const http_exception& error) {
            std::wcout << U("Error al extraer JSON de la respuesta: ") << error.what() << std::endl;
        }
    }
    else {
        std::wcout << U("Error en la solicitud POST. Código de estado: ") << response.status_code() << endl;
    }
}




void rollback_in_server(json::value& rollback, json::value& changes) {
    // Crear el cliente HTTP
    http_client client(U("https://localhost:7092"));

    // Construir la URI y comenzar la solicitud
    uri_builder builder(U("/Rollback/"));

    // Hacer la solicitud POST
    pplx::task<void> requestTask = client.request(methods::POST, builder.to_string(), rollback.serialize(), U("application/json"))
        .then([&changes](http_response response) {
        get_changes_rollback(response, changes);
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

    //Zona de pruebas para las peticiones

    //INIT REPO
    string id_repo;
    string repo_name = "Repo1";
    string description = "Ola putos";

    init_repo_in_server(id_repo, repo_name, description);


    // Obtener la fecha y hora actual formateada
    string current_datetime = get_current_datetime();

    //COMMIT
    cout << id_repo << endl;


    // Crear el objeto JSON a enviar
    json::value commit_model;
    commit_model[U("id")] = json::value::string(to_string_t("commit123"));
    commit_model[U("id_repositorio")] = json::value::string(to_string_t(id_repo));
    commit_model[U("message")] = json::value::string(to_string_t("Implement feature XYZ"));
    commit_model[U("autor")] = json::value::string(to_string_t("John Doe"));
    commit_model[U("datetime")] = json::value::string(to_string_t(current_datetime));

    // Array para almacenar los cambios
    json::value changes_array;

    // Primer cambio
    json::value change1;
    change1[U("id_commit")] = json::value::string(to_string_t("commit123"));
    change1[U("filename")] = json::value::string(to_string_t("archivo1.txt"));
    change1[U("filename_difference")] = json::value::string(to_string_t("archivo1_v2.txt"));
    change1[U("datetime")] = json::value::string(to_string_t(current_datetime));
    changes_array[0] = change1;

    // Segundo cambio
    json::value change2;
    change2[U("id_commit")] = json::value::string(to_string_t("commit123"));
    change2[U("filename")] = json::value::string(to_string_t("archivo2.png"));
    change2[U("filename_difference")] = json::value::string(to_string_t("archivo2_v2.png"));
    change2[U("datetime")] = json::value::string(to_string_t(current_datetime));
    changes_array[1] = change2;

    // Agregar el array de cambios al objeto principal
    commit_model[U("changes")] = changes_array;

    string id_commit;

    commit_in_server(commit_model, id_commit);

    cout << id_commit << endl;

    //ROLLBACK

    json::value changes;

    json::value rollback;

    // Asignar valores al objeto JSON
    rollback[U("file")] = json::value::string(U("archivo2.png"));
    rollback[U("commit_id")] = json::value::string(to_string_t(id_commit));


    rollback_in_server(rollback, changes);











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
