using System;
using System.Security.Cryptography;
using System.Text;

namespace Server_Proyecto_3_Datos_ll.Classes
{
    public class MD5_generator
    {
        public string CalculateMD5(string input)
        {
            // Crear una instancia de la clase Random
            Random random = new Random();
            int randomNumber = random.Next(0, 100001);
            input += randomNumber.ToString();

            // Convierte la cadena de entrada en un arreglo de bytes
            byte[] inputBytes = Encoding.UTF8.GetBytes(input);

            // Crea un objeto MD5
            using (MD5 md5 = MD5.Create())
            {
                // Calcula el hash MD5 de los bytes
                byte[] hashBytes = md5.ComputeHash(inputBytes);

                // Convierte el hash a una cadena hexadecimal
                StringBuilder sb = new StringBuilder();
                foreach (byte b in hashBytes)
                {
                    sb.Append(b.ToString("x2"));
                }
                return sb.ToString();
            }
        }
    }
}