using Microsoft.AspNetCore.Mvc;
using Server_Proyecto_3_Datos_ll.Models;
using Server_Proyecto_3_Datos_ll.Classes;

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
            MD5_generator md5_generator = new MD5_generator();
            commit_model.id = md5_generator.CalculateMD5(commit_model.message);
            Console.WriteLine();


            // Supongamos que la operación fue exitosa
            return Ok($"Commit realizado con exito {commit_model.id}");
        }
    }
}
