using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectileStats : MonoBehaviour
{
    public float physicalDamage = 5;
    public float energyDamage = 0;

    void Start()
    {
        Invoke("timedOut", 20);
    }

    void OnCollisionEnter2D(Collision2D col)
    {
        Destroy(this.gameObject);
    }

    void timedOut()
    {
        Destroy(this.gameObject);
    }
}
