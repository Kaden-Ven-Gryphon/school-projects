using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClosePaneButton : MonoBehaviour
{
    public GameObject pane;
    private AudioSource source;
    void Start()
    {
        source = GetComponent<AudioSource>();
    }

    public void OnButtonPress()
    {
        source.Play();
        pane.SetActive(false);
    }
}
