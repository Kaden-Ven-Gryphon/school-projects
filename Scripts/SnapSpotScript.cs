using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SnapSpotScript : MonoBehaviour
{
    

    public float xoff = 0f;
	public float yoff = 0f;
	public float zoff = 0f;
	
	public GameObject objectToSnap;
    public bool HasObject = false;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        



        if (objectToSnap != null)
        {
            Snapable snapable = objectToSnap.GetComponent<Snapable>();
            if(snapable.isBeingHeld == false)
            {
                  Vector3 pos = transform.position;
                  pos.x += xoff;
                  pos.y += yoff;
                  pos.z += zoff;
                  objectToSnap.transform.position = pos;
                
               
                  objectToSnap = null;
            }
            
        }
    }
    
    
    void OnTriggerEnter(Collider other)
    {
    	//Debug.Log("Trigger Enter");
    	if(other.gameObject.tag == "Snapable")
    	{
            objectToSnap = other.gameObject;
            objectToSnap.GetComponent<Rigidbody>().useGravity = false;
            objectToSnap.GetComponent<Rigidbody>().isKinematic = true;
            objectToSnap.GetComponent<BoxCollider>().isTrigger = true;
            HasObject = true;
        }
    }
    private void OnTriggerExit(Collider other)
    {
      objectToSnap = null;
        if (other.gameObject.tag == "Snapable")
        {
            //objectToSnap = other.gameObject;
            //objectToSnap.GetComponent<Rigidbody>().useGravity = true;

            //objectToSnap = null;
            HasObject = false;
        }
    }
}
