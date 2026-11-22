using System.Collections;
using UnityEngine;

public class WarningController : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    public Shooter shooter;
    int shootstock;
    string label;
    bool is_warning = false;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        shootstock = shooter.GetShootstock();
    }

    private void OnGUI()
    {
        GUI.color = Color.red;
        GUI.matrix=Matrix4x4.Scale(Vector3.one*3);
        GUI.Label(new Rect(66, 40, 150, 50), label);
    }

    public void shootwarn(int param) 
    {
        if(!is_warning)StartCoroutine(Warn(param));
    }

    IEnumerator Warn(int param) 
    {
        is_warning = true;
        string l="";
        if (param == 0) { l = "Shootstock"; }
        else if (param == 1) { l = "Cookie"; }
        label = (l + " is run out!\nJust a moment!");
        yield return new WaitForSeconds(3.0f);
        label = null;
        is_warning = false;
    }

    public void cookiewarn(int param) 
    {
        if (!is_warning) StartCoroutine(Warn(param));
    }
}
