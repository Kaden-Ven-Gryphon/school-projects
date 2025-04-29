using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class LetterBlock : MonoBehaviour
{
      public char letter = 'A';
     
      
      private TextMeshPro frontLetter;
      private TextMeshPro topLetter;
      
      
      void Start()
      {
            letter = (char)Random.Range(65, 91);
            frontLetter = transform.GetChild(0).GetComponent<TextMeshPro>();
            topLetter = transform.GetChild(1).GetComponent<TextMeshPro>();
            frontLetter.text = letter.ToString();
            topLetter.text = letter.ToString();
      }
}
