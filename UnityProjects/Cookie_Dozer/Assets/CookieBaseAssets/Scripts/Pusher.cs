using UnityEngine;

public class Pusher : MonoBehaviour
{
    public float Amplitude;//ˆÚ“®—Ê
    public float Speed;//ˆÚ“®‘¬“x

    Vector3 _startPosition;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        _startPosition = transform.localPosition;
    }

    // Update is called once per frame
    void Update()
    {
        float z = Amplitude * Mathf.Sin(Time.time * Speed);
        transform.localPosition = _startPosition + new Vector3(0, 0, z);
    }
}
