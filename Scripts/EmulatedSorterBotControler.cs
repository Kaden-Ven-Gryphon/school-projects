using System.Collections;
using System.Collections.Generic;
using System;
using System.Text;
using System.IO;
using UnityEngine;
using TMPro;

public class EmulatedSorterBotControler : MonoBehaviour
{

    private SorterBotControler botControler;
    public ARMThumbEmulator emulator;
    public bool run = false;
    public bool interupt = false;
    bool loadedProgram = false;
    public int IOPins = -1;
    public int state = 0;
    public int ClockCycles = 0;

    public TextMeshProUGUI RegText;

    // Start is called before the first frame update
    void Awake()
    {
        emulator = new ARMThumbEmulator();
        botControler = transform.gameObject.GetComponent<SorterBotControler>();
    }

    void Update()
    {
        updateRegText();

        switch (state)
        {
            //Runing without interupt
            case 1:
                updateRegText();
                Debug.Log("state " + state);
                if (emulator.EndOfProgram)
                {
                    Debug.Log("END");
                    state = -1;
                }
                else
                {
                    Debug.Log("PC= " + emulator.PC);
                    IOPins = emulator.Step();
                    ClockCycles++;
                    Debug.Log("Stepped  IO= " + IOPins);
                    Debug.Log("Comand " + emulator.decodedComand);
                    if (IOPins >= 0)
                    {
                        state = 2;
                        Debug.Log("Switching to 2");
                    }
                    if (!run && state !=2) state = 0;
                }
                break;
            //Got interupt
            case 2:
                Debug.Log("state " + state);
                switch (IOPins)
                {
                    case 1:
                        botControler.setVel = 1.03f;
                        if(emulator.registers[0]<1) botControler.setVel*=-1f;
                        state = 5;
                        StartCoroutine(DelayedStateChange(1, 6));
                        break;
                    case 2:
                        if (botControler.isHoldingBlock)
                        {
                            emulator.registers[1] = 2;
                            state = 3;
                            botControler.putDownBlock();
                            emulator.registers[1] = 2;
                        }
                        else
                        {
                            state = 1;
                            if (!run) state = 0;
                            botControler.pickUpBlock();
                            if (botControler.isHoldingBlock)
                            {
                                emulator.registers[1] = 1;
                            }
                            else
                            {
                                emulator.registers[1] = 0;
                            }
                        }
                        break;
                    case 3:
                        if (botControler.isHoldingBlock)
                        {
                            emulator.registers[1] = botControler.GetBlockVal();
                        }
                        else
                        {
                            emulator.registers[1] = -1;
                        }
                        state = 1;
                        break;
                }
                IOPins = -1;
                updateRegText();
                break;
            //putting downblock
            case 3:
                //Debug.Log("state " + state);
                if (botControler.isHoldingBlock == false)
                {
                    state = 1;
                    if (!run) state = 0;
                }
                break;
            //picking up block
            case 4:
                //Debug.Log("state " + state);
                if (botControler.isHoldingBlock)
                {
                    state = 1;
                    if (!run) state = 0;
                }
                break;
            //moving
            case 5:
                //Debug.Log("state " + state);
                
                break;
            //end move
            case 6:
                //Debug.Log("state " + state);
                botControler.setVel = 0;
                state = 1;
                if (!run) state = 0;
                break;
        }
        
    }

    public void LoadProgram()
    {
        emulator.LoadProgram("programBIN");
        loadedProgram = true;
        ClockCycles = 0;
    }

    public void StepProgram()
    {
        if(!loadedProgram)
        {
            LoadProgram();
            
        }    
        state = 1;
        run = false;
        
    }

    public void Run()
    {
        if(!loadedProgram) LoadProgram();
        
        run = true;
        state = 1;
    }


    IEnumerator DelayedStateChange(float delayTime, int stateChange)
    {
        yield return new WaitForSeconds(delayTime);
        state = stateChange;
    }



    void updateRegText()
    {
        var sb = new System.Text.StringBuilder();
        sb.Append(String.Format("{0,-25} ", emulator.decodedComand) + "CCR(CVNZ): " + Convert.ToString(emulator.CCR, 2).PadLeft(4, '0'));
        //sb.Append(emulator.decodedComand);
        //sb.Append("\t\tCCR (CVZN): ");
        //sb.Append(Convert.ToString(emulator.CCR, 2).PadLeft(4, '0'));
        sb.Append("\n");
        sb.Append(string.Format("{0,4} {1,8} {2,4} {3,8} {4,4} {5,8}\n", "PC: ", emulator.PC.ToString(), "R0: ", emulator.registers[0].ToString(), "R1: ", emulator.registers[1]).ToString());
        sb.Append(string.Format("{0,4} {1,8} {2,4} {3,8} {4,4} {5,8}\n", "R2: ", emulator.registers[2].ToString(), "R3: ", emulator.registers[3].ToString(), "R4: ", emulator.registers[4].ToString()));
        sb.Append(string.Format("{0,4} {1,8} {2,4} {3,8} {4,4} {5,8}\n", "R5: ", emulator.registers[5].ToString(), "R6: ", emulator.registers[6].ToString(), "R7: ", emulator.registers[7].ToString()));
        RegText.text = sb.ToString();
    }


    public void Reset()
    {
        if (botControler == null) return;
        botControler.setVel = 0;
        botControler.putDownBlock();
        botControler.isHoldingBlock = false;
        state = 0;
        run = false;
        loadedProgram = false;
        emulator.clearRegs();
        ClockCycles = 0;
    }

}
