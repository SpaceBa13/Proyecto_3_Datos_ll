using Server_Proyecto_3_Datos_ll.MySQLModels;

namespace Server_Proyecto_3_Datos_ll.Models
{
    public class StatusResponseModel
    {
        public List<MySQLChangesModel> updated_files {  get; set; }
        public List<MySQLChangesModel> new_files { get; set; }

        public List<MySQLChangesModel> deleted_files { get; set; }
    }
}
