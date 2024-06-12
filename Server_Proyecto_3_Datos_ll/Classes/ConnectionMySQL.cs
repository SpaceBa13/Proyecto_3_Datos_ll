using MySql.Data.MySqlClient;

namespace Server_Proyecto_3_Datos_ll.Classes
{
    public class ConnectionMySQL
    {
        string connection_stirng = "server=localhost;uid=root;pwd=root;database=github";
        public MySqlConnection connection = new MySqlConnection();

        public void create_connection_to_MySQL()
        {
            connection.ConnectionString = connection_stirng;
            connection.Open();
        }
    }
}
