using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using System.Security.Cryptography;
using Server_Proyecto_3_Datos_ll.Models;
using Server_Proyecto_3_Datos_ll.Classes;
using Mysqlx;

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

            ConnectionMySQL MySql_connection = new ConnectionMySQL();
            MySql_connection.create_connection_to_MySQL();

            MD5_generator md5_generator = new MD5_generator();
            string id_rep_md5 = md5_generator.CalculateMD5(repo_model.repo_name);

            try
            {
                string query = "INSERT INTO Repositorio(id_repositorio, nombre, descripcion) VALUES (@ID, @Nombre, @Descripcion)";
                MySqlCommand sql_command = new MySqlCommand(query, MySql_connection.connection);
                sql_command.Parameters.AddWithValue("@ID", id_rep_md5);
                sql_command.Parameters.AddWithValue("@Nombre", repo_name);
                sql_command.Parameters.AddWithValue("@Descripcion", repo_model.description);

                // Ejecutar el comando
                int rowsAffected = sql_command.ExecuteNonQuery();

                Console.WriteLine($"Número de registros eliminados: {rowsAffected}");
            }
            catch (Exception) { 
                
            }

            InitResponseModel response = new InitResponseModel();
            response.description = repo_model.description;
            response.repo_name = repo_name;
            response.ID = id_rep_md5;

            // Supongamos que la operación fue exitosa
            return Ok(response);
        }

    }
}
