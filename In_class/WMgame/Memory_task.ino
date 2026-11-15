//UNOのピン設定
const int datapin  = 12;
const int latchpin = 11; 
const int clockpin = 9; 

const int numbers[4] = {8, 16, 32, 64};
const int max_record=5;//ここは任意に設定可能

// ジョイスティック設定
const int Xpin = 0;
const int SWpin = 2; 
const int low_border = 300;
const int high_border = 700;

// 現在プレイヤーが選択しているLEDの位置(8,16,32,64)
int sel_LED = 8;
//モード切替/0開始待ち/1指示表示/2入力OK/3ゲームオーバー/4ゲームクリア
int mode = 0;
//連続記録
int record = 0;
//今のところ何回連続で成功しているか
int now = 0;
//LEDの順番を格納
int order[max_record]={0};
//モードが切り替わったかどうか
bool mode_switch=false;

//int randomINDEX;

void setup()
{
  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);

  pinMode(SWpin, INPUT);
  digitalWrite(SWpin, HIGH);
  Serial.begin(9600);
  while(! Serial);
  randomSeed(analogRead(A3));
  // 最初のLEDを点灯させる
  //updateLEDs();
  mode_switch=true;
}

void loop()
{
  int is_pressed=digitalRead(SWpin);//0が押された。1が押されていない
  if(is_pressed==0){
    Serial.print("\npressed\n");
  }

  //開始待ち。開始されたらすべての盤面を初期化する。
  if(mode==0){
    if(is_pressed==0)
    {
      Serial.print("\nLet's start!\n");
      sel_LED=8;mode=1;record=0;
      memset(order, 0, sizeof(order));now=0;
      mode_switch=true;
      delay(500);
      return;
    }
    else{
      if(mode_switch){
        Serial.print("Press the button.\n");
        updateLEDs(mode,0);
        mode_switch=false;
      }
      return;
    }
  }
  //クリアした場合、ボタンが押されたらタイトルへ
  if(mode==4){
    if(is_pressed==0)
    {
      record=0;mode=0;
      mode_switch=true;
      delay(500);
      return;
    }
    else if(!mode_switch)
    {
      return;
    }
    else
    {
      updateLEDs(mode, 0);
      Serial.print("\nCongratulations!\n");
      Serial.print("Will you go back to the title?\n");
      Serial.print("If you want, press the button.\n");
      mode_switch=false;
      return;
    }
  }
  
  //ゲームオーバー
  if(mode==3){
    if(is_pressed==0)
    {
      record=0;mode=0;
      mode_switch=true;
      delay(500);
      return;
    }
    if(!mode_switch){
      return;
    }
    else{
      updateLEDs(mode,0);
      Serial.print("\nYou failed! Your record is ");
      Serial.print(record);
      Serial.print("\nTry again!\n");
      mode_switch=false;
      return;
    }
  }

  //入力すべきLEDの順番を表示
  if(mode==1){
    if(record==max_record){
      mode=4;mode_switch=true;return;
    }
    /*if(is_pressed==0){
      mode=2;mode_switch=true;
      delay(500);
      return;
    }*/
    if(mode_switch){
      Serial.print("\nround ");
      Serial.print(record+1);
      Serial.print("\n");
      updateLEDs(mode, 0);
      delay(1000);
      order[record]=numbers[random(0,4)];
      mode_switch=false;
    }
    
    for(int i=0;i<record+1;++i)
    {
      updateLEDs(mode,order[i]);
      delay(300);
      updateLEDs(mode, 0);
      delay(300);
    }
    delay(700);
    mode=2;mode_switch=true;
    return;
  }

  int x = analogRead(Xpin);
  bool is_moved = false;
  
  if(mode==2){
    //モード移行時の処理
    if(mode_switch){
      Serial.print("\nYou can input: \n");
      mode_switch=false;
      updateLEDs(mode, sel_LED);
    }

    //スイッチが押されたときの処理
    if(is_pressed==0){
      if(sel_LED==order[now]){
        if(now==record){
          Serial.print("\nCorrect. Next phase.\n");
          ++record;now=0;mode=1;mode_switch=true;
          delay(500);return;
        }
        else{
          Serial.print("Correct\n");
          ++now;//sel_LED=8;
          delay(500);
          return;
        }
      }
      else{
        mode=3;mode_switch=true;
        delay(500);return;
      }
    }
    // 左に倒された && まだ一番左(0)じゃないなら、位置をマイナス1
    if (x < low_border){
      if (sel_LED > 8)
      {
        sel_LED /= 2;
        is_moved = true;
      }
    } 
    // 右に倒された ＆ まだ一番右(3)じゃないなら、位置をプラス1
    else if (x > high_border) {
      if (sel_LED/64 < 1) {
        sel_LED *=2;
        is_moved = true;
      }
    }

    // 位置が変わったときだけICにデータを送ってLEDを更新
    if (is_moved){
      updateLEDs(mode, sel_LED);
      delay(300); // 連続で高速移動するのを防ぐためのウェイト
    }
  }
  
}

// SN74HC595にデータを送る関数
void updateLEDs(int mode, int LED)
{
  byte ledData = mode + LED;

  // ラッチをLOWにして送信開始
  digitalWrite(latchpin, LOW);
  
  // データをICへ送り出す
  shiftOut(datapin, clockpin, MSBFIRST, ledData);
  
  // ラッチをHIGHにしてLEDに反映
  digitalWrite(latchpin, HIGH);
}