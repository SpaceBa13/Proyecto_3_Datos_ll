#include <iostream>
#include <string>
#include <vector>
#include "guit.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;
using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client features


vector<string> stagedFiles; // Almacenar archivos pendientes de los commits
int commitCounter = 0; 

void help() {


    void showHelp();
    cout << "Available commands:\n";

    cout << "                      Inicializar un nuevo repositorio con el nombree\n";
    cout << "                      Instancia un nuevo repositorio en el servidor y lo identifica con el nombre indicado por <name>.\n";
    cout << "  init                Adem�s, crea cualquier estructura de datos necesaria para llevar el control del lado del cliente\n";
    cout << "                      sobre cu�les archivos est�n siendo controlados por el server y cu�les no. Debe crear un archivo\n";
    cout << "                      llamado .guitignore que permite especificar cu�les archivos son ignorados por el control de \n";
    cout << "                      versiones. El servidor almacena esta informaci�n tambi�n.\n\n";

    cout << "  help                Mostrar este mensaje de ayuda\n";
    cout << "                      Este comando va a mostrar en la consola la informaci�n de lo que hace cada comando en guit.\n\n";

    cout << "                      Agregar archivos al repositorio\n";
    cout << "                      Permite agregar todos los archivos que no est�n registrados o que tengan nuevos cambios al repo-\n";
    cout << "  add                 sitorio. Ignora los archivos que est�n configurados en .guitignore. El usuario puede indicar cada\n";
    cout << "                      archivo por agregar, o puede usar el flag -A para agregar todos los archivos relevantes. Cuando\n";
    cout << "                      los archivos se agregan, se marcan como pendientes de commit.\n\n";

    cout << "                      Confirmar cambios con un mensaje\n";
    cout << "                      Env�a los archivos agregados y pendientes de commit al server. Se debe especificar un mensaje a\n";
    cout << "  commit <message>    la hora de hacer el commit. El server recibe los cambios, y cuando ha terminado de procesar los\n";
    cout << "                      cambios, retorna un id de commit al cliente generado con MD5. El server debe verificar que el \n";
    cout << "                      commit del cliente est� al d�a con el resto de cambios realizados por otros usuarios. En caso \n";
    cout << "                      de que no sea as�, rechaza el commit.\n\n";

    cout << "                      Mostrar el estado de los archivos\n";
    cout << "  status <file>       Este comando nos va a mostrar cu�les archivos han sido cambiados, agregados o eliminados de \n";
    cout << "                      acuerdo al commit anterior. Si el usuario especifica <file>, muestra el historial de cambios, \n";
    cout << "                      recuperando el historial de cambios desde el server.\n\n";

    cout << "                      Revertir un archivo a una confirmaci�n espec�fica\n";
    cout << "  rollback            Permite regresar un archivo en el tiempo a un commit espec�fico. Para esto, se comunica al server\n";
    cout << "                      y recupera el archivo hasta dicha versi�n.\n\n";

    cout << "  reset <file>        Restablecer los cambios de un archivo a la �ltima confirmaci�n\n";
    cout << "                      Deshace cambios locales para un archivo y lo regresa al �ltimo commit.\n\n";

    cout << "                      Sincronizar un archivo con el servidor\n";
    cout << "  sync <file>         Recupera los cambios para un archivo en el server y lo sincroniza con el archivo en el cliente.\n";
    cout << "                      Si hay cambios locales, debe permitirle de alguna forma, que el usuario haga merge de los cambios \n";
    cout << "                      interactivamente. \n\n";
}

void initRepo(const string& name) {
    cout << "Repositorio '" << name << "' inicializado.\n";
}

void addFiles(const string& options, const string& name) {
    if (options == "-A") {
        cout << "Adding all files...\n";
        // Simulaci�n de agregar archivos
        stagedFiles.push_back("file1.txt");
        stagedFiles.push_back("file2.txt");
    }
    else {
        cout << "Adding file: " << name << "\n";
        stagedFiles.push_back(name);
    }
}

string generarCommitId() {
    return "commit_" + to_string(++commitCounter);
}

void commitChanges(const string& message) {
    if (stagedFiles.empty()) {
        cout << "No hay archivos pendientes de commit.\n";
        return;
    }

    cout << "Comenzando commit...\n";
    cout << "Mensaje del commit: " << message << "\n";

    // Generar un ID de commit �nico basado en el contador
    string commitId = generarCommitId();
    cout << "Commit ID: " << commitId << "\n";

    // archivos que se commitean
    cout << "Archivos a commitear:\n";
    for (const auto& file : stagedFiles) {
        cout << "  " << file << "\n";
    }

    // Limpiar archivos staged (si es necesario)
    stagedFiles.clear(); // Limpiamos los archivos despu�s de commitear

    cout << "Commit realizado correctamente.\n";
}

void showStatus(const string& file) {
    // Implementar seg�n sea necesario
}

void rollbackFile(const string& file, const string& commit) {
    // Implementar seg�n sea necesario
}

void resetFile(const string& file) {
    // Implementar seg�n sea necesario
}

void syncFile(const string& file) {
    // Implementar seg�n sea necesario
}