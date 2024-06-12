using Microsoft.AspNetCore.Mvc;
using Server_Proyecto_3_Datos_ll.Models;
using Server_Proyecto_3_Datos_ll.Classes;
using Microsoft.AspNetCore.Routing.Constraints;
using MySql.Data.MySqlClient;
using Server_Proyecto_3_Datos_ll.MySQLModels;
using System.Data;

namespace Server_Proyecto_3_Datos_ll.Controllers
{
    [ApiController]
    [Route("[controller]")]

    public class SyncController : ControllerBase
    {
        // Este es el método POST que recibe un string
        [HttpGet(Name = "Sync")]
        public IActionResult Sync([FromBody] SyncModel sync_model)
        {
            List<MySQLChangesModel> archivos = new List<MySQLChangesModel>();
            //Archivo a buscar en la base de datos
            string filename = sync_model.file;
            ConnectionMySQL MySql_connection = new ConnectionMySQL();
            MySql_connection.create_connection_to_MySQL();

            try
            {
                string query = "SELECT id, nombre, contenido FROM Archivos WHERE nombre = @filename";
                MySqlCommand sqlCommand = new MySqlCommand(query, MySql_connection.connection);
                sqlCommand.Parameters.AddWithValue("@filename", filename);

                using (MySqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        archivos.Add(new MySQLChangesModel
                        {
                            id_commit = reader.GetString("id"),
                            filename = reader.GetString("filename"),
                            filename_difference = reader.GetString("filename_diferencia"),
                            datetime = reader.GetDateTime("fecha_cambio")
                        });
                    }
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, $"Error interno del servidor: {ex.Message}");
            }

            // Supongamos que la operación fue exitosa
            return Ok($"Sincronizacion realizada con exito");
        }
    }
}
