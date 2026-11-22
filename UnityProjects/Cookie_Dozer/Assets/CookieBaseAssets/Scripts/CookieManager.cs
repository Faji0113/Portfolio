using System.Collections;
using UnityEngine;

public class CookieManager : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    const int DefaultCookieAmount = 0;
    const int recovery_seconds = 10;
    private bool is_recovering = false;
    private Coroutine Recovercoroutine;

    int CookieAmount = DefaultCookieAmount;

    int timer;//残り秒数
    
    public int GetCookieAmount() {  return CookieAmount; }
    public void ConsumeCookie() 
    {
        if (CookieAmount > 0) CookieAmount--;
    }

    public void AddCookie(int amount) 
    {
        CookieAmount += amount;
    }

    void OnGUI()//スコアやボタンを表示するとき使う
    {
        GUI.color = Color.black;//文字の色
        string label = "Cookie : " + CookieAmount;//表示する文字
        if (timer > 0 && is_recovering) label = label + "(Recovering time: " + timer + " s)";
        GUI.matrix = Matrix4x4.Scale(Vector3.one * 2);
        GUI.Label(new Rect(10, 10, 200, 30), label);//長方形領域Rect(x, y, width, height)
    }

    private void Update()
    {
        if (timer <= 0 && CookieAmount == DefaultCookieAmount && !is_recovering) 
        {
            Recovercoroutine=StartCoroutine(RecoverCookie());
        }

        if (CookieAmount > DefaultCookieAmount && is_recovering) 
        {
            StopCoroutine(Recovercoroutine);
            Recovercoroutine= null;
            is_recovering= false;
            timer = 0;
        }
    }

    IEnumerator RecoverCookie() 
    {
        is_recovering = true;
        timer = recovery_seconds;

        while (timer > 0) 
        { 
            yield return new WaitForSeconds(1.0f);
            --timer;
        }
        CookieAmount++;
        is_recovering = false;
    }

}
