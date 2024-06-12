using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using System.Security.Cryptography;
using Server_Proyecto_3_Datos_ll.Models;

namespace Server_Proyecto_3_Datos_ll.Controllers
{
    [ApiController]
    [Route("[controller]")]

    public class InitRepoController : ControllerBase
    {
        // Este es el método POST que recibe un string
        [HttpPost(Name = "init_repo")]
        public IActionResult InitRepo([FromBody] InitRepoModel repo_model)
        {
            string repo_name = repo_model.repo_name;
            if (string.IsNullOrEmpty(repo_name))
            {
                return BadRequest("El nombre del repositorio no puede estar vacío.");
            }

            ConnectionModel MySql_connection = new ConnectionModel();
            MySql_connection.create_connection_to_MySQL();

            try
            {
                string query = "INSERT INTO Persona(Nombre, Apellido, Edad) VALUES (@Nombre, @Apellido, @Edad)";
                MySqlCommand sql_command = new MySqlCommand(query, MySql_connection.connection);
                sql_command.Parameters.AddWithValue("@Nombre", repo_name);
                sql_command.Parameters.AddWithValue("@Apellido", "ATI");
                sql_command.Parameters.AddWithValue("@Edad", 25);


                // Ejecutar el comando
                int rowsAffected = sql_command.ExecuteNonQuery();

                Console.WriteLine($"Número de registros eliminados: {rowsAffected}");
            }
            catch (Exception) { 

            }

            // Aquí puedes añadir tu lógica para inicializar el repositorio
            // Por ejemplo, crear un nuevo directorio o inicializar un repositorio Git

            // Supongamos que la operación fue exitosa
            return Ok($"Repositorio '{repo_name}' inicializado correctamente.");
        }

    }
}
