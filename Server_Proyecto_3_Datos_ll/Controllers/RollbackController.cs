using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using System.Security.Cryptography;
using Server_Proyecto_3_Datos_ll.Models;
using Server_Proyecto_3_Datos_ll.Classes;
using Server_Proyecto_3_Datos_ll.MySQLModels;
using System.Reflection.PortableExecutable;

namespace Server_Proyecto_3_Datos_ll.Controllers
{
    [ApiController]
    [Route("[controller]")]

    public class RollbackController : ControllerBase
    {
        [HttpPost(Name = "rollback")]
        public IActionResult Rollback([FromBody] RollbackModel rollback_model)
        {
            ConnectionMySQL MySql_connection = new ConnectionMySQL();
            MySql_connection.create_connection_to_MySQL();

            try
            {
                // Consulta para obtener los cambios de ese commit
                string get_changes_query = "SELECT * FROM Cambios WHERE id_commit = @ID_COMMIT";
                MySqlCommand get_changes_cmd = new MySqlCommand(get_changes_query, MySql_connection.connection);
                get_changes_cmd.Parameters.AddWithValue("@ID_COMMIT", rollback_model.commit_id);

                MySqlDataReader reader = get_changes_cmd.ExecuteReader();

                List<MySQLChangesModel> rollback_changes = new List<MySQLChangesModel>();

                //Obtenemos los cambios relacionados al commit
                while (reader.Read())
                {
                    MySQLChangesModel change = new MySQLChangesModel
                    {
                        id_commit = reader["id_commit"].ToString(),
                        filename = reader["filename"].ToString(),
                        filename_difference = reader["filename_diferencia"].ToString(),
                        datetime = Convert.ToDateTime(reader["fecha_cambio"])
                    };
                    rollback_changes.Add(change);
                }

                reader.Close();
                MySql_connection.connection.Close();

                // Devolver los cambios en formato JSON
                return Ok(rollback_changes);
            }
            catch (Exception ex)
            {
                // Registro del error
                Console.WriteLine($"Error: {ex.Message}");
                return StatusCode(500, "Internal server error");
            }
        }
    }
}
