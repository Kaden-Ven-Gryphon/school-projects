using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ConveyorBelt : MonoBehaviour
{
    public bool ConveyorOn = true;

    void OnCollisionStay(Collision collision)
    {
        if (collision.gameObject.tag == "Snapable")
        {
            if(ConveyorOn) collision.gameObject.GetComponent<Rigidbody>().velocity = new Vector3(0f, 0f, -0.5f);
            else collision.gameObject.GetComponent<Rigidbody>().velocity = new Vector3(0f, 0f, 0f);
        }
    }
}
