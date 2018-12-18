#include "LedControl.h" //  need the library
#include <SimpleList.h>
#include "pitches.h"

#define BUTTON_LEFT_PIN A3
#define BUTTON_MIDDLE_PIN A4
#define BUTTON_RIGHT_PIN A5
#define PIEZO_PIN 8

LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER
 
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219

struct Point
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
    lc.setLed(0, currY, currX, true);
  }
  void hide()
  {
    lc.setLed(0, currY, currX, false);   
  }
  
};

struct Note
{
   Point left;
   Point right;

   int moveDown()
   {
      left.hide();
      right.hide();
      int valLeftY = left.currY + 1;
      int valRightY = right.currY + 1;
      if(valLeftY == 8 && valRightY == 8)
      {
        hide();
        return -1;
      }
        
      left.moveY(valLeftY);
      right.moveY(valRightY);
      
      left.show();
      right.show();
      return 0;
   }

   void hide()
   {
      left.hide();
      right.hide();
   }
   void create(int col)
   {
      left.moveX(col*3);
      left.moveY(0);
      left.show();
      
      right.moveX(col*3 + 1);
      right.moveY(0);
      right.show();
   }
};

struct Song
{
  int melody[16] = {
  NOTE_E4, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_D4, NOTE_FS4, NOTE_A4,
  NOTE_A4, NOTE_G4, NOTE_D5, NOTE_G4, NOTE_B4, NOTE_B3, NOTE_DS4,
  NOTE_A4, NOTE_A4
  };

  int index = 0;
  int melodySize = 16;

  void playNextNote()
  {
    tone(PIEZO_PIN, melody[index], 200);
    index = (index + 1) % melodySize;
  }
  
} song;

struct NotesManager
{
  SimpleList<Note> noteColumns[3];

  int buttonStates[3];
  int lastButtonStates[3] = {LOW, LOW, LOW};
  int lastDebounceTimes[3] = {0, 0, 0};
  int debounceDelay = 25;
  bool wasPressed[3] = {false, false, false};

  int score = 0;

  void addNewNote(int col)
  {
      Note newNote;
      newNote.create(col);
      noteColumns[col].push_front(newNote);
      moveAllDown();    
  }

  void moveAllDown()
  {
      for(auto& col: noteColumns)
      {
          for(auto& note: col)
          {
              if(note.moveDown() != 0)
                col.pop_back();
          }
      }
  }

  void toggleNoteBar(bool on_off)
  {
      lc.setLed(0, 6, 2, on_off);
      lc.setLed(0, 6, 5, on_off);
  }

  void readInputAndUpdateScore(int col)
  {
    int reading;
    bool toUpdate = false;
    switch (col)
    {
      case 0:
        {
          reading = digitalRead(BUTTON_LEFT_PIN);
          break;
        }
      case 1:
        {
          reading = digitalRead(BUTTON_MIDDLE_PIN);
          break;
        }
      case 2:
        {
          reading = digitalRead(BUTTON_RIGHT_PIN);
          break;
        }
    }

    if (reading != lastButtonStates[col])
    {
      lastDebounceTimes[col] = millis();
    }

    if ((millis() - lastDebounceTimes[col]) > debounceDelay)
    {
      if (reading != buttonStates[col])
      {
        buttonStates[col] = reading;

        if (buttonStates[col] == HIGH)// && !wasPressed[col])
        {
          //Serial.println(col);
          toUpdate = true;
          wasPressed[col] = true;
        }
        if(buttonStates[col] == LOW)
        {
          toUpdate = false;
          wasPressed[col] = false;
          //Serial.println(-col);
        }
          
      }
    }

    if (toUpdate)
      updateColumnScore(col);

    lastButtonStates[col] = reading;
  }
  
  void updateColumnScore(int col)
  {
    //song.playNextNote();
    
    Note lastNoteOnColumn = *(noteColumns[col].end() - 1);
    Serial.println("***");
    if (lastNoteOnColumn.left.currY == 5)
    {
      score += 5;
      Serial.println(lastNoteOnColumn.left.currY);
      //lastNoteOnColumn.hide();
      hideLastThree(col);
      noteColumns[col].pop_back();
    }
    else if (lastNoteOnColumn.left.currY == 6)
    {
      score += 10;
      Serial.println(lastNoteOnColumn.left.currY);
      //lastNoteOnColumn.hide();
      hideLastThree(col);
      noteColumns[col].pop_back();
    }
    else if (lastNoteOnColumn.left.currY == 7)
    {
      score += 5;
      Serial.println(lastNoteOnColumn.left.currY);
      //lastNoteOnColumn.hide();
      hideLastThree(col);
      noteColumns[col].pop_back();
    }
    else
    {
      score -= 5;
    }
  }

  void hideLastThree(int col)
  {
    lc.setLed(0, 5, col * 3, false);
    lc.setLed(0, 5, col * 3 + 1, false);
    
    lc.setLed(0, 6, col * 3, false);
    lc.setLed(0, 6, col * 3 + 1, false);
    
    lc.setLed(0, 7, col * 3, false);
    lc.setLed(0, 7, col * 3 + 1, false);
  }
}noteManager;



void setup()
{
  Serial.begin(9600);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  randomSeed(analogRead(0));
  pinMode(BUTTON_LEFT_PIN, INPUT);
  pinMode(BUTTON_MIDDLE_PIN, INPUT);
  pinMode(BUTTON_RIGHT_PIN, INPUT);
  noteManager.toggleNoteBar(true);
  
}

void loop()
{
  song.playNextNote();
  
  //delay(100);
  Serial.print("SCORE: ");
  Serial.println(noteManager.score);
  int col = random(0, 3);
  noteManager.addNewNote(col);

  delay(200);
  noteManager.readInputAndUpdateScore(0);
  noteManager.readInputAndUpdateScore(1);
  noteManager.readInputAndUpdateScore(2);
  
  noteManager.moveAllDown();
  
  delay(200);

  noteManager.readInputAndUpdateScore(0);
  noteManager.readInputAndUpdateScore(1);
  noteManager.readInputAndUpdateScore(2);
  
  noteManager.moveAllDown();
  
}