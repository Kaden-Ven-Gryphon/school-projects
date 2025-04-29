using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SortingSelf : MonoBehaviour
{

    public GameObject SnapSpotPrefab;
    public GameObject LetterBoxPrefab;
    public ConveyorBelt CB;
    public TriggerHasObject ConveyorSwitch;
    public GameObject[] snapSpots;
    public GameObject[] LetterBoxes;
    public int NumberOfLetterBoxes = 4;
    // Start is called before the first frame update
    void Start()
    {
        snapSpots = new GameObject[11];
        LetterBoxes = new GameObject[NumberOfLetterBoxes];
        Vector3 startpos = new Vector3(-4.2f, 0.31f, 0f);
        startpos += gameObject.transform.position;
        float offset = 0.8f;
        for(int i = 0; i < 11; i++)
        {
            snapSpots[i] = Instantiate(SnapSpotPrefab, startpos, Quaternion.identity, gameObject.transform);
            startpos.x += offset;

        }
        ResetBlocks();
    }

    // Update is called once per frame
    void Update()
    {
        CB.ConveyorOn = !ConveyorSwitch.Ocupied;
    }

    public void ResetBlocks()
    {
        for (int i = 0; i < NumberOfLetterBoxes; i++)
        {
            Destroy(LetterBoxes[i]);
        }
        Vector3 startpos = new Vector3(-4.66f, -0.4f, -1f);
        float offset = 2f;
        for (int i = 0; i < NumberOfLetterBoxes; i++)
        {
            LetterBoxes[i] = Instantiate(LetterBoxPrefab, startpos, Quaternion.identity);
            startpos.z += offset;
        }
        ConveyorSwitch.Ocupied = false;
    }
}
