using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public Text scoreTxt;
    public Text endScoreTxt;
    public GameObject gameoverScreen;

    public static int score = 0;
    public static bool gameOver = false;

    int level = 1;

    public GameObject enemyPreFab;

    public GameObject voidShip;

    // Start is called before the first frame update
    void Start()
    {
        Vector3 pos = findPos();
        Instantiate(enemyPreFab, pos, transform.rotation);
    }

    // Update is called once per frame
    void Update()
    {
        if(score >=(level*(level+1)/2))
        {
            level++;
            for(int i = 0; i < level; i++)
            {
                Vector3 pos = findPos();
                Instantiate(enemyPreFab, pos, transform.rotation);
            }
        }
        scoreTxt.text = "Score: " + score.ToString();
        if(gameOver)
        {
            gameoverScreen.SetActive(true);
            endScoreTxt.text = "Score: " + score.ToString();
        }
    }

    Vector3 findPos()
    {
        int dir = Random.Range(-1, 1);
        if (dir == 0) dir = 1;

        Vector3 ret = voidShip.transform.position;

        ret.x += (Random.Range(50,100) * dir);
        ret.y += (Random.Range(50, 100) * dir);

        return ret;
    }


    public void retartButton()
    {
        GameManager.score = 0;
        level = 1;
        GameManager.gameOver = false;
        gameoverScreen.SetActive(false);
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex-1);
    }

}
