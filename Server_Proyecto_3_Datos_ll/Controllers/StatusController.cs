using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using Server_Proyecto_3_Datos_ll.Classes;
using Server_Proyecto_3_Datos_ll.Models;
using Server_Proyecto_3_Datos_ll.MySQLModels;
using System.Text.Json;
using System.Xml;

namespace Server_Proyecto_3_Datos_ll.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class StatusController : ControllerBase
    {
        [HttpPost(Name = "status")]
        public IActionResult Status([FromBody] StatusModel status_model)
        {
            ConnectionMySQL MySql_connection = new ConnectionMySQL();
            MySql_connection.create_connection_to_MySQL();

            MySQLCommitModel last_commit = new MySQLCommitModel();

            //Obtenemos el commit mas reciente
            try
            {
                // Consulta para obtener el último commit
                string query = "SELECT * FROM Commits ORDER BY fecha DESC LIMIT 1";
                //Creacion del comando MySQL a ejecutar
                MySqlCommand cmd = new MySqlCommand(query, MySql_connection.connection);
                MySqlDataReader reader = cmd.ExecuteReader();
                //Lee los datos de la base
                if (reader.Read())
                {
                    //Asigna los valores correspondientes
                    last_commit.id = reader["id_commit"].ToString();
                    last_commit.id_repositorio = reader["id_repositorio"].ToString();
                    last_commit.message = reader["mensaje"].ToString();
                    last_commit.autor = reader["autor"].ToString();
                    last_commit.datetime = Convert.ToDateTime(reader["fecha"]);
                    reader.Close();

                    // Consulta para obtener los cambios de ese commit
                    string cambiosQuery = "SELECT * FROM Cambios WHERE id_commit = @id_commit";
                    //Creacion del comando MySQL a ejecutar
                    MySqlCommand cambiosCmd = new MySqlCommand(cambiosQuery, MySql_connection.connection);
                    cambiosCmd.Parameters.AddWithValue("@id_commit", last_commit.id);
                    reader = cambiosCmd.ExecuteReader();

                    last_commit.changes = new List<MySQLChangesModel>();

                    //Obtenemos los cambios relacionados al commit mas reciente
                    while (reader.Read())
                    {
                        //Se crea un modelo para obtener los cmabios desde MySQL
                        MySQLChangesModel change = new MySQLChangesModel
                        {
                            //Se aignan los valores correspondientes
                            id_commit = reader["id_commit"].ToString(),
                            filename = reader["filename"].ToString(),
                            filename_difference = reader["filename_diferencia"].ToString(),
                            datetime = Convert.ToDateTime(reader["fecha_cambio"])
                        };
                        last_commit.changes.Add(change);
                    }
                }
                else
                {
                 //Sin no se encuenrar un commit
                }
                reader.Close();

                List<MySQLChangesModel> updated_files = new List<MySQLChangesModel>();
                List<MySQLChangesModel> new_files = new List<MySQLChangesModel>();
                List<MySQLChangesModel> deleted_files = new List<MySQLChangesModel>();

                //Verifica si se eliminaron archivos y si se actualizaron
                foreach (var last_commit_change in last_commit.changes)
                {
                    bool is_found = false;
                    foreach (var incoming_commit_change in status_model.commit.changes)
                    {
                        if (last_commit_change.filename == incoming_commit_change.filename)
                        {
                            is_found = true;
                            if (last_commit_change.datetime != incoming_commit_change.datetime)
                            {
                                updated_files.Add(last_commit_change);
                            }
                        }
                    }
                    if (is_found == false){
                        deleted_files.Add(last_commit_change);
                    }
                }

                //Verifica hay nuevos archivos
                foreach (var incoming_commit_change in status_model.commit.changes )
                {
                    bool is_found = false;
                    foreach (var last_commit_change in last_commit.changes)
                    {
                        if (last_commit_change.filename == incoming_commit_change.filename)
                        {
                            is_found = true;
                        }
                    }
                    if (is_found == false)
                    {
                        new_files.Add(incoming_commit_change);
                    }
                }


                StatusResponseModel response = new StatusResponseModel();
                response.new_files = new_files;
                response.updated_files = updated_files;
                response.deleted_files = deleted_files;
                // Serializar el objeto a JSON
                var options = new JsonSerializerOptions
                {
                    WriteIndented = true,
                    PropertyNamingPolicy = JsonNamingPolicy.CamelCase
                };

                string changes_json = JsonSerializer.Serialize(response, options);

                return Ok(response);

            }

            catch (Exception) { }
            return StatusCode(500, "Error");

        }
    }
}
