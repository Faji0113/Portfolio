using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneController : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //Title sceneのText Mesh ProのButtonにアタッチしOn click()で呼び出し
    public void GotoMain() 
    {
        SceneManager.LoadScene("Main");
    }

    //Main sceneでクリアウィンドウのText Mesh ProのButtonにアタッチしOn click()で呼び出し
    public void GotoTitle() 
    {
        SceneManager.LoadScene("Title");
    }
}
