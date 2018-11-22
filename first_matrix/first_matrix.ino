#define PIN_X A1
#define PIN_Y A0

const int row[] = {6, 11, A5, 9, 5, A4, 4, A2};

                 //C1 C2 C3 C4  C5 C6  C7  C8 - anozi
const int col[] = {10, 3, 2, 7, A3, 8, 12, 13};

struct punct
{
  int currX = 0;
  int currY = 0;
  int prevX = 0;
  int prevY = 0;
  bool movedX = false;
  bool movedY = false;
  
  void moveX(int x)
  {
    prevX = currX;
    currX = x;
    movedX = true;
  }
  void moveY(int y)
  {
    prevY = currY;
    currY = y;
    movedY = true;
  }
  void setNotMoved()
  {
    movedX = movedY = false;
  }
  void show()
  {
    digitalWrite(col[prevX], LOW);
    digitalWrite(row[prevY], HIGH);

    digitalWrite(col[currX], HIGH);
    digitalWrite(row[currY], LOW);
  }
  
}pct;

void setup() {
  for (int i = 0; i < 8; i++) 
  {
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) 
  {
    digitalWrite(col[i], LOW);
    digitalWrite(row[i], HIGH);
  }

  pinMode(PIN_X, INPUT);
  pinMode(PIN_Y, INPUT);

  digitalWrite(col[0], HIGH);
  digitalWrite(row[0], LOW);
  
  randomSeed(analogRead(0));

  Serial.begin(9600);
}

void lightRandomLED()
{
  int colNo = random(0, 8);
  int rowNo = random(0, 8);

  digitalWrite(col[colNo], HIGH);
  digitalWrite(row[rowNo], LOW);

  delay(1000);

  digitalWrite(col[colNo], LOW);
  digitalWrite(row[rowNo], HIGH);
}

void loop() {
  delay(150);
  int valY = analogRead(PIN_Y);
  int valX = analogRead(PIN_X);

  if(valY > 600 && !pct.movedY)
  {
    pct.moveY(pct.currY + 1 > 7 ? 0 : pct.currY + 1);
    pct.show();
  }
  if(valY < 400 && !pct.movedY)
  {
    pct.moveY(pct.currY - 1 < 0 ? 7 : pct.currY - 1);
    pct.show();
  }

  if(valX > 600 && !pct.movedX)
  {
    pct.moveX(pct.currX + 1 > 7 ? 0 : pct.currX + 1);
    pct.show();
  }
  if(valX < 400 && !pct.movedX)
  {
    pct.moveX(pct.currX - 1 < 0 ? 7 : pct.currX - 1);
    pct.show();
  }

  if(valX <= 600 && valX >= 400 && valY >= 400 && valY <= 600)
    pct.setNotMoved();
  
}
