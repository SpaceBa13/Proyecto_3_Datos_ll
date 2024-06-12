using System.Security.Cryptography.Xml;
using System;

namespace Server_Proyecto_3_Datos_ll.MySQLModels
{
    public class MySQLChangesModel
    {
        public string id_commit { get; set; }
        public string filename { get; set; }

        public string filename_difference { get; set; }

        public DateTime datetime { get; set; }

    }
}
