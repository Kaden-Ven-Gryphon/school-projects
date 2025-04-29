using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SorterBotControler : MonoBehaviour
{

    private Rigidbody rigidbody;
    private float direction = 0f;
    private float armTargetRot = 0f;
    private GameObject heldBlock;
    private SorterBotGraber graber;
    private GameObject graberArm;
    private float smooth = 5f;
    private bool puttingDownBlock = false;
    public float setVel = 0f;

    public bool isHoldingBlock;
    // Start is called before the first frame update
    void Start()
    {
        rigidbody = GetComponent<Rigidbody>();
        graber = GetComponentInChildren<SorterBotGraber>();
        graberArm = this.gameObject.transform.GetChild(2).gameObject;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if(isHoldingBlock)
        {
            heldBlock.transform.position = graber.transform.position;
            
            heldBlock.transform.rotation = graber.transform.rotation * Quaternion.Euler(-90,0,0);
        }

        Quaternion target = Quaternion.Euler(armTargetRot, 0, 0);
        graberArm.transform.rotation = Quaternion.Slerp(graberArm.transform.rotation, target, Time.deltaTime * smooth);

        if(target == graberArm.transform.rotation && puttingDownBlock)
        {
            if (heldBlock != null)
            {
                Snapable snapable = heldBlock.GetComponent<Snapable>();
                snapable.isBeingHeld = false;
                heldBlock = null;
                isHoldingBlock = false;
                puttingDownBlock = false;

            }
            else puttingDownBlock = false;

        }

        setMovement(setVel);


    }

    

    public void setMovement(float dir)
    {
        direction = dir;
        rigidbody.velocity = new Vector3(direction, 0, 0);
    }

    public void pickUpBlock()
    {
        if (isHoldingBlock) return;
        heldBlock = graber.overlabedCube;
        if (heldBlock != null)
        {
        	Snapable snapable = heldBlock.GetComponent<Snapable>();
        	if(snapable.isBeingHeld == false)
        	{
        		isHoldingBlock = true;
            	armTargetRot = -90f;
            	snapable.isBeingHeld = true;
        	}
            
        }
    }

    public void putDownBlock()
    {
        puttingDownBlock = true;
        armTargetRot = 0f;
    }


    public short GetBlockVal()
    {
        if(isHoldingBlock)
        {
            LetterBlock lb = heldBlock.GetComponent<LetterBlock>();
            return (short)lb.letter;
        }
        else
        {
            return -1;
        }
    }
}
