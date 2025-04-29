using System;

namespace ARMThumb
{
    class Program
    {
        static void Main(string[] args)
        {
            ARMThumbCompiler comp = new ARMThumbCompiler();
            //Console.WriteLine(Convert.ToString(-5, 2));
            comp.Compile("test");

            ARMThumbEmulator emulator = new ARMThumbEmulator();
            emulator.LoadProgram("testBIN");
            //emulator.PrintMemory();
            while(emulator.EndOfProgram == false)
            {
                emulator.Step();
                emulator.PrintRegs();
                Console.WriteLine("_________");
            }
        }
    }
}
