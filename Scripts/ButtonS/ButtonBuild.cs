using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonBuild : MonoBehaviour
{
    public GMscript gm;
    private AudioSource source;
    void Start()
    {
        source = GetComponent<AudioSource>();
    }


    public void OnButtonPress()
    {
        source.Play();
        gm.CompileProgram();
    }
}
