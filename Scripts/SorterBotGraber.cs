using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SorterBotGraber : MonoBehaviour
{
    private SorterBotControler bot;
    public GameObject overlabedCube;
    // Start is called before the first frame update
    void Start()
    {
        bot = transform.parent.parent.gameObject.GetComponent<SorterBotControler>();
    }

 
    private void OnTriggerEnter(Collider other)
    {
        overlabedCube = other.gameObject;
        if(other.gameObject.tag == "Snapable")
        {
            Snapable snapable = other.gameObject.GetComponent<Snapable>();
            snapable.renderer.material.color = snapable.highlightColor;
        }
    }
    private void OnTriggerExit(Collider other)
    {
        overlabedCube = null;
        if(other.gameObject.tag == "Snapable")
        {
            Snapable snapable = other.gameObject.GetComponent<Snapable>();
            snapable.renderer.material.color = snapable.normalColor;
        }
    }
}
