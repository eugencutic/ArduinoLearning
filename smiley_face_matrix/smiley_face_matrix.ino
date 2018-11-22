#define PIN_X A1
#define PIN_Y A0

const int row[] = {6, 11, A5, 9, 5, A4, 4, A2};

                 //C1 C2 C3 C4  C5 C6  C7  C8 - anozi
const int col[] = {10, 3, 2, 7, A3, 8, 12, 13};

void on(int x, int y)
{
  digitalWrite(row[x], LOW);
  digitalWrite(col[y], HIGH);
}

void off(int x, int y)
{
  digitalWrite(row[x], HIGH);
  digitalWrite(col[y], LOW);
}

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
  
  Serial.begin(9600);
}

void makeSmiley()
{
  on(1, 2); on(1, 5);
  off(1, 2);  off(1, 5);
  on(2, 2); on(2, 5);
  off(2, 3); off(2, 5);
  on(3, 2); on(3, 5);
  off(3, 2); off(3, 5);
  on(5, 1); on(5, 6);
  off(5, 1); off(5, 6);
  on(6, 2); on(6, 3); on(6, 4); on(6, 5);
  off(6, 2); off(6, 3); off(6, 4); off(6, 5);
}

void loop() {
  makeSmiley();

}
