using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System.IO;
using UnityEngine.SceneManagement;

public class GMscript : MonoBehaviour
{

    ARMThumbCompiler compiler;
    public TextMeshProUGUI inputProgramTxt;
    public TextMeshProUGUI LineNumbers;
    public TextMeshProUGUI ConsoleTxt;
    public TextMeshProUGUI EndScoreTxt;
    public GameObject SorterBot;
    public SortingSelf SortSelf;
    private EmulatedSorterBotControler botComputer;
    public int LevelNumber = 0;
    public GameObject WinPane;
    public GameObject LosePane;

    private float targetWinVal;

    string PATH = "";


    private void Awake()
    {
        PATH = Application.persistentDataPath + @"/";
        Debug.Log("Path is: " + PATH);
        SetLineNumbers(200);
        botComputer = SorterBot.GetComponent<EmulatedSorterBotControler>();
        
    }

    // Start is called before the first frame update
    void Start()
    {
        compiler = new ARMThumbCompiler();
        ResetSim();
    }

    // Update is called once per frame
    void Update()
    {

        if(Input.GetKeyDown(KeyCode.RightControl))
        {
            Scene scene = SceneManager.GetActiveScene();
            SceneManager.LoadScene(scene.buildIndex + 1);
        }
        if (Input.GetKeyDown(KeyCode.RightAlt))
        {
            Scene scene = SceneManager.GetActiveScene();
            SceneManager.LoadScene(scene.buildIndex - 1);
        }

        if (SorterBot.transform.position.x > 4.9f)
        {
            botComputer.state = -1;
        }




        if(botComputer.state == -1)
        {
            
            switch (LevelNumber)
            {
                case 0:
                    break;
                case 1:
                    if (botComputer.emulator.registers[2] == 5)
                    {
                        //level complete
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        //level failed
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 2:
                    if(botComputer.emulator.registers[6] == botComputer.emulator.registers[3])
                    {
                        //level complete
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        //level failed
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 3:
                    if(SorterBot.transform.position.x > -4.2f)
                    {
                        //level complete
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        //level failed
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 4:
                    if (botComputer.emulator.registers[0] == (int)targetWinVal)
                    {
                        //level complete
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        //level failed
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 5:
                    if(SorterBot.transform.position.x > 4.8f)
                    {
                        //level complete
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        //level failed
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 6:
                    //int temp = botComputer.emulattor.registers[3]*0.8f + -4.2f;
                    Debug.Log(targetWinVal);
                    Debug.Log(SorterBot.transform.position.x);
                    if(SorterBot.transform.position.x < targetWinVal + 0.4f && SorterBot.transform.position.x > targetWinVal -0.4f)
                    {
                        //level complete
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        //level failed
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 7:
                    SnapSpotScript spot = SortSelf.snapSpots[1].GetComponent<SnapSpotScript>();
                    if (spot.HasObject)
                    {
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 8:
                    SnapSpotScript spot1 = SortSelf.snapSpots[(int)targetWinVal].GetComponent<SnapSpotScript>();
                    Debug.Log(targetWinVal);
                    Debug.Log(spot1.HasObject);
                    if (spot1.HasObject)
                    {
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 9:
                    /*
                    bool test9 = true;
                    for(int i = 1; i < 6; i++)
                    {
                        SnapSpotScript spot2 = SortSelf.snapSpots[i].GetComponent<SnapSpotScript>();
                        Debug.Log(spot2.HasObject);
                        if (spot2.HasObject)
                        {

                        }
                        else
                        {
                            test9 = false;
                        }
                    }
                    */
                    if (SortSelf.LetterBoxes[SortSelf.NumberOfLetterBoxes - 1].transform.position.z < -1.5f)
                    {
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                case 10:
                    LetterBlock lb1 = SortSelf.LetterBoxes[0].GetComponent<LetterBlock>();
                    LetterBlock lb2 = SortSelf.LetterBoxes[1].GetComponent<LetterBlock>();
                    if(SortSelf.LetterBoxes[0].transform.position.x < SortSelf.LetterBoxes[1].transform.position.x && !(lb1.letter > lb2.letter))
                    {
                        WinPane.SetActive(true);
                    }
                    else if (SortSelf.LetterBoxes[0].transform.position.x > SortSelf.LetterBoxes[1].transform.position.x && !(lb1.letter < lb2.letter))
                    {
                        WinPane.SetActive(true);
                    }
                    else
                    {
                        LosePane.SetActive(true);
                    }
                    botComputer.state = -2;
                    break;
                default:
                    break;
            }
            EndScoreTxt.text = "Clock Cycles: " + botComputer.ClockCycles + "\nProgram Size: " + botComputer.emulator.ProgramSize + " Bytes";
        }
    }


    public void CompileProgram()
    {
        Debug.Log("Compile");
        Debug.Log(inputProgramTxt.text);
        string programText = inputProgramTxt.text.Trim((char)8203);
        StreamWriter sw = new StreamWriter(PATH + "program" + ".txt");
        sw.Write(programText);
        sw.Close();
        
        compiler.Compile("program");

        string line;
        try
        {
            ConsoleTxt.text = "";
            StreamReader sr = new StreamReader(PATH + "programERROR.txt");
            line = sr.ReadLine();

            while (line != null)
            {
                ConsoleTxt.text += line;
                ConsoleTxt.text += "\n";
                line = sr.ReadLine();
            }
            sr.Close();
        }
        catch
        {
            ConsoleTxt.text = "No errors found";
        }
        ResetSim();
        
    }


    public void SetLineNumbers(int number)
    {
        var sb = new System.Text.StringBuilder();
        for(int i = 0; i<number; i++)
        {
            sb.Append(string.Format("{0,4}:\n", i));
        }
        LineNumbers.text = sb.ToString();
    }

    public void ResetSim()
    {
        SorterBot.transform.position = new Vector3(-4.63f, -1.67f, -2.52f);
        botComputer.Reset();
        SortSelf.ResetBlocks();
        SortSelf.CB.ConveyorOn = true;


        switch(LevelNumber)
        {
            case 2:
                botComputer.emulator.registers[3] = (short)Random.Range(5, 32);
                break;
            case 4:
                botComputer.emulator.registers[2] = (short)Random.Range(5, 32);
                botComputer.emulator.registers[3] = (short)Random.Range(5, 32);
                targetWinVal = botComputer.emulator.registers[2] + botComputer.emulator.registers[3];
                break;
            case 6:
                botComputer.emulator.registers[3] = (short)Random.Range(2, 8);
                targetWinVal = (float)botComputer.emulator.registers[3] * 0.8f + -4.629f;
                break;
            case 8:
                botComputer.emulator.registers[2] = (short)Random.Range(2, 8);
                targetWinVal = botComputer.emulator.registers[2];
                break;
            default:
                break;
        }
    }

}
