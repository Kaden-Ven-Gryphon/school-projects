using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Text;
using System.IO;

public class ARMThumbEmulator
{
    public string PATH = Application.persistentDataPath + @"/";
    public int memorySize = 2048; //2kB
    public bool EndOfProgram;
    public string decodedComand;
    public short[] registers;
    byte[] memory;
    ushort IR;
    ushort MAR;
    ushort MBR;
    public ushort PC;
    public byte CCR;   //xxxxCVNZ

    public int ProgramSize = 0;

 
    public ARMThumbEmulator()
    {
        registers = new short[8];
        memory = new byte[memorySize];
        for (int i = 0; i < memorySize; i++)
        {
            memory[i] = 0;
        }

    }


    public void clearRegs()
    {
        IR = 0;
        MAR = 0;
        MBR = 0;
        CCR = 0;
        PC = 0;

        for (int i = 0; i < 8; i++) { registers[i] = 0; }

        decodedComand = "";
    }
    public void ClearSomeRegs()
    {
        IR = 0;
        MAR = 0;
        MBR = 0;
        CCR = 0;
        PC = 0;
        decodedComand = "";
    }

    public int LoadProgram(string programName)
    {
        ClearSomeRegs();
        string line;
        int numberOfBytes;
        try
        {
            StreamReader sr = new StreamReader(PATH + programName + ".txt");
            line = sr.ReadLine();

            numberOfBytes = Convert.ToInt32(line, 10);
            if (numberOfBytes > memorySize)
            {
                //program to large
                return -1;
            }

            line = sr.ReadLine();
            int i = 0;
            while (line != null)
            {
                memory[i] = Convert.ToByte(line, 2);
                i++;
                line = sr.ReadLine();
            }
            sr.Close();
            ProgramSize = i;
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

    public int Step()
    {
        if (EndOfProgram) return -1;
        int retVal = -1;
        decodedComand = "";
        MAR = PC;
        readHalfword();
        IR = MBR;
        //IR = twoBytesToHalfWord(memory[PC], memory[PC+1]);

        //format 2 THESE NEED TO SET CODES
        if ((ushort)(Convert.ToUInt16("1111111000000000", 2) & IR) == Convert.ToUInt16("0001100000000000", 2))
        {
            //ADD Rd, Rs, Rn
            ushort regN = Convert.ToUInt16("0000000111000000", 2);
            ushort regS = Convert.ToUInt16("0000000000111000", 2);
            ushort regD = Convert.ToUInt16("0000000000000111", 2);

            regN &= IR;
            regN >>= 6;
            regS &= IR;
            regS >>= 3;
            regD &= IR;

            registers[regD] = (short)((short)registers[regS] + (short)registers[regN]);

            CCR = 0;
            if (registers[regD] == 0)
            {
                CCR |= 1;
            }
            if (registers[regD] < 0)
            {
                CCR |= 2;
            }

            decodedComand = "ADD R" + regD.ToString() + ", R" + regS.ToString() + ", R" + regN.ToString();

        }
        else if ((ushort)(Convert.ToUInt16("1111111000000000", 2) & IR) == Convert.ToUInt16("0001110000000000", 2))
        {
            //ADD Rd, Rs, #
            ushort regN = Convert.ToUInt16("0000000111000000", 2);
            ushort regS = Convert.ToUInt16("0000000000111000", 2);
            ushort regD = Convert.ToUInt16("0000000000000111", 2);
            regN &= IR;
            regN >>= 6;
            regS &= IR;
            regS >>= 3;
            regD &= IR;

            registers[regD] = (short)((short)registers[regS] + (short)regN);

            CCR = 0;
            if (registers[regD] == 0)
            {
                CCR |= 1;
            }
            if (registers[regD] < 0)
            {
                CCR |= 2;
            }

            decodedComand = "ADD R" + regD.ToString() + ", R" + regS.ToString() + ", #" + regN.ToString();

        }
        else if ((ushort)(Convert.ToUInt16("1111111000000000", 2) & IR) == Convert.ToUInt16("0001101000000000", 2))
        {
            //SUB Rd, Rs, Rn
            ushort regN = Convert.ToUInt16("0000000111000000", 2);
            ushort regS = Convert.ToUInt16("0000000000111000", 2);
            ushort regD = Convert.ToUInt16("0000000000000111", 2);
            regN &= IR;
            regN >>= 6;
            regS &= IR;
            regS >>= 3;
            regD &= IR;

            registers[regD] = (short)((short)registers[regS] - (short)registers[regN]);

            CCR = 0;
            if (registers[regD] == 0)
            {
                CCR |= 1;
            }
            if (registers[regD] < 0)
            {
                CCR |= 2;
            }

            decodedComand = "SUB R" + regD.ToString() + ", R" + regS.ToString() + ", R" + regN.ToString();

        }
        else if ((ushort)(Convert.ToUInt16("1111111000000000", 2) & IR) == Convert.ToUInt16("0001111000000000", 2))
        {
            //ADD Rd, Rs, #
            ushort regN = Convert.ToUInt16("0000000111000000", 2);
            ushort regS = Convert.ToUInt16("0000000000111000", 2);
            ushort regD = Convert.ToUInt16("0000000000000111", 2);
            regN &= IR;
            regN >>= 6;
            regS &= IR;
            regS >>= 3;
            regD &= IR;

            registers[regD] = (short)((short)registers[regS] - (short)regN);

            CCR = 0;
            if (registers[regD] == 0)
            {
                CCR |= 1;
            }
            if (registers[regD] < 0)
            {
                CCR |= 2;
            }

            decodedComand = "SUB R" + regD.ToString() + ", R" + regS.ToString() + ", #" + regN.ToString();

        }
        else if ((ushort)(Convert.ToUInt16("1111100000000000", 2) & IR) == Convert.ToUInt16("0010000000000000", 2))
        {
            //MOV Rd, #Off8
            ushort reg = Convert.ToUInt16("0000011100000000", 2);
            ushort literal = Convert.ToUInt16("0000000011111111", 2);
            reg &= IR;
            reg >>= 8;
            literal &= IR;

            registers[reg] = (short)literal;
            CCR = 0;

            decodedComand = "MOV R" + reg.ToString() + ", #" + literal.ToString();

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

            CCR = 0;
            if (registers[reg] == 0)
            {
                CCR |= 1;
            }
            if (registers[reg] < 0)
            {
                CCR |= 2;
            }

            decodedComand = "ADD R" + reg.ToString() + ", #" + literal.ToString();

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

            CCR = 0;
            if (registers[reg] == 0)
            {
                CCR |= 1;
            }
            if (registers[reg] < 0)
            {
                CCR |= 2;
            }

            decodedComand = "SUB R" + reg.ToString() + ", #" + literal.ToString();

        }
        else if ((ushort)(Convert.ToUInt16("1111100000000000", 2) & IR) == Convert.ToUInt16("0010100000000000", 2))
        {
            //CMP Rd, #Off8
            ushort reg = Convert.ToUInt16("0000011100000000", 2);
            ushort literal = Convert.ToUInt16("0000000011111111", 2);
            reg &= IR;
            reg >>= 8;
            literal &= IR;

            short buf = registers[reg];
            buf -= (short)literal;

            CCR = 0;
            if (buf == 0)
            {
                CCR |= 1;
            }
            if (buf < 0)
            {
                CCR |= 2;
            }

            decodedComand = "CMP R" + reg.ToString() + ", #" + literal.ToString();

        }
        //Format 4
        else if ((ushort)(Convert.ToUInt16("1111111111000000", 2) & IR) == Convert.ToUInt16("0100001010000000", 2))
        {
            //CMP Rd, #Off8
            ushort reg = Convert.ToUInt16("0000000000111000", 2);
            ushort literal = Convert.ToUInt16("0000000000000111", 2);
            reg &= IR;
            reg >>= 3;
            literal &= IR;

            short buf = registers[reg];
            buf -= registers[literal];

            CCR = 0;
            if (buf == 0)
            {
                CCR |= 1;
            }
            if (buf < 0)
            {
                CCR |= 2;
            }

            decodedComand = "CMP R" + literal.ToString() + ", R" + reg.ToString();

        }
        //Format 9
        else if ((ushort)(Convert.ToUInt16("1111100000000000", 2) & IR) == Convert.ToUInt16("0110000000000000", 2))
        {
            //STR Rd [Rb, #]
            ushort offsetU = Convert.ToUInt16("0000011111000000", 2);
            offsetU &= IR;
            offsetU >>= 6;
            ushort regB = Convert.ToUInt16("0000000000111000", 2);
            regB &= IR;
            regB >>= 3;
            ushort regD = Convert.ToUInt16("0000000000000111", 2);
            regD &= IR;

            string buf = Convert.ToString(offsetU, 2).PadLeft(5, '0');

            if (buf[0] == '1')
            {
                buf = buf.PadLeft(16, '1');
                decodedComand = "STR R" + regD.ToString() + ", [R" + regB.ToString() + ", #" + Convert.ToInt16(buf, 2) * 2 + "]";
                short off = Convert.ToInt16(buf, 2);
                off *= 2;
                off += registers[regB];
                MAR = (ushort)(PC - Math.Abs(off));
                MBR = (ushort)registers[regD];
                writeHalfword();
            }
            else
            {
                buf = buf.PadLeft(16, '0');
                decodedComand = "STR R" + regD.ToString() + ", [R" + regB.ToString() + ", #" + Convert.ToInt16(buf, 2) * 2 + "]";
            }

        }
        //Format 18
        else if ((ushort)(Convert.ToUInt16("1111100000000000", 2) & IR) == Convert.ToUInt16("1110000000000000", 2))
        {
            //B label
            ushort offsetU = Convert.ToUInt16("0000011111111111", 2);
            offsetU &= IR;
            string buf = Convert.ToString(offsetU, 2).PadLeft(11, '0');

            if (buf[0] == '1')
            {
                buf = buf.PadLeft(16, '1');
                decodedComand = "B " + Convert.ToInt16(buf, 2) * 2;
            }
            else
            {
                buf = buf.PadLeft(16, '0');
                decodedComand = "B " + Convert.ToInt16(buf, 2) * 2;
            }

            if (buf[0] == '1')
            {
                buf = buf.PadLeft(16, '1');
                ushort off = Convert.ToUInt16(buf, 2);
                off = (ushort)~off;
                off += 1;
                off *= 2;
                PC -= off;
            }
            else
            {
                buf = buf.PadLeft(16, '0');
                ushort off = Convert.ToUInt16(buf, 2);
                off *= 2;
                PC += off;
            }

        }
        //Format 16
        else if ((ushort)(Convert.ToUInt16("1111111100000000", 2) & IR) == Convert.ToUInt16("1101000000000000", 2))
        {
            //BEQ label
            ushort offsetU = Convert.ToUInt16("0000000011111111", 2);
            offsetU &= IR;
            string buf = Convert.ToString(offsetU, 2).PadLeft(8, '0');

            if (buf[0] == '1')
            {
                buf = buf.PadLeft(16, '1');
                decodedComand = "BNE " + Convert.ToInt16(buf, 2) * 2;
            }
            else
            {
                buf = buf.PadLeft(16, '0');
                decodedComand = "BNE " + Convert.ToInt16(buf, 2) * 2;
            }

            if ((CCR ^ 1) == 0)
            {

                if (buf[0] == '1')
                {
                    buf = buf.PadLeft(16, '1');
                    ushort off = Convert.ToUInt16(buf, 2);
                    off = (ushort)~off;
                    off += 1;
                    off *= 2;
                    PC -= off;
                }
                else
                {
                    buf = buf.PadLeft(16, '0');
                    ushort off = Convert.ToUInt16(buf, 2);
                    off *= 2;
                    PC += off;
                }

            }

        }
        else if ((ushort)(Convert.ToUInt16("1111111100000000", 2) & IR) == Convert.ToUInt16("1101000100000000", 2))
        {
            //BNE label
            ushort offsetU = Convert.ToUInt16("0000000011111111", 2);
            offsetU &= IR;
            string buf = Convert.ToString(offsetU, 2).PadLeft(8, '0');

            if (buf[0] == '1')
            {
                buf = buf.PadLeft(16, '1');
                decodedComand = "BNE " + Convert.ToInt16(buf, 2) * 2;
            }
            else
            {
                buf = buf.PadLeft(16, '0');
                decodedComand = "BNE " + Convert.ToInt16(buf, 2) * 2;
            }


            if ((CCR ^ 1) != 0)
            {

                if (buf[0] == '1')
                {
                    buf = buf.PadLeft(16, '1');
                    ushort off = Convert.ToUInt16(buf, 2);
                    off = (ushort)~off;
                    off += 1;
                    off *= 2;
                    PC -= off;
                }
                else
                {
                    buf = buf.PadLeft(16, '0');
                    ushort off = Convert.ToUInt16(buf, 2);
                    off *= 2;
                    PC += off;
                }

            }

        }
        else if ((ushort)(Convert.ToUInt16("1111111100000000", 2) & IR) == Convert.ToUInt16("1101101100000000", 2))
        {
            //BLT label
            ushort offsetU = Convert.ToUInt16("0000000011111111", 2);
            offsetU &= IR;
            string buf = Convert.ToString(offsetU, 2).PadLeft(8, '0');

            if (buf[0] == '1')
            {
                buf = buf.PadLeft(16, '1');
                decodedComand = "BNE " + Convert.ToInt16(buf, 2) * 2;
            }
            else
            {
                buf = buf.PadLeft(16, '0');
                decodedComand = "BNE " + Convert.ToInt16(buf, 2) * 2;
            }


            if ((CCR ^ 2) != 0)
            {

                if (buf[0] == '1')
                {
                    buf = buf.PadLeft(16, '1');
                    ushort off = Convert.ToUInt16(buf, 2);
                    off = (ushort)~off;
                    off += 1;
                    off *= 2;
                    PC -= off;
                }
                else
                {
                    buf = buf.PadLeft(16, '0');
                    ushort off = Convert.ToUInt16(buf, 2);
                    off *= 2;
                    PC += off;
                }

            }

        }
        //Format 17
        else if ((ushort)(Convert.ToUInt16("1111111100000000", 2) & IR) == Convert.ToUInt16("1101111100000000", 2))
        {
            //ROBOT#
            ushort literal = Convert.ToUInt16("0000000011111111", 2);

            literal &= IR;


            decodedComand = "ROBOT" + literal;

            switch (literal)
            {
                case 0:
                    //Robot0();
                    retVal = 0;
                    break;
                case 1:
                    //Robot1();
                    retVal = 1;
                    break;
                case 2:
                    //Robot2();
                    retVal = 2;
                    break;
                case 3:
                    retVal = 3;
                    break;
                case 4:
                    //Robot4();
                    retVal = 4;
                    break;
                case 5:
                    //Robot5();
                    retVal = 5;
                    break;
                case 6:
                    //Robot6();
                    retVal = 6;
                    break;
                case 7:
                    //Robot7();
                    retVal = 7;
                    break;
            }

        }
        else if ((ushort)(Convert.ToUInt16("1111111111111111", 2) & IR) == Convert.ToUInt16("1111111111111111", 2))
        {
            EndOfProgram = true;
            decodedComand = "END OF PROGRAM";
        }
        else
        {
            //ERROR UNKNOWN COMAND
            EndOfProgram = true;
            decodedComand = "ERROR UNKNOWN COMAND " + IR;
        }


        PC += 2;
        return retVal;
    }



    public void PrintRegs()
    {
        StringBuilder sb = new StringBuilder();

        Console.WriteLine(decodedComand);

        for (int i = 0; i < 8; i++)
        {
            sb.Append((string)(" R" + i).PadRight(8, ' '));
            sb.Append("|");
        }
        sb.Append(" C V N Z |");
        Console.WriteLine(sb.ToString());
        sb.Clear();
        for (int i = 0; i < 8; i++)
        {
            sb.Append(registers[i].ToString().PadLeft(8, ' '));
            sb.Append("|");
        }
        string conCodes = Convert.ToString(CCR, 2).PadLeft(4, '0');
        sb.Append(" " + conCodes[0] + " " + conCodes[1] + " " + conCodes[2] + " " + conCodes[3] + " |");
        Console.WriteLine(sb.ToString());
        Console.WriteLine("__________________________________________________________________________________");
    }

    public void PrintMemory()
    {
        for (int i = 0; i < memory.Length; i++)
        {
            Console.WriteLine(i + "\t" + memory[i].ToString() + "\t" + Convert.ToString(memory[i], 2).PadLeft(8, '0'));
        }
    }

    private ushort twoBytesToHalfWord(byte a, byte b)
    {
        return Convert.ToUInt16(Convert.ToString(a, 2).PadLeft(8, '0') + Convert.ToString(b, 2).PadLeft(8, '0'), 2);
    }

    private void writeHalfword()
    {
        byte b1;
        byte b2;
        ushort s1 = Convert.ToUInt16("1111111100000000", 2);
        ushort s2 = Convert.ToUInt16("0000000011111111", 2);
        s1 &= MBR;
        s2 &= MBR;
        b1 = Convert.ToByte(Convert.ToString(s1, 2).PadLeft(16, '0').Substring(0, 8));
        b2 = Convert.ToByte(Convert.ToString(s2, 2).PadLeft(16, '0').Substring(8));
        memory[MAR] = b1;
        memory[MAR + 1] = b2;
    }

    private void readHalfword()
    {
        MBR = twoBytesToHalfWord(memory[MAR], memory[MAR + 1]);
    }

    public void Robot0()
    {
        Console.WriteLine("Robot 0 Funciton");
    }

    public void Robot1()
    {
        Console.WriteLine("Robot 1 Function MOVE");
    }

    public void Robot2()
    {
        Console.WriteLine("Robot 2 Function GRAB");
    }

    public void Robot3()
    {
        Console.WriteLine("Robot 3 Function LOOK");
    }

    public void Robot4()
    {
        Console.WriteLine("Robot 4 Function");
    }

    public void Robot5()
    {
        Console.WriteLine("Robot 5 Function");
    }

    public void Robot6()
    {
        Console.WriteLine("Robot 6 Function");
    }

    public void Robot7()
    {
        Console.WriteLine("Robot 7 Function");
    }





}
