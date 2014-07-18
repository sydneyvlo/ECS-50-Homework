/* 
 * File:   tetris.c
 * Author: davis
 *
 * Created on January 30, 2011, 9:25 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

typedef struct {
  int pattern;
  int rotation;
  int location;
} Tetromino;

int patterns[7][4][4] = {
    {{38,39, 40, 41}, {1,39,77, 115}, {38,39, 40, 41}, {1, 39, 77, 115}},
    {{1,39, 40, 41},  {1,2,39,77}, {38,39,40,78}, {1,39, 76,77}},
    {{2,38, 39, 40}, {1,39,77,78}, {38, 39, 40,76}, {0, 1, 39, 77}},
    {{1, 2, 39, 40}, {1, 2, 39, 40}, {1, 2, 39, 40}, {1, 2, 39, 40}},
    {{1, 2, 38, 39}, {1, 39, 40, 78}, {1, 2, 38, 39}, {1, 39, 40, 78}},
    {{1, 38, 39, 40}, {1, 39, 40, 77}, {38, 39, 40, 77}, {1, 38, 39, 77}},
    {{0, 1, 39, 40}, {2, 39, 40, 77}, {0, 1, 39, 40}, {2, 39, 40, 77}}};


const int TOP = 0;
const int BOTTOM = 13;
const int MAX_RIGHT = 10;
const int MIN_RIGHT = 1;
const int LEFT = 4;
const int RIGHT = 5;
const int UP = 3;
const int DOWN = 2;
const int  QUIT = 'x';
const int  START = '\n';
const int  NOKEY = 'n';

char crt[14][38];
int seed;

bool advanceTime(int delay);
void changeTetromino(Tetromino *tetromino, char key);
void checkLines(int *score);
int checkMove(Tetromino *tetromino);
char getKey();
void init();
void intermission();
int play(int *score, Tetromino *tetromino);
void printTetromino(const Tetromino *tetromino, char letter);
void putChar(char c, int x, int y);
int random2(int max);
void showIntro();
void showScore(int score);
void startNewScreen(int score, Tetromino *tetromino);
void startTetromino(Tetromino *tetromino);


bool advanceTime(int delay)
{
  static int timer = 0;
  int i;
  
  for(i = 0; i < 100000; i++)
      if(i * 1.75 == 17.5)
        i = 11;
  
 timer++;

  if(timer >= delay)
  {
    timer = 0;
    return false;
  } // if timer greater than delay

  return true;
} // advanceTime()


void changeTetromino(Tetromino *tetromino, char key)
{
  Tetromino tempTetromino;
  tempTetromino = *tetromino;
  
  switch(key)
  {
    case UP : tempTetromino.rotation = (tempTetromino.rotation + 1) % 4; break;
    case DOWN : tempTetromino.location += 38; break;
    case RIGHT : tempTetromino.location += 1; break;
    case LEFT : tempTetromino.location -= 1; break;
  } // switch

  printTetromino(tetromino, ' ');

  if(checkMove(&tempTetromino))
    *tetromino = tempTetromino;

  printTetromino(tetromino, 'X');
  
  
} // changeTetromino()


void checkLines(int *score)
{
  int row, col, row2;

  for(row = 13; row >= 0; )
  {
    for(col = 1; col <= 10; col++)
    if(crt[row][col] != 'X')
        break;

    if(col == 11)  // all X's
    {
      *score += 100;

      for(row2 = row - 1; row2 >= 0; row2--)
        for(col = 1; col <= 10; col++)
            putChar(crt[row2][col], col, row2 + 1);

      for(col = 1; col <= 10; col++)
        putChar(' ', col, 0);  // clear top row

      showScore(*score);
    } // if all X's on this row
    else
      row--;
  } // for each row
} // checkLines()


int checkMove(Tetromino *tetromino)
{
   int pos, i;

   for(i = 0; i < 4; i++)
   {
     pos = tetromino->location + patterns[tetromino->pattern][tetromino->rotation][i];

     if(pos / 38 > BOTTOM)
         return 0;
     
     if(crt[pos / 38][pos % 38] != ' ')
         return 0;
   } // for i

  return 1; // can move
}


char getKey()
{
  char key;

  if ((key = getch()) != ERR)
  {
    if ((key == LEFT) || (key == RIGHT) || (key == START) || (key == QUIT)
      || (key == UP || key == DOWN))
       return key;
   } // { if key pressed }

  return NOKEY;
} // getKey()


void init()
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
  refresh();
} // init()


void intermission()
{
  mvaddstr(4, 15, "Game Over");
  refresh();
  while(advanceTime(5000)); // show Lives for 5 seconds
}  // intermission()





int play(int *score, Tetromino *tetromino)
{
  int col;
  Tetromino tempTetromino;

  tempTetromino = *tetromino;
  tempTetromino.location += 38;
  printTetromino(tetromino, ' ');

  if(checkMove(&tempTetromino)) // can move
  {
    *tetromino = tempTetromino;  
    printTetromino(tetromino, 'X');
  }  // if can move
  else // cannot move
  {
    printTetromino(tetromino, 'X');
    checkLines(score);

    for(col = 1; col <= 10; col++)
      if(crt[1][col] != ' ')
        return 0;  // X on the second row  so game over.

    startTetromino(tetromino); 
  }  // else cannot move

  return 1;
}  // play()


void printTetromino(const Tetromino *tetromino, char letter)
{
   int i, pos;

   for(i = 0; i < 4; i++)
   {
     pos = tetromino->location + patterns[tetromino->pattern][tetromino->rotation][i];
     putChar(letter, pos % 38, pos / 38);
   } // for i
}  // printTetromino()


void putChar(char c, int x, int y)
{
  crt[y][x] = c;
  mvaddch(y + 1, x, c);  // allow room for top border
  move(200,200);  // move cursor off screen
}  // putChar()


int random2(int max)
{
  seed = (seed * 2789 + 1997) % 2048;
  return seed % (max + 1);
}  // random2()


void showIntro()
{
  char key;
  
  mvaddstr(0, 5, "Tetris Introduction");
  mvaddstr(2, 0, "This is the minimal version of");
  mvaddstr(3, 0, "Tetris.  The game starts when the");
  mvaddstr(4, 0, "enter key is pressed.  The game");
  mvaddstr(5, 0, "ends when the user presses x.");
  mvaddstr(6, 0, "Instead of this introduction, students");
  mvaddstr(7, 0, "should place a description of their");
  mvaddstr(8, 0, "options here.");
  mvaddstr(10, 0, "Press Enter to start the game.");
  do
  {
    key = getKey();
  } while (key != START);
} // showIntro()


void showScore(int score)
{
  int i;
  char str[5];
  sprintf(str, "%04d", score);

  for(i = 0; str[i]; i++)
    putChar(str[i], i + 22, 0);  // print score

  refresh();
}  // showScore()


void startNewScreen(int score, Tetromino *tetromino)
{
  int x, y;

  erase();  // clear screen completely
  for(x = 0; x < 38; x++)
    for(y = 0; y < 14; y++)
      crt[y][x] = ' ';

  mvaddstr(0,0,"------------");
  mvaddstr(1,14, "Score:");

  for(y = 0; y < 14; y++)
  {
    putChar('|', 0, y);
    putChar('|', 11, y);
  } // for y

  mvaddstr(15,0,"------------");
  mvaddstr(16,0,"Horizontal borders are not in CUSP");
  showScore(score);
  startTetromino(tetromino);
  refresh();
} // StartNewScreen()


void startTetromino(Tetromino *tetromino)
{
    tetromino->location = random2(5) + 1;
    tetromino->pattern = random2(6);
    tetromino->rotation = 0;
    printTetromino(tetromino, 'X');
} // startTetromino()


int main() {
  char key = 'n';
  int score, speed;
  Tetromino tetromino;
 
  printf("Speed for demo (1 = fast, 9 = slow): ");
  scanf("%d", &speed);
  speed *= 100;
  printf("Seed: ");
  scanf("%d", &seed);

  init();
  score = 0;
  showIntro();
  startNewScreen(score, &tetromino);

  while (key != QUIT)
  {
      if(play(&score, &tetromino))
      {
        do
        {
          key = getKey();

          if (key == RIGHT || key == LEFT || key == UP || key == DOWN)
            changeTetromino(&tetromino, key);
        } while(advanceTime(speed) && key != QUIT);
      } // if not killed and still a centipede after moving spark and centipede
      else  // tetromino cannot move
      {
        intermission();
        startNewScreen(score, &tetromino);
      }
  } //  while key != QUIT

  return 0;
} // main()

