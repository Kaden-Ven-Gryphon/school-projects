using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ButtonHelp : MonoBehaviour
{
    public GameObject helpPane;
    private AudioSource source;
    void Start()
    {
        source = GetComponent<AudioSource>();
    }

    public void OnButtonPress()
    {
        source.Play();
        helpPane.SetActive(true);
    }
    public void MoreHelpButtonPress()
    {
        source.Play();
        SceneManager.LoadScene(1);
    }
}
