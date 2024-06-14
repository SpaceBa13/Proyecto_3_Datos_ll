#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> stagedFiles; // Almacenar archivos pendientes de los commits
int commitCounter = 0; 

void help() {
#include "guit.h"
#include <iostream>

    void showHelp();
    std::cout << "Available commands:\n";

    std::cout << "                      Inicializar un nuevo repositorio con el nombree\n";
    std::cout << "                      Instancia un nuevo repositorio en el servidor y lo identifica con el nombre indicado por <name>.\n";
    std::cout << "  init                Adem�s, crea cualquier estructura de datos necesaria para llevar el control del lado del cliente\n";
    std::cout << "                      sobre cu�les archivos est�n siendo controlados por el server y cu�les no. Debe crear un archivo\n";
    std::cout << "                      llamado .guitignore que permite especificar cu�les archivos son ignorados por el control de \n";
    std::cout << "                      versiones. El servidor almacena esta informaci�n tambi�n.\n\n";

    std::cout << "  help                Mostrar este mensaje de ayuda\n";
    std::cout << "                      Este comando va a mostrar en la consola la informaci�n de lo que hace cada comando en guit.\n\n";

    std::cout << "                      Agregar archivos al repositorio\n";
    std::cout << "                      Permite agregar todos los archivos que no est�n registrados o que tengan nuevos cambios al repo-\n";
    std::cout << "  add                 sitorio. Ignora los archivos que est�n configurados en .guitignore. El usuario puede indicar cada\n";
    std::cout << "                      archivo por agregar, o puede usar el flag -A para agregar todos los archivos relevantes. Cuando\n";
    std::cout << "                      los archivos se agregan, se marcan como pendientes de commit.\n\n";

    std::cout << "                      Confirmar cambios con un mensaje\n";
    std::cout << "                      Env�a los archivos agregados y pendientes de commit al server. Se debe especificar un mensaje a\n";
    std::cout << "  commit <message>    la hora de hacer el commit. El server recibe los cambios, y cuando ha terminado de procesar los\n";
    std::cout << "                      cambios, retorna un id de commit al cliente generado con MD5. El server debe verificar que el \n";
    std::cout << "                      commit del cliente est� al d�a con el resto de cambios realizados por otros usuarios. En caso \n";
    std::cout << "                      de que no sea as�, rechaza el commit.\n\n";

    std::cout << "                      Mostrar el estado de los archivos\n";
    std::cout << "  status <file>       Este comando nos va a mostrar cu�les archivos han sido cambiados, agregados o eliminados de \n";
    std::cout << "                      acuerdo al commit anterior. Si el usuario especifica <file>, muestra el historial de cambios, \n";
    std::cout << "                      recuperando el historial de cambios desde el server.\n\n";

    std::cout << "                      Revertir un archivo a una confirmaci�n espec�fica\n";
    std::cout << "  rollback            Permite regresar un archivo en el tiempo a un commit espec�fico. Para esto, se comunica al server\n";
    std::cout << "                      y recupera el archivo hasta dicha versi�n.\n\n";

    std::cout << "  reset <file>        Restablecer los cambios de un archivo a la �ltima confirmaci�n\n";
    std::cout << "                      Deshace cambios locales para un archivo y lo regresa al �ltimo commit.\n\n";

    std::cout << "                      Sincronizar un archivo con el servidor\n";
    std::cout << "  sync <file>         Recupera los cambios para un archivo en el server y lo sincroniza con el archivo en el cliente.\n";
    std::cout << "                      Si hay cambios locales, debe permitirle de alguna forma, que el usuario haga merge de los cambios \n";
    std::cout << "                      interactivamente. \n\n";
}

void initRepo(const std::string& name) {
    std::cout << "Repositorio '" << name << "' inicializado.\n";
}

void addFiles(const std::string& options, const std::string& name) {
    if (options == "-A") {
        std::cout << "Todos los archivos agregados.\n";
        // Simulaci�n
        stagedFiles.push_back("archprue1.txt");
        stagedFiles.push_back("archprue2.txt");
    }
    else {
        std::cout << "Archivo '" << name << "' agregado.\n";
        stagedFiles.push_back(name);
    }
}

std::string generarCommitId() {
    return "commit_" + std::to_string(++commitCounter); // se genera un id basado en el commit que se hace
}

void commitChanges(const std::string& message) {
    if (stagedFiles.empty()) {
        std::cout << "No hay archivos pendientes de commit.\n";
        return;
    }

    std::cout << "Comenzando commit...\n";
    std::cout << "Mensaje del commit: " << message << "\n";

    // Generar un ID de commit �nico basado en el contador
    std::string commitId = generarCommitId();
    std::cout << "Commit ID: " << commitId << "\n";

    // archivos que se commitean
    std::cout << "Archivos a commitear:\n";
    for (const auto& file : stagedFiles) {
        std::cout << "  " << file << "\n";
    }

    // Limpiar archivos staged
    stagedFiles.clear();

    std::cout << "Commit realizado nivel GOD.\n";
}

void showStatus(const std::string& file) {
   
}

void rollbackFile(const std::string& file, const std::string& commit) {
   
}

void resetFile(const std::string& file) {
 
}

void syncFile(const std::string& file) {
   
}
