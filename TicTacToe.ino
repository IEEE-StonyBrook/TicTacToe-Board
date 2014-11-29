//---------------- Picaso librarys ---------------
#include "Picaso_Serial_4DLib.h"                //
#include "Picaso_const4D.h"                     //
//------------------------------------------------
 
//-------Various Arduino setups-------------------
#define DisplaySerial Serial                  // Uncomment if using AR shield or Pins 0, 1 and comment out Serial2 line
//#define DisplaySerial Serial2                   // Mega 2560 digital pins 16 & 17
// Uncomment the following 2 lines and comment out Serial2 line if using sofware serial and set pins accordingly
//#include <SoftwareSerial.h>                   // 
//SoftwareSerial DisplaySerial(2,3) ;           // pin 2 = TX of display, pin3 = RX
Picaso_Serial_4DLib Display(&DisplaySerial);    //
//------------------------------------------------
#define XMARK 1
#define OMARK 0
#define NOMARK -1
#define DRAW -2
char mdl[20]; // handle for display model
int mnt; // handle for sd mount
int Board[10];
int turndicator = OMARK;
void setup(){
 
  // ------------ Initialize screen --------------
  //     Call these functions in this order..   //
  //     Do not address the screen before or    //
  //     within this group.                     //
  delay(2000);                                  // wait for screen to power up
  DisplaySerial.begin(9600);                  // set baud to whatever screen is running
  Display.TimeLimit4D = 5000;                   //
  Display.sys_GetModel(mdl);                    //
  //----------^--- End initialize ---^------------
 
  //------- clear screen and display model -------
  Display.gfx_ScreenMode(PORTRAIT);            // set orientation
  Display.gfx_Cls();                            //
  Display.gfx_MoveTo(0,0);                      //
  Display.putstr("Model=");                     //
  Display.putstr(mdl);                          //
  delay(1500);                                  // optional pause to display model
  //--------------------------------------------
 
  //---Mount uSD-Uncomment if using uSD card------
  Display.gfx_Cls();                            //
  Display.putstr("Mounting...");                //
  mnt = Display.file_Mount();                   //
  Display.txt_MoveCursor(1,0);                  // 
  if (mnt) Display.putstr("Mounted");           // 
  else Display.putstr("Not Mounted");           //
  delay(1500);                                  // optional pause to display mount status
  //----------------------------------------------
 
  //--------- Enable touch and touch region-------
  Display.touch_Set(TOUCH_ENABLE);             //
  Display.touch_DetectRegion(0,0, 399, 239);   //
  Display.gfx_Cls();                           //
  //----------------------------------------------
  Display.txt_Height(2);
  Display.txt_Width(2);
  DrawSplash();
  delay(3000);
  ResetGame();
  //Display.gfx_MoveTo(0,300);
  //Display.putstr("O TURN");
}
 
void DrawSplash() {
  DrawBoard(); 
  drawX(5);
  delay(500);
  drawO(1);
  delay(500);
  drawX(4);
  delay(500);
  drawO(3);
  delay(500);
  drawX(6);
  Display.gfx_MoveTo(0,300);
  Display.putstr("TIC-TAC-TOE");
}
 
void loop(){
  int state;
  state = Display.touch_Get(0);
  int MapX1 = 239;
  int MapX2 = 0;
  int MapY1 = 319;
  int MapY2 = 0;
 
  //holds x y coordinates
  int p;
  int x;
  int y;
  int q;
  x = Display.touch_Get(TOUCH_GETX);
  y = Display.touch_Get(TOUCH_GETY);
 
  p = map(x, 239, 0, MapX1, MapX2);
  q = map(y, 399, -80, MapY1, MapY2);
 
  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (state == TOUCH_RELEASED){
    int pos = GetFingerPos(p,q);
    if (Board[pos] == -1) {
      if (turndicator == OMARK ) {
        Display.gfx_MoveTo(0,300);
        Display.putstr("X TURN");
        drawO(pos);
        Board[pos] = turndicator;
        turndicator = XMARK;
      } 
      else {
        Display.gfx_MoveTo(0,300);
        Display.putstr("O TURN");
        drawX(pos);
        Board[pos] = turndicator;
        turndicator = OMARK;
      }
      int winner = CheckWinner();
      if (winner != NOMARK) {
        Display.gfx_MoveTo(0,300);
        if (winner == XMARK) { 
          Display.putstr("X WINS");
        } 
        if (winner == OMARK) { 
          Display.putstr("O WINS");
        } 
        if (winner == DRAW) {
          Display.putstr("DRAW");
        } 
        delay(2000);
        ResetGame();
      }
    }   
    //  Tft.fillCircle(p.x,p.y,2,RED);
  }
}
 
