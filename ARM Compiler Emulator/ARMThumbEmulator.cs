using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace ARMThumb
{
    class ARMThumbEmulator
    {
        public string PATH = @"C:\Users\NoxNo\Desktop\School 2022\Thumb Assembler\";
        public int memorySize = 2048; //2kB
        public bool EndOfProgram;
        short[] registers;
        byte[] memory;
        ushort IR;
        ushort MAR;
        ushort MBR;
        ushort PC;
        byte CCR;

        public ARMThumbEmulator()
        {
            registers = new short[8];
            memory = new byte[memorySize];
            for (int i = 0; i < memorySize; i++)
            {
                memory[i] = 0;
            }

        }

        public int LoadProgram(string programName)
        {
            string line;
            int numberOfBytes;
            try
            {
                StreamReader sr = new StreamReader(PATH + programName + ".txt");
                line = sr.ReadLine();

                numberOfBytes = Convert.ToInt32(line, 10);
                if(numberOfBytes > memorySize)
                {
                    //program to large
                    return -1;
                }

                line = sr.ReadLine();
                int i = 0;
                while(line != null)
                {
                    memory[i] = Convert.ToByte(line, 2);
                    i++;
                    line = sr.ReadLine();
                }
                sr.Close();

            }
            catch (Exception e)
            {
                //Console.WriteLine("Exception: " + e.Message);
            }
            finally
            {
                //Console.WriteLine("Executing finally block.");
            }
            PC = twoBytesToHalfWord(memory[0], memory[1]);

            EndOfProgram = false;
            return 1;
        }

        public void Step()
        {
            
            IR = twoBytesToHalfWord(memory[PC], memory[PC+1]);

            //format 3 THESE NEED TO SET CODES
            
            if((ushort)(Convert.ToUInt16("1111100000000000", 2) & IR) == Convert.ToUInt16("0010000000000000", 2))
            {
                //MOV Rd, #Off8
                ushort reg = Convert.ToUInt16("0000011100000000", 2);
                ushort literal = Convert.ToUInt16("0000000011111111", 2);
                reg &= IR;
                reg >>= 8;
                literal &= IR;

                registers[reg] = (short)literal;

            }
            else if ((ushort)(Convert.ToUInt16("1111100000000000", 2) & IR) == Convert.ToUInt16("0011000000000000", 2))
            {
                //ADD Rd, #Off8
                ushort reg = Convert.ToUInt16("0000011100000000", 2);
                ushort literal = Convert.ToUInt16("0000000011111111", 2);
                reg &= IR;
                reg >>= 8;
                literal &= IR;

                registers[reg] += (short)literal;

            }
            else if ((ushort)(Convert.ToUInt16("1111100000000000", 2) & IR) == Convert.ToUInt16("0011100000000000", 2))
            {
                //SUB Rd, #Off8
                ushort reg = Convert.ToUInt16("0000011100000000", 2);
                ushort literal = Convert.ToUInt16("0000000011111111", 2);
                reg &= IR;
                reg >>= 8;
                literal &= IR;

                registers[reg] -= (short)literal;

            }
            else if ((ushort)(Convert.ToUInt16("111111111111", 2) & IR) == Convert.ToUInt16("111111111111", 2))
            {
                EndOfProgram = true;
            }
            else
            {
                //ERROR UNKNOWN COMAND
                EndOfProgram = true;
            }


            PC += 2;
        }



        public void PrintRegs()
        {
            for(int i = 0; i < 8; i++)
            {
                Console.WriteLine("R" + i + ": " + registers[i]);
            }
        }

        public void PrintMemory()
        {
            for(int i = 0; i < memory.Length; i++)
            {
                Console.WriteLine(i + "\t" + memory[i].ToString() + "\t" + Convert.ToString(memory[i], 2).PadLeft(8, '0'));
            }
        }

        private ushort twoBytesToHalfWord(byte a, byte b)
        {
            return Convert.ToUInt16(Convert.ToString(a, 2).PadLeft(8, '0') + Convert.ToString(b, 2).PadLeft(8, '0'), 2);
        }

    }
}
