#include <iostream>
#include <string>
#include "guit.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "Connection_With_Server.cpp"

using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client features
using namespace std;
using namespace utility::conversions;

//Funcion para obtener el tiempo actual
string get_current_datetime() {
    // Obtener el tiempo actual
    auto now = chrono::system_clock::now();

    // Convertir el tiempo actual a tiempo local
    time_t current_time = chrono::system_clock::to_time_t(now);

    // Crear una estructura tm a partir del tiempo local
    tm time_info;
    localtime_s(&time_info, &current_time); // Para Windows

    // Formatear la fecha y hora según el formato deseado
    char buffer[20]; // Suficiente para "YYYY-MM-DDTHH:MM:SS" + el carácter nulo
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &time_info);

    return string(buffer);
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


json::value build_change(string id_commit, string filename, string filename_difference) {
    // Obtener la fecha y hora actual formateada
    string current_datetime = get_current_datetime();
    json::value change;
    change[U("id_commit")] = json::value::string(to_string_t(id_commit));
    change[U("filename")] = json::value::string(to_string_t(filename));
    change[U("filename_difference")] = json::value::string(to_string_t(filename_difference));
    change[U("datetime")] = json::value::string(to_string_t(current_datetime));
    return change;
}
   

int main(int argc, char* argv[]) {
    Connection_With_Server* connetion_with_server = new Connection_With_Server(U("https://localhost:7092"));
    //Zona de pruebas para las peticiones
    string id_repo;
 




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
    json::value change1 = build_change("commit123", "archivo1.txt", "archivo1_v2.txt");
    changes_array[0] = change1;

    // Segundo cambio
    json::value change2 = build_change("commit123", "archivo2.png", "archivo2_v2.png");
    changes_array[1] = change2;

    // Agregar el array de cambios al objeto principal
    commit_model[U("changes")] = changes_array;

    string id_commit;

    connetion_with_server->commit_in_server(commit_model, id_commit);

    cout << id_commit << endl;

    //ROLLBACK

    json::value changes;
    json::value rollback;

    // Asignar valores al objeto JSON
    rollback[U("file")] = json::value::string(U("archivo2.png"));
    rollback[U("commit_id")] = json::value::string(to_string_t(id_commit));
    connetion_with_server->rollback_in_server(rollback, changes);

    //Status

    string new_current_datetime = get_current_datetime();

     // Construir el JSON según la estructura deseada
    json::value status_commit;
    status_commit[U("id")] = json::value::string(to_string_t("af5c0553c533e1d7b77d0682e21231eb"));
    status_commit[U("id_repositorio")] = json::value::string(to_string_t("2ab1fc6a345199263f7d77315fcb5b63"));
    status_commit[U("message")] = json::value::string(to_string_t("Initial commit"));
    status_commit[U("autor")] = json::value::string(to_string_t("John Doe"));
    status_commit[U("datetime")] = json::value::string(to_string_t(new_current_datetime));

    // Construir el array 'changes'
    json::value status_changes;
    json::value change3 = build_change("af5c0553c533e1d7b77d0682e21231eb", "archivo3.txt", "archivo1_v2.txt");
    json::value change4 = build_change("af5c0553c533e1d7b77d0682e21231eb", "archivo4.png", "archivo2_v2.png");

    status_changes[0] = change3;
    status_changes[1] = change4;

    status_commit[U("changes")] = status_changes;

    // Construir el JSON final
    json::value status_from_server;
    json::value json_final;
    json_final[U("file")] = json::value::string(to_string_t("example.txt"));
    json_final[U("commit")] = status_commit;

    connetion_with_server->status_in_server(json_final, status_from_server);


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
            // Extraer nombre del repositorio y descripción
            string args = input.substr(5); // Eliminar "init "
            size_t spacePos = args.find(' ');
            if (spacePos != string::npos) {
                string repo_name = args.substr(0, spacePos);
                string description = args.substr(spacePos + 1);

                if (!repo_name.empty() && !description.empty()) {
                    initRepo(repo_name);

                    string id_repo;
                    Connection_With_Server* connetion_with_server = new Connection_With_Server(U("https://localhost:7092"));
                    connection_with_server->init_repo_in_server(id_repo, repo_name, description);

                    cout << "Repositorio '" << repo_name << "' inicializado con descripción: " << description << "\n";
                }
                else {
                    cerr << "Error: Nombre del repositorio o descripción no pueden estar vacíos.\n";
                }
            }
            else {
                cerr << "Error: Formato incorrecto. Usa 'init <nombre> <descripcion>'.\n";
            }
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
