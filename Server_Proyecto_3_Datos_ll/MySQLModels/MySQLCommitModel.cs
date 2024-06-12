﻿using Server_Proyecto_3_Datos_ll.MySQLModels;

namespace Server_Proyecto_3_Datos_ll.MySQLModels
{
    public class MySQLCommitModel
    {
        public string id { get; set; }
        public string message { get; set; }
        
        public string autor { get; set; }

        public DateTime datetime { get; set; }

        public List<MySQLChangesModel> changes { get; set; }

    }
}
