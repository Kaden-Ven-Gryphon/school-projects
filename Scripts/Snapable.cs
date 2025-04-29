using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Snapable : MonoBehaviour
{
      public Color32 normalColor = new Color32(255,255,255,255);
      public Color32 highlightColor = new Color32(100,100,100,255);
      public Renderer renderer;

	public bool isBeingHeld = false;
	void Start()
	{
	      renderer = GetComponent<Renderer>();
	}
}
