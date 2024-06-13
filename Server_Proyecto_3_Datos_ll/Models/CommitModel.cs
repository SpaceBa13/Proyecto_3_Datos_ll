using Server_Proyecto_3_Datos_ll.MySQLModels;

namespace Server_Proyecto_3_Datos_ll.Models
{
    public class CommitModel
    {
        public string? id { get; set; }

        public string? id_repositorio { get; set; }

        public string message { get; set; }

        public string autor { get; set; }

        public List<MySQLChangesModel> changes { get; set; }

    }
}
