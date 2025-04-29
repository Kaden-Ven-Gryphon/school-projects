using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControler : MonoBehaviour
{
    public float scrollScale = 0.1f;
    public Camera mainCam;


    GameObject voidShip;


    // Start is called before the first frame update
    void Start()
    {
        voidShip = GameObject.Find("VoidShip");
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 pos = voidShip.transform.position;
        pos.z = -10;
        transform.position = pos;

        float scroldelta = Input.mouseScrollDelta.y * -scrollScale;
        if(scroldelta != 0)
        {
            mainCam.orthographicSize += scroldelta;
            if (mainCam.orthographicSize > 50) mainCam.orthographicSize = 50;
            if (mainCam.orthographicSize < 10) mainCam.orthographicSize = 10;
        }
    }
}
