using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TurretControler : MonoBehaviour
{
    public Rigidbody2D projectile;

    public bool trackMouse = true;

    public int loadedshots = 2;
    public float reloadTime = 5;
    public float B16Impulse = 10;
    public float rotationSpeed = 1;

    public Vector2 manuelTarget;


    private AudioSource audioSource;
    public AudioClip reloadSound;

    GameObject parentShip;
    Rigidbody2D parent_RD2D;

    // Start is called before the first frame update
    void Start()
    {
        audioSource = GetComponent<AudioSource>();
        manuelTarget = new Vector2(0, 0);
        parentShip = transform.parent.gameObject;
        parent_RD2D = parentShip.GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        if (trackMouse)
        {
            Vector3 pz = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            Vector2 target = pz;
            lookAt(target);
        }
        else lookAt(manuelTarget);
    }


    public void fire()
    {
        if(loadedshots > 0)
        {
            audioSource.Play();
            Vector3 pz = transform.up;
            //Debug.Log(pz);
            pz.z = 0;
            Rigidbody2D bullet = Instantiate(projectile, transform.position, transform.rotation);
            bullet.velocity = parent_RD2D.velocity;
            //pz = pz - transform.position;
            pz.Normalize();

            bullet.AddForce(new Vector2(pz.x, pz.y) * B16Impulse, ForceMode2D.Impulse);

            loadedshots--;
            Invoke("reload", reloadTime);
        }
        
    }


    void lookAt(Vector2 target)
    {
        Vector2 vectorToTarget = target - (Vector2)transform.position;
        float angle = Mathf.Atan2(vectorToTarget.y, vectorToTarget.x) * Mathf.Rad2Deg - 90;
        Quaternion q = Quaternion.AngleAxis(angle, Vector3.forward);
        transform.rotation = Quaternion.Slerp(transform.rotation, q, Time.deltaTime * rotationSpeed);
    }

    void reload()
    {
        audioSource.PlayOneShot(reloadSound, 0.5f);
        loadedshots++;
    }

    

}
