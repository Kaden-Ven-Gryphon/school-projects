using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Text;
using System.IO;


public class ARMThumbCompiler
{

    public string PATH = Application.persistentDataPath + @"/";
    public char COMMENTCHAR = '@';
    public const string ROBOT0 = "BOT0";
    public const string ROBOT1 = "BOTMOVE";
    public const string ROBOT2 = "BOTGRAB";
    public const string ROBOT3 = "BOTLOOK";
    public const string ROBOT4 = "BOT4";
    public const string ROBOT5 = "BOT5";
    public const string ROBOT6 = "BOT6";
    public const string ROBOT7 = "BOT7";

    private List<Label> dataLabels;
    private List<Label> textLabels;
    private List<string> machineCode;
    private int globalAdressCounter = 0;







    class Label
    {
        public string Name;
        public int Adress;
        public string ValBin;
        public int ValInt;
        //might want to add type here too

        public Label()
        {
            Name = "";
            Adress = 0;
            ValBin = "";
            ValInt = 0;
        }
    }



    private void clearLists()
    {
        dataLabels.Clear();
        textLabels.Clear();
        machineCode.Clear();
    }

    

    public ARMThumbCompiler()
    {
        dataLabels = new List<Label>();
        textLabels = new List<Label>();
        machineCode = new List<string>();
        return;
    }

    private string regToMachine(string reg)
    {
        switch (reg)
        {
            case "R0":
                return "000";
            case "R1":
                return "001";
            case "R2":
                return "010";
            case "R3":
                return "011";
            case "R4":
                return "100";
            case "R5":
                return "101";
            case "R6":
                return "110";
            case "R7":
                return "111";
            default:
                return " E UNKNOWN REG";
        }
    }
    private string decodeLiteral(string literal, int bits)
    {
    	
        var sb = new StringBuilder();
        if (literal[0] == '#')
        {
           
            if (literal.Length > 1)
            {
                
                if (literal.Length > 3 && literal[2] == 'x')
                {
                    //IS HEX NUMBER
                }
                else if (literal[1] == '-')
                {
                    sb.Append(" E no negative literals alowed ");
                }
                else
                {
                    int buf = 0;
                    
                    string temp = literal.Substring(1);
                    /*
                    for(int i = 0; i < temp.Length; i++)
                    {
                        if (Char.IsDigit(temp[i])) { }
                        else
                        {
                            
                            temp = temp.Substring(0, i);
                            break;
                        }

                    }
                    */
                    try
                    {
                        buf = Convert.ToInt32(temp, 10);
                    }
                    catch (Exception e) 
                    {
                        Debug.Log("Length of lit: " + literal.Length);
                        Debug.Log("Exception " + e.Message);
                    }

                    
                    
                    
                    if (buf < (int)(Math.Pow(2.0, (double)bits)))
                    {
                       sb.Append(Convert.ToString(buf, 2).PadLeft(bits, '0'));
                       
                    }
                    else
                    {
                    sb.Append(" E BIT OVERFLOW ");
                    }
                    
                }
            }
            else
            {
                sb.Append(" E NO NUMBER WRITEN ");
            }
        }
        else
        {
            sb.Append(" E NOT A LITERAL ");
        }

       
        return sb.ToString();
    }

