using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonRun : MonoBehaviour
{
    public EmulatedSorterBotControler botControler;
    private AudioSource source;
    void Start()
    {
        source = GetComponent<AudioSource>();
    }



    public void OnButtonPress()
    {
        source.Play();
        botControler.Run();
    }
}
