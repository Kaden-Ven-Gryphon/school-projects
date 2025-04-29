using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InfiniteTiles : MonoBehaviour
{
    public GameObject tile;
    public float xOffset = 340;
    public float yOffset = 340;

    private bool yFlip = false;
    private bool xFilp = false;

    GameObject voidShip;
    Rigidbody2D voidShip_RD2D;

    GameObject[] tileSet = new GameObject[4];
    // Start is called before the first frame update
    void Start()
    {
        voidShip = GameObject.Find("VoidShip");
        voidShip_RD2D = voidShip.GetComponent<Rigidbody2D>();

        Vector3 pos = new Vector3(0, 0, 0);
        tileSet[0] = Instantiate(tile, pos, transform.rotation);
        tileSet[0].transform.parent = gameObject.transform;
        pos.y += yOffset;
        tileSet[1] = Instantiate(tile, pos, transform.rotation);
        tileSet[1].transform.parent = gameObject.transform;
        pos.y = 0;
        pos.x -= xOffset;
        tileSet[2] = Instantiate(tile, pos, transform.rotation);
        tileSet[2].transform.parent = gameObject.transform;
        pos.y += yOffset;
        tileSet[3] = Instantiate(tile, pos, transform.rotation);
        tileSet[3].transform.parent = gameObject.transform;
    }

    // Update is called once per frame
    void Update()
    {
        if(!yFlip)
        {
            if(voidShip.transform.position.y < tileSet[0].transform.position.y)
            {
                yFlip = true;
                Vector3 pos = tileSet[0].transform.position;
                pos.y -= yOffset;
                tileSet[1].transform.position = pos;
                pos = tileSet[2].transform.position;
                pos.y -= yOffset;
                tileSet[3].transform.position = pos;
            }
            else if(voidShip.transform.position.y > tileSet[1].transform.position.y)
            {
                yFlip = true;
                Vector3 pos = tileSet[1].transform.position;
                pos.y += yOffset;
                tileSet[0].transform.position = pos;
                pos = tileSet[3].transform.position;
                pos.y += yOffset;
                tileSet[2].transform.position = pos;
            }
        }
        else
        {
            if (voidShip.transform.position.y < tileSet[1].transform.position.y)
            {
                yFlip = false;
                Vector3 pos = tileSet[1].transform.position;
                pos.y -= yOffset;
                tileSet[0].transform.position = pos;
                pos = tileSet[3].transform.position;
                pos.y -= yOffset;
                tileSet[2].transform.position = pos;
            }
            else if (voidShip.transform.position.y > tileSet[0].transform.position.y)
            {
                yFlip = false;
                Vector3 pos = tileSet[0].transform.position;
                pos.y += yOffset;
                tileSet[1].transform.position = pos;
                pos = tileSet[2].transform.position;
                pos.y += yOffset;
                tileSet[3].transform.position = pos;
            }
        }
        if(!xFilp)
        {
            if (voidShip.transform.position.x < tileSet[2].transform.position.x)
            {
                xFilp = true;
                Vector3 pos = tileSet[2].transform.position;
                pos.x -= xOffset;
                tileSet[0].transform.position = pos;
                pos = tileSet[3].transform.position;
                pos.x -= xOffset;
                tileSet[1].transform.position = pos;
            }
            else if (voidShip.transform.position.x > tileSet[0].transform.position.x)
            {
                xFilp = true;
                Vector3 pos = tileSet[0].transform.position;
                pos.x += xOffset;
                tileSet[2].transform.position = pos;
                pos = tileSet[1].transform.position;
                pos.x += xOffset;
                tileSet[3].transform.position = pos;
            }
        }
        else
        {
            if (voidShip.transform.position.x < tileSet[0].transform.position.x)
            {
                xFilp = false;
                Vector3 pos = tileSet[0].transform.position;
                pos.x -= xOffset;
                tileSet[2].transform.position = pos;
                pos = tileSet[1].transform.position;
                pos.x -= xOffset;
                tileSet[3].transform.position = pos;
            }
            else if (voidShip.transform.position.x > tileSet[2].transform.position.x)
            {
                xFilp = false;
                Vector3 pos = tileSet[2].transform.position;
                pos.x += xOffset;
                tileSet[0].transform.position = pos;
                pos = tileSet[3].transform.position;
                pos.x += xOffset;
                tileSet[1].transform.position = pos;
            }
        }
    }
}
