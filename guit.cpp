#include <iostream>
#include <string>
#include <vector>
#include "guit.h"

using namespace std;

vector<string> stagedFiles; // Almacenar archivos pendientes de los commits
int commitCounter = 0; 

void help() {


    void showHelp();
    cout << "Available commands:\n";

    cout << "                      Inicializar un nuevo repositorio con el nombree\n";
    cout << "                      Instancia un nuevo repositorio en el servidor y lo identifica con el nombre indicado por <name>.\n";
    cout << "  init                Además, crea cualquier estructura de datos necesaria para llevar el control del lado del cliente\n";
    cout << "                      sobre cuáles archivos están siendo controlados por el server y cuáles no. Debe crear un archivo\n";
    cout << "                      llamado .guitignore que permite especificar cuáles archivos son ignorados por el control de \n";
    cout << "                      versiones. El servidor almacena esta información también.\n\n";

    cout << "  help                Mostrar este mensaje de ayuda\n";
    cout << "                      Este comando va a mostrar en la consola la información de lo que hace cada comando en guit.\n\n";

    cout << "                      Agregar archivos al repositorio\n";
    cout << "                      Permite agregar todos los archivos que no estén registrados o que tengan nuevos cambios al repo-\n";
    cout << "  add                 sitorio. Ignora los archivos que estén configurados en .guitignore. El usuario puede indicar cada\n";
    cout << "                      archivo por agregar, o puede usar el flag -A para agregar todos los archivos relevantes. Cuando\n";
    cout << "                      los archivos se agregan, se marcan como pendientes de commit.\n\n";

    cout << "                      Confirmar cambios con un mensaje\n";
    cout << "                      Envía los archivos agregados y pendientes de commit al server. Se debe especificar un mensaje a\n";
    cout << "  commit <message>    la hora de hacer el commit. El server recibe los cambios, y cuando ha terminado de procesar los\n";
    cout << "                      cambios, retorna un id de commit al cliente generado con MD5. El server debe verificar que el \n";
    cout << "                      commit del cliente esté al día con el resto de cambios realizados por otros usuarios. En caso \n";
    cout << "                      de que no sea así, rechaza el commit.\n\n";

    cout << "                      Mostrar el estado de los archivos\n";
    cout << "  status <file>       Este comando nos va a mostrar cuáles archivos han sido cambiados, agregados o eliminados de \n";
    cout << "                      acuerdo al commit anterior. Si el usuario especifica <file>, muestra el historial de cambios, \n";
    cout << "                      recuperando el historial de cambios desde el server.\n\n";

    cout << "                      Revertir un archivo a una confirmación específica\n";
    cout << "  rollback            Permite regresar un archivo en el tiempo a un commit específico. Para esto, se comunica al server\n";
    cout << "                      y recupera el archivo hasta dicha versión.\n\n";

    cout << "  reset <file>        Restablecer los cambios de un archivo a la última confirmación\n";
    cout << "                      Deshace cambios locales para un archivo y lo regresa al último commit.\n\n";

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
        cout << "Todos los archivos agregados.\n";
        // Simulación
        stagedFiles.push_back("archprue1.txt");
        stagedFiles.push_back("archprue2.txt");
    }
    else {
        cout << "Archivo '" << name << "' agregado.\n";
        stagedFiles.push_back(name);
    }
}

string generarCommitId() {
    return "commit_" + to_string(++commitCounter); // se genera un id basado en el commit que se hace
}

void commitChanges(const string& message) {
    if (stagedFiles.empty()) {
        cout << "No hay archivos pendientes de commit.\n";
        return;
    }

    cout << "Comenzando commit...\n";
    cout << "Mensaje del commit: " << message << "\n";

    // Generar un ID de commit único basado en el contador
    string commitId = generarCommitId();
    cout << "Commit ID: " << commitId << "\n";

    // archivos que se commitean
    cout << "Archivos a commitear:\n";
    for (const auto& file : stagedFiles) {
        cout << "  " << file << "\n";
    }

    // Limpiar archivos staged
    stagedFiles.clear();

    cout << "Commit realizado nivel GOD.\n";
}

void showStatus(const string& file) {
   
}

void rollbackFile(const string& file, const string& commit) {
   
}

void resetFile(const string& file) {
 
}

void syncFile(const string& file) {
   
}
/*
int main() {
    help(); // Mostrar mensaje de ayuda al inicio
    return 0;
}
*/