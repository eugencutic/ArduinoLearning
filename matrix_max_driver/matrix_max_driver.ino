#include "LedControl.h" //  need the library
#include "QueueList.h"
#include <SimpleList.h>
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER
 
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219

bool matrix[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

void drawMatrix()
{
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      lc.setLed(0, row, col, matrix[row][col]);
    }
}

}

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
    matrix[currY][currX] = 1;
    drawMatrix();
  }
  void hide()
  {
    matrix[currY][currX] = 0;
    drawMatrix();   
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
}testNote;

struct NotesManager
{
  SimpleList<Note> noteColumns[3];

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
}testManager;

  int col = 0;
void setup()
{
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  randomSeed(analogRead(0));
  
}

void loop()
{
  col = random(0, 3);
  testManager.addNewNote(col);
  delay(25);
  testManager.moveAllDown();
  delay(25);
}
