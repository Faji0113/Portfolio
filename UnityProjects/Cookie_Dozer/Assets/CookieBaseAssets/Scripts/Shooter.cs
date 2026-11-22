using System.Collections;
using UnityEngine;
using UnityEngine.InputSystem;

public class Shooter : MonoBehaviour
{
    public GameObject[] CookiePrehabs;
    public CookieManager CookieManager;
    public WarningController wc;
    public Transform CookieParentTransform;//クッキーを発射する場所
    public float ShotForce;//力
    public float ShotTorque;//回転
    public float BaseWidth;

    const int max_power = 5;//連続投入できる最大
    const int recovery_seconds = 3;

    InputAction _fireAction;
    int shotpower = max_power;
    bool continue_shot = true;

    AudioSource shotsound;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        _fireAction = InputSystem.actions.FindAction("Attack");//入力の準備
        shotsound = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (_fireAction.WasPressedThisFrame() && continue_shot) Shot();
    }

    //CookiePrehabをランダムに選ぶ
    GameObject SampleCookie() 
    {
        int index = Random.Range(0, CookiePrehabs.Length);
        return CookiePrehabs[index];
    }

    Vector3 GetInstantinatePosition() 
    {
        //画面サイズとInputの割合からクッキー生成のポジションを計算
        float x = BaseWidth *
            (Mouse.current.position.ReadValue().x / Screen.width) - //Mouse.current.position.ReadValue()...マウスがクリックした位置を示すVector3, Screen.width...スクリーンの広さ
            (BaseWidth / 2);
        return transform.position + new Vector3(x, 0, 0);
    }
    public void Shot() 
    {
        if (CookieManager.GetCookieAmount() <= 0) { wc.shootwarn(1); return; }
        if (shotpower <= 0) { wc.shootwarn(0); return; }
        
        GameObject cookie=Instantiate(
                SampleCookie(), 
                GetInstantinatePosition(),
                Quaternion.identity//生成物...CookiePrehab, 生成位置...原点,Quaternion...回転, identity...なし
            );

        cookie.transform.parent = CookieParentTransform;

        Rigidbody Cookierigidbody = cookie.GetComponent<Rigidbody>();//生成したcookieオブジェクトにrigidbodyのcomponentを追加しそれを編集
        Cookierigidbody.AddForce(transform.forward * ShotForce);//z軸正方向にShotForce分の力を加える
        Cookierigidbody.AddTorque(new Vector3(0, ShotTorque, 0));//y軸中心の回転方向

        CookieManager.ConsumeCookie();

        ConsumePower();

        shotsound.Play();
    }

    private void OnGUI()
    {
        GUI.color = Color.black;
        string label = "Shootstock:";
        for (int i = 0; i < shotpower; i++) label = label + "o";
        GUI.matrix = Matrix4x4.Scale(Vector3.one * 2);//matrix4*4は三次元座標系で拡大縮小、回転、平行移動を表現するために4次元
        GUI.Label(new Rect(10, 25, 120, 30), label);
    }

    private void ConsumePower() 
    {
        shotpower--;
        StartCoroutine(RecoverPower());
    }

    IEnumerator RecoverPower() 
    {
        yield return new WaitForSeconds(recovery_seconds);
        shotpower++;
    }

    public int GetShootstock() { return shotpower; }

    public void Stopshot() 
    {
        continue_shot = false;
    }
}
