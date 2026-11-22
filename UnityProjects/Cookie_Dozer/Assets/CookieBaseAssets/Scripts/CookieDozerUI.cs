using UnityEngine;
using UnityEngine.UI;

public class CookieDozerUI : MonoBehaviour
{
    [SerializeField] Text cookieAmountText;
    [SerializeField] Text recoverySecondsText;
    [SerializeField] GameObject[] shotPowerIcons;

    public static CookieDozerUI Instance { get; private set; }

    public void Awake()
    {
        Instance = this;
        CookieAmount = 0;
        RecoverySeconds = 0;
        ShotPower = 0;
    }

    public int CookieAmount
    {
        set => cookieAmountText.text = $"Cookies: {value}";
    }

    public int ShotPower
    {
        set
        {
            var power = Mathf.Clamp(value, 0, shotPowerIcons.Length);

            for (int i = 0; i < shotPowerIcons.Length; i++)
            {
                shotPowerIcons[i].SetActive(i < power);
            }
        }
    }

    public int RecoverySeconds
    {
        set
        {
            recoverySecondsText.gameObject.SetActive(value != 0);
            recoverySecondsText.text = $"回復まで：{value}sec";
        }
    }
}
