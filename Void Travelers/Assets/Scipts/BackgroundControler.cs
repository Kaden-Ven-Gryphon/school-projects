using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackgroundControler : MonoBehaviour
{
    public GameObject baseLayer;
    public GameObject layer1;
    public GameObject layer2;
    public GameObject layer3;

    public float baseLayerSpeed = 1;
    public float layer1Speed = 1;
    public float layer2Speed = 1;
    public float layer3Speed = 1;

    GameObject voidShip;
    Rigidbody2D voidShip_RD2D;

    


    // Start is called before the first frame update
    void Start()
    {
        

        voidShip = GameObject.Find("VoidShip");
        voidShip_RD2D = voidShip.GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        baseLayer.transform.position += (Vector3)voidShip_RD2D.velocity * baseLayerSpeed * Time.deltaTime;
        layer1.transform.position += (Vector3)voidShip_RD2D.velocity * layer1Speed * Time.deltaTime;
        layer2.transform.position += (Vector3)voidShip_RD2D.velocity * layer2Speed * Time.deltaTime;
        layer3.transform.position += (Vector3)voidShip_RD2D.velocity * layer3Speed * Time.deltaTime;
    }
}
