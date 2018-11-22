#define PIN_A 7
#define PIN_B 6
#define PIN_C 4
#define PIN_D 3
#define PIN_E 2
#define PIN_F 8
#define PIN_G 9
#define PIN_DP 5
#define SW_PIN 12
#define X_PIN A1
#define Y_PIN A0

byte matrix_digit[10][7] = {{1, 1, 1, 1, 1, 1, 0}, // 0
                            {0, 1, 1, 0, 0, 0, 0}, // 1
                            {1, 1, 0, 1, 1, 0, 1}, // 2
                            {1, 1, 1, 1, 0, 0, 1}, // 3
                            {0, 1, 1, 0, 0, 1, 1}, // 4
                            {1, 0, 1, 1, 0, 1, 1}, // 5
                            {1, 0, 1, 1, 1, 1, 1}, // 6
                            {1, 1, 1, 0, 0, 0, 0}, // 7
                            {1, 1, 1, 1, 1, 1, 1}, // 8
                            {1, 1, 1, 1, 0, 1, 1}};  // 9

byte inv_matrix_digit[10][7] = {{1, 1, 1, 1, 1, 1, 0}, // 0
                                {0, 0, 0, 0, 1, 1, 0}, // 1
                                {1, 0, 1, 1, 0, 1, 1}, // 2
                                {1, 0, 0, 1, 1, 1, 1}, // 3
                                {0, 1, 0, 0, 1, 1, 1}, // 4
                                {1, 1, 0, 1, 1, 0, 1}, // 5
                                {1, 1, 1, 1, 1, 0, 1}, // 6
                                {1, 0, 0, 0, 1, 1, 0}, // 7
                                {1, 1, 1, 1, 1, 1, 1}, // 8
                                {1, 1, 0, 1, 1, 1, 1}};  // 9

byte dow_matrix_digit[10][7] = {{1, 1, 1, 1, 1, 1, 0}, // 0
                            {0, 1, 1, 0, 0, 0, 0}, // 1
                            {1, 1, 0, 1, 1, 0, 1}, // 2
                            {1, 0, 0, 1, 1, 1, 1}, // 3
                            {0, 0, 1, 0, 1, 1, 1}, // 4
                            {1, 0, 1, 1, 0, 1, 1}, // 5
                            {1, 1, 1, 1, 0, 1, 1}, // 6
                            {0, 0, 0, 1, 1, 1, 0}, // 7
                            {1, 1, 1, 1, 1, 1, 1}, // 8
                            {1, 0, 1, 1, 1, 1, 1}};  // 9

int pins[8] = {PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G};
int count = 5;
int switch_value;
int x_value, y_value;
bool joyMoved = false;
bool invert = false;
bool upDown = false;


void afisNr(int nr) {

  for(int i = 0; i < 7; i++)
  {
     digitalWrite(pins[i], matrix_digit[nr][i]);
  }
}

void afisInv(int nr)
{
  for(int i = 0; i < 7; i++)
  {
     digitalWrite(pins[i], inv_matrix_digit[nr][i]);
  }
}

void afisUpDown(int nr)
{
  for(int i = 0; i < 7; i++)
  {
     digitalWrite(pins[i], dow_matrix_digit[nr][i]);
  }
}

void setup() {
  
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_E, OUTPUT);
  pinMode(PIN_F, OUTPUT);
  pinMode(PIN_G, OUTPUT);  
  pinMode(PIN_DP, OUTPUT);
  pinMode(SW_PIN, INPUT);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);

  digitalWrite(SW_PIN, HIGH); //SW_PIN are valoarea 1 neapasat si 0 apasat, nu stiu daca e ok
  Serial.begin(9600);

}

void loop() {
  switch_value = digitalRead(SW_PIN);
  x_value = analogRead(X_PIN);
  y_value = analogRead(Y_PIN);
 
  /*Serial.print("Switch:  ");
  Serial.print(switch_value);
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(x_value);
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.print(y_value);
  Serial.print("\n\n");
  delay(500);*/

  if(!invert && !upDown)
    afisNr(count);
  else 
  {
    if(upDown)
    {
      afisUpDown(count);
    }
    else
      afisInv(count);
    
  }
    
    
  if(y_value > 600 && !joyMoved)
  {
    count++;
    joyMoved = true;
  }
  if(y_value < 400 && !joyMoved)
  {
    count--;
    joyMoved = true;
  }
  if(count == 10)
    count = 0;
  if(count == -1)
    count = 9;
    
  if(switch_value != 1 && !joyMoved)
  {
    invert = !invert;
    joyMoved = true;
    delay(100);
  }
  if(x_value < 400 && !joyMoved)
  {
    upDown = true;
    joyMoved = true;
  }
  if(x_value > 600 && !joyMoved)
  {
    upDown = false;
    joyMoved = true;
  }
  
  if(y_value >= 400 && y_value <= 600 && x_value >= 400 && x_value <= 600)
    joyMoved = false;
}
