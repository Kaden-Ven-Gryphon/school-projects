using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GoalButton : MonoBehaviour
{
    public GameObject ObjectivePane;
    private AudioSource source;
    void Start()
    {
        source = GetComponent<AudioSource>();
    }



    public void OnButtonPress()
    {
        source.Play();
        ObjectivePane.SetActive(true);
    }
}
