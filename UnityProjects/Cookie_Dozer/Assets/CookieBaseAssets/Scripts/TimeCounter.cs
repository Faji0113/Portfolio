using System.Collections;
using TMPro;
using Unity.Collections;
using UnityEngine;
using UnityEngine.UI;

public class TimeCounter : MonoBehaviour
{
    Coroutine c_timer;
    public CookieManager cm;
    float time = 0.0f;
    public GameObject menuPanel;
    public Shooter shooter;
    bool is_menu = false;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        c_timer = StartCoroutine(timer());
        menuPanel.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (cm.GetCookieAmount() >= 100 && !is_menu) 
        {
            StopCoroutine(c_timer);
            //shotの受付を停止
            shooter.Stopshot();
            //メニューパネルにスコアを表示するテキストを追加
            GameObject score_text = new GameObject("Score_text");
            score_text.transform.SetParent(menuPanel.transform, false);
            TextMeshProUGUI ST = score_text.AddComponent<TextMeshProUGUI>();
            RectTransform rect = score_text.GetComponent<RectTransform>();
            rect.sizeDelta = new Vector2(400, 50);
            rect.localPosition = new Vector3(0, 4, 0);

            ST.text = "Clear time: " + time.ToString("F1") + "s";

            ST.fontSize = 36;

            ST.color = Color.black;

            ST.fontStyle=FontStyles.Bold;

            ST.alignment = TextAlignmentOptions.Center;
            //メニュー画面を表示する
            menuPanel.SetActive(true);
            is_menu = true;
        }
    }

    IEnumerator timer() 
    {
        while (cm.GetCookieAmount() < 100)
        {
            yield return new WaitForSeconds(0.1f);
            time += 0.1f;
        }
    }

    private void OnGUI()
    {
        GUI.color = Color.black;
        string label = time.ToString("F1") + "s";
        GUI.matrix = Matrix4x4.Scale(Vector3.one * 2);
        GUI.Label(new Rect(300, 10,200, 30), label);
    }
}
