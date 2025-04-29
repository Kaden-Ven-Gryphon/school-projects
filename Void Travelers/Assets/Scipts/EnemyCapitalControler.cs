using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyCapitalControler : MonoBehaviour
{
    GameObject voidShip;
    private Rigidbody2D ship_RB2D;


    public float maxSpeed = 2;
    public float rotationSpeed = 0.1f;

    public int hullPoints = 100;
    public HealthBar hullBar;
    public int shieldPoints = 100;
    public HealthBar shieldBar;
    public GameObject sheild;
    bool sheildActve = false;

    public float inAproachRad = 30;
    public float outAproachRad = 50;
    public TurretControler turret1;


    private AudioSource audioSource;
    public AudioClip sheildHitSound;
    public AudioClip hullHitSound;
    public AudioClip engineSound;




    // Start is called before the first frame update
    void Start()
    {
        voidShip = GameObject.Find("VoidShip");
        ship_RB2D = GetComponent<Rigidbody2D>();

        audioSource = GetComponent<AudioSource>();

        shieldBar.SetHealth(shieldPoints);
        hullBar.SetHealth(hullPoints);
        chargeShield();
    }

    // Update is called once per frame
    void Update()
    {
        if(hullPoints<=0)
        {
            GameManager.score += 1;
            Destroy(this.gameObject);
        }
        turret1.manuelTarget = (Vector2)voidShip.transform.position;
        turret1.fire();
    }
    void FixedUpdate()
    {
        Vector2 target = new Vector2(voidShip.transform.position.x, voidShip.transform.position.y);
        if (new Vector2(voidShip.transform.position.x- transform.position.x, voidShip.transform.position.y - transform.position.y).magnitude< inAproachRad)
        {
            Vector2 away = (Vector2)transform.position - target;
            away.Normalize();
            away *= inAproachRad +((outAproachRad-inAproachRad)/2);
            seek(away);
            lookAtDirection((Vector2)ship_RB2D.velocity);
        }
        else if(new Vector2(voidShip.transform.position.x - transform.position.x, voidShip.transform.position.y - transform.position.y).magnitude> outAproachRad)
        {
            seek(target);
            lookAtDirection((Vector2)ship_RB2D.velocity);
        }
        else
        {
            lookAt(target);
            Vector2 slow = new Vector2(ship_RB2D.velocity.x, ship_RB2D.velocity.y);
            slow *= 0.7f;
            ship_RB2D.velocity = slow;
        }

        

        
    }
    void LateUpdate()
    {
        if (sheildActve)
        {
            sheild.SetActive(true);
        }
        else sheild.SetActive(false);
    }



    void OnCollisionEnter2D(Collision2D col)
    {
        
        if (col.gameObject.tag == "Player_Projectile")
        {
            
            ProjectileStats ps = col.gameObject.GetComponent<ProjectileStats>();
            takeDamage(ps.physicalDamage, ps.energyDamage);
            

        }
    }

    void takeDamage(float physical, float energy)
    {
        if(shieldPoints > 0)
        {
            sheildActve = true;
            Invoke("deactivateShield", 1);
            audioSource.PlayOneShot(sheildHitSound, 0.6f);
            shieldPoints -= (int)energy;
            shieldBar.SetHealth(shieldPoints);

        }
        else
        {
            hullPoints -= (int)physical;
            audioSource.PlayOneShot(hullHitSound, 0.6f);
            hullBar.SetHealth(hullPoints);
        }
    }


    void seek(Vector2 target)
    {
        Vector2 results;
        Vector2 pos = transform.position;
        results = target - pos;
        results.Normalize();
        results *= maxSpeed;
        ship_RB2D.velocity = results;
    }


    void lookAt(Vector2 target)
    {
        
        Vector2 vectorToTarget = target - (Vector2)transform.position;
        float angle = Mathf.Atan2(vectorToTarget.y, vectorToTarget.x) * Mathf.Rad2Deg - 90;
        Quaternion q = Quaternion.AngleAxis(angle, Vector3.forward);
        transform.rotation = Quaternion.Slerp(transform.rotation, q, Time.deltaTime * rotationSpeed);
    }
    void lookAtDirection(Vector2 target)
    {
        Vector2 vectorToTarget = target;
        float angle = Mathf.Atan2(vectorToTarget.y, vectorToTarget.x) * Mathf.Rad2Deg - 90;
        Quaternion q = Quaternion.AngleAxis(angle, Vector3.forward);
        transform.rotation = Quaternion.Slerp(transform.rotation, q, Time.deltaTime * rotationSpeed);
    }


    void deactivateShield()
    {
        sheildActve = false;
    }

    void chargeShield()
    {
        Invoke("chargeShield", 5);
        if(shieldPoints < 100)
        {
            shieldPoints += 10;
            if (shieldPoints > 100) shieldPoints = 100;
            shieldBar.SetHealth(shieldPoints);
        }
    }


}
