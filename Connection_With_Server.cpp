#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client features
using namespace std;
using namespace utility::conversions;

class Connection_With_Server {
public:
    // Constructor para inicializar el cliente HTTP con la URL base
    Connection_With_Server(const std::wstring& base_url) : client_(base_url) {}

    // Función para inicializar un repositorio en el servidor
    void init_repo_in_server(std::string& id_repo, const std::string& repo_name, const std::string& description) {
        // Construir la URI y comenzar la solicitud
        uri_builder builder(U("/InitRepo/"));

        // Crear el objeto JSON a enviar
        json::value init_repo_model;
        init_repo_model[U("repo_name")] = json::value::string(utility::conversions::to_string_t(repo_name));
        init_repo_model[U("description")] = json::value::string(utility::conversions::to_string_t(description));

        // Hacer la solicitud POST
        pplx::task<void> requestTask = client_.request(methods::POST, builder.to_string(), init_repo_model.serialize(), U("application/json"))
            .then([this, &id_repo](http_response response) {
            this->get_id_repo(response, id_repo);
                });

        // Esperar a que la solicitud termine
        try {
            requestTask.wait();
        }
        catch (const std::exception& e) {
            std::wcout << U("Error: ") << e.what() << std::endl;
        }
    }

    // Función para realizar un commit en el servidor
    void commit_in_server(const json::value& commit, std::string& id_commit) {
        // Construir la URI y comenzar la solicitud
        uri_builder builder(U("/Commit/"));

        // Hacer la solicitud POST
        pplx::task<void> requestTask = client_.request(methods::POST, builder.to_string(), commit.serialize(), U("application/json"))
            .then([this, &id_commit](http_response response) {
            this->get_id_commit(response, id_commit);
                });

        // Esperar a que la solicitud termine
        try {
            requestTask.wait();
        }
        catch (const std::exception& e) {
            std::wcout << U("Error: ") << e.what() << std::endl;
        }
    }

    // Función para realizar un rollback en el servidor
    void rollback_in_server(const json::value& rollback, json::value& changes) {
        // Construir la URI y comenzar la solicitud
        uri_builder builder(U("/Rollback/"));

        // Hacer la solicitud POST
        pplx::task<void> requestTask = client_.request(methods::POST, builder.to_string(), rollback.serialize(), U("application/json"))
            .then([this, &changes](http_response response) {
            this->get_changes_rollback(response, changes);
                });

        // Esperar a que la solicitud termine
        try {
            requestTask.wait();
        }
        catch (const std::exception& e) {
            std::wcout << U("Error: ") << e.what() << std::endl;
        }
    }

    // Función para realizar un rollback en el servidor
    void status_in_server(const json::value& status, json::value& changes) {
        // Construir la URI y comenzar la solicitud
        uri_builder builder(U("/Status/"));
        // Hacer la solicitud POST
        pplx::task<void> requestTask = client_.request(methods::POST, builder.to_string(), status.serialize(), U("application/json"))
            .then([this, &changes](http_response response) {
            this->get_status_changes(response, changes);
                });

        // Esperar a que la solicitud termine
        try {
            requestTask.wait();
        }
        catch (const std::exception& e) {
            std::wcout << U("Error: ") << e.what() << std::endl;
        }
    }

    // Función para obtener el contenido del archivo desde el servidor
    string getFileContent(const std::string& file_name) {
        std::string resultado;
        http_client client(U("http://localhost:7092"));
        uri_builder builder(U("Sync"));
        builder.append_query(U("file"), utility::conversions::to_string_t(file_name));

        client.request(methods::GET, builder.to_string()).then([&resultado](http_response response) {
            if (response.status_code() == status_codes::OK) {
                return response.extract_string();
            }
            else {
                std::cerr << "Error: " << response.status_code() << std::endl;
                return pplx::task_from_result(utility::string_t());
            }
            }).then([&resultado](pplx::task<utility::string_t> previousTask) {
                try {
                    resultado = utility::conversions::to_utf8string(previousTask.get());
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception: " << e.what() << std::endl;
                }
                }).wait();

                return resultado;
    }

private:
    // Cliente HTTP para realizar las solicitudes
    http_client client_;

    // Función para manejar la respuesta HTTP y obtener el ID del repositorio
    void get_id_repo(http_response response, std::string& id_repo) {
        if (response.status_code() == status_codes::OK) {
            auto json_task = response.extract_json();
            try {
                json::value v = json_task.get();
                if (v.has_field(U("id"))) {
                    id_repo = utility::conversions::to_utf8string(v.at(U("id")).as_string());
                    std::wcout << U("Repositorio iniciado con éxito, su ID es: ") << v.at(U("id")).as_string() << std::endl;
                }
                else {
                    std::wcout << U("La respuesta no contiene la etiqueta 'id'.") << std::endl;
                }
            }
            catch (const http_exception& e) {
                std::wcout << U("Error al obtener la respuesta JSON: ") << e.what() << std::endl;
            }
        }
        else {
            std::wcout << U("Error en la solicitud POST. Código de estado: ") << response.status_code() << std::endl;
        }
    }

    // Función para manejar la respuesta HTTP y obtener el ID del commit
    void get_id_commit(http_response response, std::string& id_commit) {
        if (response.status_code() == status_codes::OK) {
            auto json_task = response.extract_json();
            try {
                json::value v = json_task.get();
                if (v.has_field(U("id"))) {
                    id_commit = utility::conversions::to_utf8string(v.at(U("id")).as_string());
                    std::wcout << U("Commit realizado con éxito, su ID es: ") << v.at(U("id")).as_string() << std::endl;
                }
                else {
                    std::wcout << U("La respuesta no contiene la etiqueta 'id'.") << std::endl;
                }
            }
            catch (const http_exception& e) {
                std::wcout << U("Error al obtener la respuesta JSON: ") << e.what() << std::endl;
            }
        }
        else {
            std::wcout << U("Error en la solicitud POST. Código de estado: ") << response.status_code() << std::endl;
        }
    }

    // Función para manejar la respuesta HTTP y obtener los cambios realizados en el rollback
    void get_changes_rollback(http_response response, json::value& changes) {
        if (response.status_code() == status_codes::OK) {
            auto json_task = response.extract_json();
            try {
                json::value changes_from_server = json_task.get();
                changes = changes_from_server;
                std::wcout << U("Rollback realizado con éxito. Cambios realizados: ") << changes_from_server << std::endl;
            }
            catch (const http_exception& e) {
                std::wcout << U("Error al extraer JSON de la respuesta: ") << e.what() << std::endl;
            }
        }
        else {
            std::wcout << U("Error en la solicitud POST. Código de estado: ") << response.status_code() << std::endl;
        }
    }

    // Función para manejar la respuesta HTTP y obtener los cambios realizados en el rollback
    void get_status_changes(http_response response, json::value& changes) {
        if (response.status_code() == status_codes::OK) {
            auto json_task = response.extract_json();
            try {
                json::value changes_from_server = json_task.get();
                changes = changes_from_server;
                std::wcout << U("Status obtenido con éxito. Cambios realizados: ") << changes_from_server << std::endl;
            }
            catch (const http_exception& e) {
                std::wcout << U("Error al extraer JSON de la respuesta: ") << e.what() << std::endl;
            }
        }
        else {
            std::wcout << U("Error en la solicitud POST. Código de estado: ") << response.status_code() << std::endl;
        }
    }
};