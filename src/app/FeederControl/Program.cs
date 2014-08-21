using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FeederControl
{
    class Program
    {
        static void Main()
        {
            // mimics Arduino calling structure
            var printController = new PrintController("COM5");

            Random rand = new Random((int) DateTime.Now.Ticks & 0x0000FFFF);

            while (true)
            {
                for (int i = 0; i < 8; i++)
                {
                    printController.Print((ushort)rand.Next(5), (ushort)rand.Next(30));

                }

                while (PrintController.JobCompleteCount < 8)
                {
                    System.Threading.Thread.Sleep(500);
                }
            }
        }

    }
}
