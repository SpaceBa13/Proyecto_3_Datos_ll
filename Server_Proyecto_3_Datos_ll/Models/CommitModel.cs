namespace Server_Proyecto_3_Datos_ll.Models
{
    public class CommitModel
    {
        public string? id { get; set; }

        public string message { get; set; }
        public List<string> changes { get; set; }

    }
}
