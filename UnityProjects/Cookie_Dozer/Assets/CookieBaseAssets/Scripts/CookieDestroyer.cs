using UnityEngine;

public class CookieDestroyer : MonoBehaviour
{
    [SerializeField]
    public CookieManager cookieManager;
    public int Reward;

    public GameObject EffectPrehab;
    public Vector3 EffectRotation;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    private void OnTriggerEnter(Collider other)
    {
        if (other.transform.parent.tag == "Cookie") 
        {
            cookieManager.AddCookie(Reward);
            Destroy(other.transform.parent.gameObject);//cookie Prehabはもとのempty objectがモデルとcolliderを子に持っている状態。つまりここでいうotherはcolliderのみを指すため、親ごと消去する必要がある。
            if (EffectPrehab != null) 
            {
                Instantiate(EffectPrehab, other.transform.position, Quaternion.Euler(EffectRotation));
            }
        }
    }
}
