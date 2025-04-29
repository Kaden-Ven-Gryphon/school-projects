using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TutorialMenuSwitcher : MonoBehaviour
{
    public int numberOfPanels = 0;
    public GameObject[] panes = new GameObject[10];
    private AudioSource source;   
    public int currentPane = 0;
    // Start is called before the first frame update
    void Start()
    {
        source = GetComponent<AudioSource>();
        if(numberOfPanels>0)
        {
            panes[currentPane].SetActive(true);
        }
    }

    public void NextButton()
    {
        source.Play();
        if(currentPane < numberOfPanels-1)
        {
            panes[currentPane].SetActive(false);
            currentPane++;
            panes[currentPane].SetActive(true);
        }
        else
        {
            Scene scene = SceneManager.GetActiveScene();
            SceneManager.LoadScene(scene.buildIndex + 1);
        }
    }

    public void BackButton()
    {
        source.Play();
        if(currentPane > 0)
        {
            panes[currentPane].SetActive(false);
            currentPane--;
            panes[currentPane].SetActive(true);
        }
    }
}
