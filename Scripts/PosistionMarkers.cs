using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PosistionMarkers : MonoBehaviour
{

      public Color32 normalColor = new Color32(255,255,255,255);
      public Color32 highlightColor = new Color32(100,100,100,255);
      private Renderer _renderer;
      
      private void Awake ()
      {
            _renderer = GetComponent<Renderer>();
      }
      
    private void OnTriggerEnter(Collider other)
    {
      if(other.gameObject.tag == "PositionMarker")
      {
            _renderer.material.color = highlightColor;
            //Debug.Log("marker enter");
      }
    }
    private void OnTriggerExit(Collider other)
    {
        if(other.gameObject.tag == "PositionMarker")
        {
            _renderer.material.color = normalColor;
            //Debug.Log("marker exit");
        }
    }
}
