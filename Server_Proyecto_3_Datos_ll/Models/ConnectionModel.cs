using MySql.Data.MySqlClient;

namespace Server_Proyecto_3_Datos_ll.Models
{
    public class ConnectionModel
    {
        string connection_stirng = "server=localhost;uid=root;pwd=root;database=prueba";
        public MySqlConnection connection = new MySqlConnection();

        public void create_connection_to_MySQL (){
            connection.ConnectionString = connection_stirng;
            connection.Open();
        }
    }
}
