using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ManuelControl : MonoBehaviour
{
    
    public SorterBotControler bot;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetAxis("Horizontal") != 0)
        {
            bot.setMovement(Input.GetAxis("Horizontal"));
        }
        if(Input.GetButtonDown("Jump"))
        {
            if(bot.isHoldingBlock) bot.putDownBlock();
            
            else bot.pickUpBlock();
        }
    }
}
