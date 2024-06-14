using Microsoft.AspNetCore.Mvc;
using Server_Proyecto_3_Datos_ll.Models;
using Server_Proyecto_3_Datos_ll.Classes;
using MySql.Data.MySqlClient;
using System.Data.Common;
using Server_Proyecto_3_Datos_ll.MySQLModels;
using Mysqlx.Crud;

namespace Server_Proyecto_3_Datos_ll.Controllers
{
    [ApiController]
    [Route("[controller]")]

    public class CommitController : ControllerBase
    {
        // Este es el método POST que recibe un string
        [HttpPost(Name = "commit")]
        public IActionResult Commit([FromBody] CommitModel commit_model)
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
                    //Si no se encontraron commits escribe el contenido en la base de datos
                    if (Write_Commit_in_MySQL(commit_model))
                    {
                        //Si el commit se escribio con exito, se escriben los cambios
                        Write_Changes_in_MySQL(commit_model);
                    }
                    //Retorna el id del commit generado
                    return Ok(commit_model);

                }
                reader.Close();

                //Verifica si los cambios estan al dia
                foreach (var last_commit_change in last_commit.changes)
                {
                    foreach (var incoming_commit_change in commit_model.changes)
                    {
                        if (last_commit_change.filename == incoming_commit_change.filename)
                        {
                            if (last_commit_change.datetime > incoming_commit_change.datetime)
                            {
                                //Si se encontro una version mas nueva en la base de datos, retorna un error
                                return StatusCode(500, "Cambios pendientes");
                            }
                        }
                    }
                }
                //Si no se encontraron commits escribe el contenido en la base de datos
                if (Write_Commit_in_MySQL(commit_model))
                {
                    //Si el commit se escribio con exito, se escriben los cambios
                    Write_Changes_in_MySQL(commit_model);
                }
                //Retorna el id del commit generado
                return Ok(commit_model);
            }
            catch (Exception){}
            return StatusCode(500, "Error");
        }

        public bool Write_Changes_in_MySQL(CommitModel commit_model)
        {
            foreach (var change in commit_model.changes)
            {
                ConnectionMySQL MySql_connection = new ConnectionMySQL();
                MySql_connection.create_connection_to_MySQL();
                string insert_change_query = "INSERT INTO Cambios (id_commit, filename, filename_diferencia) VALUES(@ID_COMMIT, @FILENAME, @FILENAME_DIF);";
                MySqlCommand sql_command_insert_change = new MySqlCommand(insert_change_query, MySql_connection.connection);
                sql_command_insert_change.Parameters.AddWithValue("@ID_COMMIT", commit_model.id);
                sql_command_insert_change.Parameters.AddWithValue("@FILENAME", change.filename);
                sql_command_insert_change.Parameters.AddWithValue("@FILENAME_DIF", change.filename_difference);
                int rowsAffected_by_change = sql_command_insert_change.ExecuteNonQuery();
                MySql_connection.close_connection_to_MySQL();
            }
            return true;
        }
        public bool Write_Commit_in_MySQL(CommitModel commit_model)
        {
            ConnectionMySQL MySql_connection = new ConnectionMySQL();
            MySql_connection.create_connection_to_MySQL();
            MD5_generator md5_generator = new MD5_generator();
            commit_model.id = md5_generator.CalculateMD5(commit_model.message);
            string insert_commit_query = "INSERT INTO Commits(id_commit, id_repositorio, mensaje, autor) VALUES(@ID, @ID_REPO, @MESSAGE, @AUTOR);";
            MySqlCommand sql_command = new MySqlCommand(insert_commit_query, MySql_connection.connection);
            sql_command.Parameters.AddWithValue("@ID", commit_model.id);
            sql_command.Parameters.AddWithValue("@ID_REPO", commit_model.id_repositorio);
            sql_command.Parameters.AddWithValue("@MESSAGE", commit_model.message);
            sql_command.Parameters.AddWithValue("@AUTOR", commit_model.autor);
            // Ejecutar el comando
            int rowsAffected = sql_command.ExecuteNonQuery();
            MySql_connection.close_connection_to_MySQL();
            return true;
        }
    }
}
