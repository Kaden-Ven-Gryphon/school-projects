using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FixedPosFollow : MonoBehaviour
{
    public float xOffSet = 0;
    public float yOffSet = 0;
    public GameObject target;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void LateUpdate()
    {
        transform.position = new Vector3(target.transform.position.x + xOffSet, target.transform.position.y + yOffSet, target.transform.position.z);
    }
}
