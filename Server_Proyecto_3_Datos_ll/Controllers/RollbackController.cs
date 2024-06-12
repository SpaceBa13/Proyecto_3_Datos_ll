using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using System.Security.Cryptography;
using Server_Proyecto_3_Datos_ll.Models;

namespace Server_Proyecto_3_Datos_ll.Controllers
{
    [ApiController]
    [Route("[controller]")]

    public class RollbackController : ControllerBase
    {
        // Este es el método POST que recibe un string
        [HttpGet(Name = "rollback")]
        public IActionResult Rollback([FromBody] RollbackModel rollback_model)
        {

            // Supongamos que la operación fue exitosa
            return Ok("Rollback realizado con exito");
        }
    }
}
