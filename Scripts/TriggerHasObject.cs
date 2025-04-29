using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TriggerHasObject : MonoBehaviour
{
    public bool Ocupied = false;
    void OnTriggerStay(Collider other)
    {
        if (other.gameObject.tag == "Snapable")
            Ocupied = true;
        other = null;
    }
    private void OnTriggerExit(Collider other)
    {
        Ocupied = false;
    }
}
