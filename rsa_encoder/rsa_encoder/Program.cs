using System.Numerics;

public class Program
{
    public static uint CRC32(String content)
    {
        uint crc = 0xFFFFFFFF;

        char[] values = content.ToCharArray(); // przekształć na tablicę charów (tablicę wartości 8-bitowych)
        uint bound = (uint)values.Length; // zwróć długość tablicy 
        uint it = 0; // pomocnicza zmienna na iterowanie po tablicy

        while (bound > it)
        {
            byte currentByte = (byte)values[it]; // pobierz bajt
            crc = crc ^ currentByte; // 

            for (byte bit = 8; bit > 0; --bit)
            {
                crc = (crc >> 1) ^ (uint)(0xEDB88320 & (-(crc & 1))); // 0xEDB88320 to oficjalny dzielnik dla standardu CRC32

            }
            it++; // przejdź do następnego bitu
        }

        return crc ^ 0xFFFFFFFF; // odwróć wszystkie bity hasza wyjściowego 

    }
    public static void Main()
    {
        Console.WriteLine(CRC32("hello world"));
    }
}