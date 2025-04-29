using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VoidShipController : MonoBehaviour
{
    public Rigidbody2D projectile;

    public AudioSource audioSource;
    public AudioSource engineSounds;
    public AudioClip sheildHitSound;
    public AudioClip hullHitSound;
    public AudioClip engineSound;

    public TurretControler turret1;
    public TurretControler turret2;
    public GameObject engineFire;
    bool engineOn = false;
    public GameObject sheild;
    bool sheildActve = false;

    public int hullPoints = 100;
    public HealthBar hullBar;
    public int shieldPoints = 100;
    public HealthBar shieldBar;

    private Rigidbody2D ship_RB2D;



    public float mainEngine_Force = 10;
    public float yawEngine_Torque = 2;



    private bool rotationLock = true;
    private bool turning = false;
    public float angularDrag = 3;
    public float drag = 2;


    public float speedCap = 5;

    // Start is called before the first frame update
    void Start()
    {
        ship_RB2D = GetComponent<Rigidbody2D>();
        shieldBar.SetHealth(shieldPoints);
        hullBar.SetHealth(hullPoints);
        
        
        chargeShield();
    }

    // Update is called once per frame
    void Update()
    {
        if(hullPoints<=0)
        {
            GameManager.gameOver = true;
        }
        //Debug.Log(ship_RB2D.velocity.magnitude);

        float horizontalInput = Input.GetAxis("Horizontal");
        float verticalInput = Input.GetAxis("Vertical");

        if (horizontalInput != 0) turning = true;
        else turning = false;
        if (verticalInput != 0) engineOn = true;
        else engineOn = false;



        if (Input.GetKeyDown(KeyCode.R))
        {
            if (rotationLock)
            {
                rotationLock = false;
            }
            else
            {
                rotationLock = true;
            }
        }


        if (Input.GetKey(KeyCode.LeftShift))
        {
            ship_RB2D.drag = 2;
            ship_RB2D.angularDrag = angularDrag;
            engineOn = true;
        }
        else
        {
            ship_RB2D.drag = 0;
            if (!rotationLock)
            {
                ship_RB2D.angularDrag = 0;
            }
            if (verticalInput == 0) engineOn = false;

        }

        if (Input.GetMouseButtonDown(0))
        {
            if (turret1.loadedshots > 0)
            {
                turret1.fire();
            }
            else if (turret2.loadedshots > 0)
            {
                turret2.fire();
            }

        }

        if(!rotationLock || turning)
        {
            ship_RB2D.angularDrag = 0;
        }
        else 
        {
            ship_RB2D.angularDrag = angularDrag;
        }

        //Debug.Log(horizontalInput);
        ship_RB2D.AddRelativeForce((new Vector2(0, verticalInput) * mainEngine_Force));
        ship_RB2D.AddTorque(-horizontalInput * yawEngine_Torque);



    }


    void LateUpdate()
    {
        if (sheildActve)
        {
            sheild.SetActive(true);
        }
        else sheild.SetActive(false);

        if (ship_RB2D.velocity.magnitude > speedCap)
        {
            ship_RB2D.velocity = ship_RB2D.velocity.normalized * speedCap;
            engineOn = false;
        }
        if(engineOn)
        {
            engineFire.SetActive(true);
            engineSounds.UnPause();
        }
        else
        {
            engineFire.SetActive(false);
            engineSounds.Pause();
        }
    }

    void takeDamage(float physical, float energy)
    {
        if (shieldPoints > 0)
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

    void OnCollisionEnter2D(Collision2D col)
    {

        if (col.gameObject.tag == "Enemy_Projectile")
        {

            ProjectileStats ps = col.gameObject.GetComponent<ProjectileStats>();
            takeDamage(ps.physicalDamage, ps.energyDamage);


        }
    }

    void deactivateShield()
    {
        sheildActve = false;
    }
    void chargeShield()
    {
        Invoke("chargeShield", 5);
        if (shieldPoints < 100)
        {
            shieldPoints += 10;
            if (shieldPoints > 100) shieldPoints = 100;
            shieldBar.SetHealth(shieldPoints);
        }
    }

}