int CheckWinner() {
 
  int winner = NOMARK;
  //Surely there is a better way?
 
  //Across
  if (Board[1] == Board[2] && Board[1] == Board[3] && winner == NOMARK) {
    winner=Board[1]; 
  }
  if (Board[4] == Board[5] && Board[4] == Board[6] && winner == NOMARK) {
    winner=Board[4]; 
  }
  if (Board[7] == Board[8] && Board[7] == Board[9] && winner == NOMARK) {
    winner=Board[7]; 
  }  
 
  //down
  if (Board[1] == Board[4] && Board[1] == Board[7] && winner == NOMARK) {
    winner=Board[1]; 
  }
  if (Board[2] == Board[5] && Board[2] == Board[8] && winner == NOMARK) {
    winner=Board[2]; 
  }
  if (Board[3] == Board[6] && Board[3] == Board[9] && winner == NOMARK) {
    winner=Board[3]; 
  }
 
  //diagonal
  if (Board[1] == Board[5] && Board[1] == Board[9] && winner == NOMARK) {
    winner=Board[1]; 
  }
 
  if (Board[3] == Board[5] && Board[3] == Board[7] && winner == NOMARK) {
    winner=Board[3]; 
  }
 
  // finally, check and see if all of the spaces are occupied.. 
  if (winner == NOMARK && Board[1] > -1 && Board[2] > -1 && Board[3] > -1 && Board[4] > -1 && Board[5] > -1 && Board[6] > -1 && Board[7] > -1 && Board[8] > -1 && Board[9] > -1 ) { 
    winner = DRAW; 
  }
 
  return winner;
}
 
void ResetGame() {
  DrawBoard(); // Draw the board
  Display.gfx_RectangleFilled(0,360,239,399,BLACK);
  for (int i=0;i<10;i++) { 
    Board[i] = NOMARK; 
  }
  Display.gfx_MoveTo(0,300);
  Display.putstr("O TURN");
}
 
void DrawBoard() {
  // I draw the game board First, blank the screen Then draw the board
  // X blocks are 80px wide
  // y blocks are 106px wide
  // x's and o's are 75% of block width, so call it 60x60 square.  
 
  Display.gfx_RectangleFilled(0,0,239,319,BLACK);
  Display.gfx_Line(0,80,239,80,RED);  
  Display.gfx_Line(0,160,239,160,RED);  
  Display.gfx_Line(80,0,80,239,RED);  
  Display.gfx_Line(160,0,160,239,RED);    
}
 
int GetFingerPos(int x, int y) {
  int row =0;
  int col =0;
 
  col = x/80;
  row = y/80;
 
  int pos = row * 3 + col +1 ;
  return pos; 
}
 
void drawX(int pos) { 
  // I draw an X at an arbitrary position on the screen
  Display.gfx_Line(getCenterX(pos)-30,getCentery(pos)-30, getCenterX(pos)+30, getCentery(pos)+30,YELLOW);
  Display.gfx_Line(getCenterX(pos)+30,getCentery(pos)-30, getCenterX(pos)-30, getCentery(pos)+30,YELLOW);
 
}
 
void drawO(int pos) { 
  Display.gfx_Circle(getCenterX(pos),getCentery(pos), 30 ,YELLOW);
}
 
int getCenterX( int pos ) {
  // I return the center x position for a given spot on the board.
 
  if (pos == 1) return 40;
  if (pos == 4) return 40;
  if (pos == 7) return 40;
  if (pos == 2) return 120;
  if (pos == 5) return 120;
  if (pos == 8) return 120;
  if (pos == 3) return 200;
  if (pos == 6) return 200;
  if (pos == 9) return 200;
 
}
 
int getCentery ( int pos ) {
  // I return the center y position for a given spot on the board.
 
  if (pos == 1) return 40;
  if (pos == 2) return 40;
  if (pos == 3) return 40;
  if (pos == 4) return 120;
  if (pos == 5) return 120;
  if (pos == 6) return 120;
  if (pos == 7) return 200;
  if (pos == 8) return 200;
  if (pos == 9) return 200;
}
 
