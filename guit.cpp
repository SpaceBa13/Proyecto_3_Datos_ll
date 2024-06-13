#include "guit.h"
#include <iostream>

void help() {
#include "guit.h"
#include <iostream>

    void showHelp();
    std::cout << "Available commands:\n";

    std::cout << "                      Inicializar un nuevo repositorio con el nombre\n";
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
    std::cout << "  status <file>       Este comando nos va a mostrar cu�les archivos han sido cambiados, agregados o eliminados de acuer-\n";
    std::cout << "                      do al commit anterior. Si el usuario especifica <file>, muestra el historial de cambios, recupe-\n";
    std::cout << "                      rando el historial de cambios desde el server.\n\n";

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
    // Implementar l�gica para los comandos que faktan
}

void addFiles(const std::string& options, const std::string& name) {
    
}

void commitChanges(const std::string& message) {
    

    //yo


}

void showStatus(const std::string& file) {
    
}

void rollbackFile(const std::string& file, const std::string& commit) {
    
}

void resetFile(const std::string& file) {
    
}

void syncFile(const std::string& file) {
   
}