    private string toMachineString(string line)
    {
        char[] delimiterChars = { ' ', '\t' };
        var sb = new StringBuilder();
        line = line.ToUpper();
        string[] tokens = line.Split(delimiterChars);
        for (int i = 0; i < tokens.Length; i++)
        {
            tokens[0].Trim();
        }
        if (tokens.Length > 0)
        {
            int numberOfTokens = 0;
            bool hasLiteral = false;
            bool hasRefrance = false;
            for (int i = 0; i < tokens.Length; i++)
            {
                if (tokens[i][0] == COMMENTCHAR) break;
                else numberOfTokens += 1;
            }
            for (int i = 0; i < tokens.Length; i++)
            {
                if (tokens[i][0] == '#') hasLiteral = true;
            }
            for (int i = 0; i < tokens.Length; i++)
            {
                if (tokens[i][0] == '=') hasRefrance = true;
            }

            if (numberOfTokens == 3)
            {
                if (tokens[1][tokens[1].Length - 1] == ',')
                {
                    tokens[1] = tokens[1].Substring(0, tokens[1].Length - 1);
                }
                else
                {
                    sb.Append(" E missing comma ");
                    return sb.ToString();
                }

                if (tokens[2][0] == '[')
                {
                    tokens[2] = tokens[2].Substring(1, tokens[2].Length - 2);
                }

            }
            if (numberOfTokens == 4)
            {
                if (tokens[1][tokens[1].Length - 1] == ',')
                {
                    tokens[1] = tokens[1].Substring(0, tokens[1].Length - 1);
                }
                else
                {
                    sb.Append(" E missing comma ");
                    return sb.ToString();
                }
                if (tokens[2][tokens[2].Length - 1] == ',')
                {
                    tokens[2] = tokens[2].Substring(0, tokens[2].Length - 1);
                }
                else
                {
                    sb.Append(" E missing comma ");
                    return sb.ToString();
                }

                if (tokens[2][0] == '[')
                {
                    tokens[2] = tokens[2].Substring(1);
                    tokens[3] = tokens[3].Substring(0, tokens[3].Length - 1);
                }

            }

            //formats done
            //1 , 2 , 3 , 4, 17, 18, 16?, 12?
            //formats to do
            //14 (NEED TO LEARN STACKPOINTER AGAIN), 9h, 7h
            //formants to ignore
            //19 , 15? , 13?, 11, 10, 8, 5, 6


            switch (tokens[0])
            {
                //unfinshed
                case "LSL":
                    if (numberOfTokens == 4)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("00000");
                            sb.Append(decodeLiteral(tokens[3], 5));
                            sb.Append(regToMachine(tokens[2]));//need to chech for errors here
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000010");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }


                    break;

                //UNFINSHED
                case "LSR":
                    if (numberOfTokens == 4)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("00001");
                            sb.Append(decodeLiteral(tokens[3], 5));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000011");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }


                    break;

                case "ASR":
                    if (numberOfTokens == 4)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("00010");
                            sb.Append(decodeLiteral(tokens[3], 5));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000100");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }


                    break;

                case "ADD":
                    if (numberOfTokens == 4)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("0001110");
                            sb.Append(decodeLiteral(tokens[3], 3));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                        else
                        {
                            sb.Append("0001100");
                            sb.Append(regToMachine(tokens[3]));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("00110");
                            sb.Append(regToMachine(tokens[1]));
                            sb.Append(decodeLiteral(tokens[2], 8));
                        }
                    }
                    else
                    {
                        sb.Append(" INVALID NUMBER OF TOKENS ");
                    }


                    break;

                case "SUB":
                    if (numberOfTokens == 4)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("0001111");
                            sb.Append(decodeLiteral(tokens[3], 3));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                        else
                        {
                            sb.Append("0001101");
                            sb.Append(regToMachine(tokens[3]));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("00111");
                            sb.Append(regToMachine(tokens[1]));
                            sb.Append(decodeLiteral(tokens[2], 8));
                        }
                        else
                        {
                            sb.Append(" E INVALID NUMBER OF TOKENS ");
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }


                    break;

                case "MOV":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("00100");
                            sb.Append(regToMachine(tokens[1]));
                            sb.Append(decodeLiteral(tokens[2], 8));
                        }
                        else
                        {
                            sb.Append("0001110000");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));

                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "CMP":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("00101");
                            sb.Append(regToMachine(tokens[1]));
                            sb.Append(decodeLiteral(tokens[2], 8));
                        }
                        else
                        {
                            sb.Append("0100001010");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "AND":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000000");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "EOR":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000001");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "ADC":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000101");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "SBC":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000110");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "ROR":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100000111");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "TST":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100001000");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "NEG":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100001001");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "CMN":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100001011");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "ORR":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100001100");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "MUL":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100001101");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "BIC":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100001110");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case "MVN":
                    if (numberOfTokens == 3)
                    {
                        if (hasLiteral)
                        {

                        }
                        else
                        {
                            sb.Append("0100001111");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E INVALID NUMBER OF TOKENS ");
                    }

                    break;

                case ROBOT0:
                    sb.Append("1101111100000000");
                    break;
                case ROBOT1:
                    sb.Append("1101111100000001");
                    break;
                case ROBOT2:
                    sb.Append("1101111100000010");
                    break;
                case ROBOT3:
                    sb.Append("1101111100000011");
                    break;
                case ROBOT4:
                    sb.Append("1101111100000100");
                    break;
                case ROBOT5:
                    sb.Append("1101111100000101");
                    break;
                case ROBOT6:
                    sb.Append("1101111100000110");
                    break;
                case ROBOT7:
                    sb.Append("1101111100000111");
                    break;

                case "B":
                    if (numberOfTokens == 2)
                    {

                        var lb = textLabels.Find(x => x.Name == (tokens[1] + ":"));
                        if (lb != null)
                        {
                            sb.Append("11100");
                            int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                            if (Math.Abs(offset) <= 2048)
                            {
                                string offBin = Convert.ToString(offset, 2);
                                if (offBin.Length > 11)
                                {
                                    sb.Append(offBin.Substring(offBin.Length - 11));
                                }
                                else
                                {
                                    for (int i = 11 - offBin.Length; i > 0; i--)
                                    {
                                        sb.Append("0");
                                    }
                                    sb.Append(offBin);
                                }
                            }
                            else
                            {
                                sb.Append(" E jump to far for one branch, branch max is 2048 ");
                            }
                        }
                        else
                        {
                            sb.Append(" E label not found ");
                        }
                    }

                    break;

                case "BEQ":
                    if (numberOfTokens == 2)
                    {

                        var lb = textLabels.Find(x => x.Name == (tokens[1] + ":"));
                        if (lb != null)
                        {
                            sb.Append("11010000");
                            int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                            if (Math.Abs(offset) <= 512)
                            {
                                string offBin = Convert.ToString(offset, 2);
                                if (offBin.Length > 8)
                                {
                                    sb.Append(offBin.Substring(offBin.Length - 8));
                                }
                                else
                                {
                                    for (int i = 8 - offBin.Length; i > 0; i--)
                                    {
                                        sb.Append("0");
                                    }
                                    sb.Append(offBin);
                                }
                            }
                            else
                            {
                                sb.Append(" E jump to far for one branch, branch max is 512 ");
                            }
                        }
                        else
                        {
                            sb.Append(" E label not found ");
                        }
                    }
                    break;

                case "BNE":
                    if (numberOfTokens == 2)
                    {

                        var lb = textLabels.Find(x => x.Name == (tokens[1] + ":"));
                        if (lb != null)
                        {
                            sb.Append("11010001");
                            int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                            if (Math.Abs(offset) <= 512)
                            {
                                string offBin = Convert.ToString(offset, 2);
                                if (offBin.Length > 8)
                                {
                                    sb.Append(offBin.Substring(offBin.Length - 8));
                                }
                                else
                                {
                                    for (int i = 8 - offBin.Length; i > 0; i--)
                                    {
                                        sb.Append("0");
                                    }
                                    sb.Append(offBin);
                                }
                            }
                            else
                            {
                                sb.Append(" E jump to far for one branch, branch max is 512 ");
                            }
                        }
                        else
                        {
                            sb.Append(" E label not found ");
                        }
                    }
                    break;

                case "BGE":
                    if (numberOfTokens == 2)
                    {

                        var lb = textLabels.Find(x => x.Name == (tokens[1] + ":"));
                        if (lb != null)
                        {
                            sb.Append("11011010");
                            int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                            if (Math.Abs(offset) <= 512)
                            {
                                string offBin = Convert.ToString(offset, 2);
                                if (offBin.Length > 8)
                                {
                                    sb.Append(offBin.Substring(offBin.Length - 8));
                                }
                                else
                                {
                                    for (int i = 8 - offBin.Length; i > 0; i--)
                                    {
                                        sb.Append("0");
                                    }
                                    sb.Append(offBin);
                                }
                            }
                            else
                            {
                                sb.Append(" E jump to far for one branch, branch max is 512 ");
                            }
                        }
                        else
                        {
                            sb.Append(" E label not found ");
                        }
                    }
                    break;

                case "BLT":
                    if (numberOfTokens == 2)
                    {

                        var lb = textLabels.Find(x => x.Name == (tokens[1] + ":"));
                        if (lb != null)
                        {
                            sb.Append("11011011");
                            int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                            if (Math.Abs(offset) <= 512)
                            {
                                string offBin = Convert.ToString(offset, 2);
                                if (offBin.Length > 8)
                                {
                                    sb.Append(offBin.Substring(offBin.Length - 8));
                                }
                                else
                                {
                                    for (int i = 8 - offBin.Length; i > 0; i--)
                                    {
                                        sb.Append("0");
                                    }
                                    sb.Append(offBin);
                                }
                            }
                            else
                            {
                                sb.Append(" E jump to far for one branch, branch max is 512 ");
                            }
                        }
                        else
                        {
                            sb.Append(" E label not found ");
                        }
                    }
                    break;

                case "BGT":
                    if (numberOfTokens == 2)
                    {

                        var lb = textLabels.Find(x => x.Name == (tokens[1] + ":"));
                        if (lb != null)
                        {
                            sb.Append("11011100");
                            int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                            if (Math.Abs(offset) <= 512)
                            {
                                string offBin = Convert.ToString(offset, 2);
                                if (offBin.Length > 8)
                                {
                                    sb.Append(offBin.Substring(offBin.Length - 8));
                                }
                                else
                                {
                                    for (int i = 8 - offBin.Length; i > 0; i--)
                                    {
                                        sb.Append("0");
                                    }
                                    sb.Append(offBin);
                                }
                            }
                            else
                            {
                                sb.Append(" E jump to far for one branch, branch max is 512 ");
                            }
                        }
                        else
                        {
                            sb.Append(" E label not found ");
                        }
                    }
                    break;

                case "BLE":
                    if (numberOfTokens == 2)
                    {

                        var lb = textLabels.Find(x => x.Name == (tokens[1] + ":"));
                        if (lb != null)
                        {
                            sb.Append("11011101");
                            int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                            if (Math.Abs(offset) <= 512)
                            {
                                string offBin = Convert.ToString(offset, 2);
                                if (offBin.Length > 8)
                                {
                                    sb.Append(offBin.Substring(offBin.Length - 8));
                                }
                                else
                                {
                                    for (int i = 8 - offBin.Length; i > 0; i--)
                                    {
                                        sb.Append("0");
                                    }
                                    sb.Append(offBin);
                                }
                            }
                            else
                            {
                                sb.Append(" E jump to far for one branch, branch max is 512 ");
                            }
                        }
                        else
                        {
                            sb.Append(" E label not found ");
                        }
                    }
                    break;

                case "LDR":
                    if (numberOfTokens == 3)
                    {
                        if (hasRefrance)
                        {
                            //Then this is acutual a format 12 add PC command
                            //This is also modified to be halfword aligned instead of word aligned
                            var lb = dataLabels.Find(x => x.Name == (tokens[2].Substring(1) + ":"));
                            if (lb != null)
                            {
                                sb.Append("10100");
                                sb.Append(regToMachine(tokens[1]));
                                int offset = (-globalAdressCounter + lb.Adress - 2) / 2;
                                if (Math.Abs(offset) <= 512)
                                {
                                    string offBin = Convert.ToString(offset, 2);
                                    if (offBin.Length > 8)
                                    {
                                        sb.Append(offBin.Substring(offBin.Length - 8));
                                    }
                                    else
                                    {
                                        for (int i = 8 - offBin.Length; i > 0; i--)
                                        {
                                            sb.Append("0");
                                        }
                                        sb.Append(offBin);
                                    }
                                }
                                else
                                {
                                    sb.Append(" E jump to far for one branch, branch max is 512 ");
                                }
                            }
                            else
                            {
                                sb.Append(" E label not found ");
                            }
                        }
                        else
                        {
                            //LDR r1, [r1]
                            sb.Append("0110100000");
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else if (numberOfTokens == 4)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("01101");
                            sb.Append(decodeLiteral(tokens[3], 5));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                        else
                        {
                            sb.Append("0101100");
                            sb.Append(regToMachine(tokens[3]));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E invalid number of tokens ");
                    }
                    break;

                case "STR":
                    if (numberOfTokens == 3)
                    {
                        //STR r1, [r1]
                        sb.Append("0110000000");
                        sb.Append(regToMachine(tokens[2]));
                        sb.Append(regToMachine(tokens[1]));
                    }
                    else if (numberOfTokens == 4)
                    {
                        if (hasLiteral)
                        {
                            sb.Append("01100");
                            sb.Append(decodeLiteral(tokens[3], 5));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                        else
                        {
                            sb.Append("0101000");
                            sb.Append(regToMachine(tokens[3]));
                            sb.Append(regToMachine(tokens[2]));
                            sb.Append(regToMachine(tokens[1]));
                        }
                    }
                    else
                    {
                        sb.Append(" E invalid number of tokens ");
                    }
                    break;

                case "PUSH":
                    if (numberOfTokens == 2)
                    {
                        sb.Append("10110100");
                        int reg = Convert.ToInt32(regToMachine(tokens[1]), 2);
                        for (int i = 8 - reg; i > 0; i--)
                        {
                            sb.Append("0");
                        }
                        sb.Append("1");
                        for (int i = reg; i > 0; i--)
                        {
                            sb.Append("0");
                        }
                    }
                    break;

                case "POP":
                    if (numberOfTokens == 2)
                    {
                        sb.Append("10111100");
                        int reg = Convert.ToInt32(regToMachine(tokens[1]), 2);
                        for (int i = 8 - reg; i > 0; i--)
                        {
                            sb.Append("0");
                        }
                        sb.Append("1");
                        for (int i = reg; i > 0; i--)
                        {
                            sb.Append("0");
                        }
                    }
                    break;

                default:
                    //UNKNOWN COMAND
                    sb.Append(" E UNKNOWN COMAND");
                    break;
            }
        }


        return sb.ToString();
    }

    public string Compile(string programName)
    {
    	clearLists();
        int textAdressStart = 0;
        string line;

        Debug.Log("Start compile: " + programName);


        //FIRST PASS TO COLLECT LABELS
        try
        {
            File.Delete(PATH + programName + "ERROR.txt");
            StreamReader sr = new StreamReader(PATH + programName + ".txt");
            StreamWriter sr2 = new StreamWriter(PATH + programName + "ERROR.txt");

            Debug.Log("File Open");

            line = sr.ReadLine();
            bool isData = false;
            int address = 2;
            int lineNumber = 1;
            int dataAddress = 2;

            while (line != null)
            {
                line = line.ToUpper();
                string[] tokens = line.Split(" "[0]);
                for (int i = 0; i < tokens.Length; i++)
                {
                    tokens[0].Trim();
                }
                if (tokens.Length > 0)
                {
                    if (tokens[0][0] != COMMENTCHAR)
                    {
                        if (tokens[0] == ".DATA")
                        {
                            //Console.WriteLine("DATASECTION");
                            isData = true;
                        }
                        else if (tokens[0] == ".TEXT")
                        {
                            //Console.WriteLine("TEXTSECTION");
                            isData = false;
                        }
                        else if (tokens[0][tokens[0].Length - 1] == ':')
                        {
                            if (isData)
                            {
                                if (dataLabels.Find(x => x.Name == tokens[0]) == null)
                                {
                                    int numberOfTokens = 0;
                                    bool hasLiteral = false;
                                    for (int i = 0; i < tokens.Length; i++)
                                    {
                                        if (tokens[i][0] == COMMENTCHAR) break;
                                        else numberOfTokens += 1;
                                    }
                                    for (int i = 0; i < tokens.Length; i++)
                                    {
                                        if (tokens[i][0] == '#') hasLiteral = true;
                                    }


                                    if (numberOfTokens == 2)
                                    {
                                        if (hasLiteral)
                                        {
                                            Label buf = new Label();
                                            buf.Name = tokens[0];
                                            buf.Adress = dataAddress;
                                            dataAddress += 2;
                                            address += 2;
                                            buf.ValBin = decodeLiteral(tokens[1], 16);
                                            //ADD BUF VALINT

                                            dataLabels.Add(buf);

                                            foreach (var tl in textLabels)
                                            {
                                                tl.Adress += 2;
                                            }
                                        }
                                        else
                                        {
                                            sr2.WriteLine("E Line " + lineNumber + " missing value literal");
                                        }
                                    }
                                    else if (numberOfTokens == 3)
                                    {
                                        //This is to alocate array
                                    }
                                    else
                                    {
                                        sr2.WriteLine("E Line " + lineNumber + " invalid tokens");
                                    }
                                }
                                else
                                {
                                    sr2.WriteLine("E Line " + lineNumber + " variable already exists");
                                }
                            }
                            else
                            {
                                if (textLabels.Find(x => x.Name == tokens[0]) == null)
                                {
                                    int numberOfTokens = 0;

                                    for (int i = 0; i < tokens.Length; i++)
                                    {
                                        if (tokens[i][0] == COMMENTCHAR) break;
                                        else numberOfTokens += 1;
                                    }

                                    if (numberOfTokens == 1)
                                    {
                                        Label buf = new Label();
                                        buf.Name = tokens[0];
                                        buf.Adress = address;
                                        //ADD BUF VALINT

                                        textLabels.Add(buf);
                                    }
                                    else
                                    {
                                        sr2.WriteLine("E Line " + lineNumber + " invalid tokens");
                                    }
                                }
                                else
                                {
                                    sr2.WriteLine("E Line " + lineNumber + " label already exists");
                                }
                            }
                        }
                        else
                        {
                            //is comand
                            address += 2;
                        }

                    }
                }
                lineNumber++;
                line = sr.ReadLine();
            }
            textAdressStart = dataAddress;
            sr.Close();
            sr2.Close();
            //Console.ReadLine();
        }
        catch (Exception e)
        {
            Debug.Log("Exception: " + e.Message);
        }
        finally
        {
            //Console.WriteLine("Executing finally block.");
        }


        //SECOND PASS TO CONVERT TO MACHINE
        try
        {
            StreamReader sr = new StreamReader(PATH + programName + ".txt");
            StreamWriter srError = File.AppendText(PATH + programName + "ERROR.txt");

            int address = textAdressStart;
            globalAdressCounter = textAdressStart;
            int lineNumber = 1;

            line = sr.ReadLine();
            while (line != null)
            {
                line = line.ToUpper();
                string[] tokens = line.Split(" "[0]);
                for (int i = 0; i < tokens.Length; i++)
                {
                    tokens[0].Trim();
                }
                if (tokens.Length > 0)
                {
                    if (tokens[0][0] != COMMENTCHAR)
                    {
                        if (tokens[0] == ".DATA")
                        {

                        }
                        else if (tokens[0] == ".TEXT")
                        {

                        }
                        else if (tokens[0][tokens[0].Length - 1] == ':')
                        {

                        }
                        else
                        {
                            string buffer = "";
                            try
                            {
                                //Console.WriteLine(toMachineString(line));
                                buffer = toMachineString(line);

                                
                            }
                            catch (Exception e)
                            {
                                Debug.Log("error in the tomachine line "+ (lineNumber - 1));
                                Debug.Log("Exception: " + e.Message);
                            }
                            if (buffer.Contains("E"))
                            {
                                machineCode.Add("ERROR LINE " + (lineNumber - 1) + ": " + buffer);
                                buffer = buffer.Replace("0", String.Empty);
                                buffer = buffer.Replace("1", String.Empty);
                                srError.WriteLine("ERROR LINE " + (lineNumber - 1) + ": " + buffer);
                            }
                            else
                            {
                                machineCode.Add(buffer);
                            }
                            address += 2;
                            globalAdressCounter += 2;
                        }
                    }
                }
                lineNumber++;
                line = sr.ReadLine();
            }
            sr.Close();
            srError.Close();
            //Console.ReadLine();
        }
        catch (Exception e)
        {
            Debug.Log("error in second pass");
            Debug.Log("Exception: " + e.Message);
        }
        finally
        {
            //Console.WriteLine("Executing finally block.");
        }



        //write machine code file
        try
        {
            File.Delete(PATH + programName + "BIN.txt");
            StreamWriter sw = new StreamWriter(PATH + programName + "BIN.txt");
            int numberOfHalfWords = dataLabels.Count + machineCode.Count + 2;
            sw.WriteLine(numberOfHalfWords.ToString());

            StringBuilder sbFile = new StringBuilder();
            sbFile.Append(Convert.ToString(textAdressStart, 2));
            for (int i = 16 - sbFile.Length; i > 0; i--)
            {
                sbFile.Insert(0, "0");
            }
            if(sbFile.ToString().Length >= 8)
            {
                sw.WriteLine(sbFile.ToString().Substring(0, 8));
                sw.WriteLine(sbFile.ToString().Substring(8));
            }
            else
            {
                Debug.Log("Error line does not have 16 charaters: " + sbFile.ToString());
            }

            foreach (var x in dataLabels)
            {
                if (x.ValBin.ToString().Length >= 8)
                {
                    sw.WriteLine(x.ValBin.Substring(0, 8));
                    sw.WriteLine(x.ValBin.Substring(8));
                }
                else
                {
                    Debug.Log("Error line does not have 16 charaters: " + x.ValBin.ToString());
                }
                
            }

            foreach (var x in machineCode)
            {
                if (x.ToString().Length >= 8)
                {
                    sw.WriteLine(x.Substring(0, 8));
                    sw.WriteLine(x.Substring(8));
                }
                else
                {
                    Debug.Log("Error line does not have 16 charaters: " + x.ToString());
                }
                
            }
            sw.WriteLine("11111111");
            sw.WriteLine("11111111");

            sw.Close();
        }
        catch (Exception e)
        {
            Debug.Log("error in bin write");
            Debug.Log("Exception: " + e.Message);
        }
        finally
        {

        }


        return null;
    }

}



