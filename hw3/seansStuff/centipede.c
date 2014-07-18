/* 
 * File:   breakout.c
 * Author: davis
 *
 * Created on February 2, 2010, 9:25 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>


typedef struct {
  int icon;
  int x;
  int y;
  int direction;
  int icon_below;
} Centipede;

const int TOP = 0;
const int BOTTOM = 13;
const int MAX_RIGHT = 37;
const int MIN_RIGHT = 1;
const int LEFT = 4;
const int RIGHT = 5;
const int MAX = 33;
const int  QUIT = 'x';
const int  START = '\n';
const int  NOKEY = 'n';
const int FIRE = ' ';
const int  MUSHROOM = 'T';
const int  LEFT_HEAD = '}';
const int RIGHT_HEAD = '{';
const int  BODY = 'O';
const int NO_BODY = ' ';
const int WAND = 'A';
const int SPARK = '*';
const int NO_SPARK = -1;
const int MUSHROOM_POINTS = 10;
const int BODY_POINTS = 10;
const int HEAD_POINTS = 200;
char crt[14][38];


bool advanceTime(int delay);
bool fireWand(int wandX, int sparkXs[]);
char getKey();
int hitCentipede(Centipede centipedes[], int sparkXs[], int counts[14][38],
  int *score, int *bodyCount, int y);
void hitMushroom(int sparkXs[14], int counts[14][38], int *score, int y);
void init();
void intermission(Centipede centipedes[], int*bodyCount, int wandX, int lives,
  char *key);
void moveCentipede(Centipede centipedes[], int *lives);
void moveDown(Centipede *centipedes, int *lives);
void moveSpark(Centipede centipedes[], int sparkXs[], int counts[14][38],
  int *score, int *bodyCount, int y);
int play(Centipede centipedes[16], int sparkXs[14], int counts[14][38],
  int *score, int *lives, int *playCount, int *bodyCount);
void putChar(char c, int x, int y);
int random2(int max);
void showIntro();
void showScore(int score);
void startCentipede(Centipede centipedes[], int *bodyCount);
void startNewScreen(Centipede centipedes[6], int counts[14][38], int sparkXs[14],
  int score, int wandX, int *bodyCount);


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
  }
  return true;
} // advanceTime()


bool fireWand(int wandX, int sparkXs[])
{
  if(sparkXs[BOTTOM - 1] == NO_SPARK)
    // guarantee alternate sparks
    sparkXs[BOTTOM] = wandX;
}

char getKey()
{
  char key;

  if ((key = getch()) != ERR)
  {
    if ((key == LEFT) || (key == RIGHT) || (key == START) || (key == QUIT)
      || (key == FIRE))
       return key;
   } // { if key pressed }

  return NOKEY;
} // getKey()

int hitCentipede(Centipede centipedes[], int sparkXs[], int counts[14][38],
  int *score, int *bodyCount, int y)
{
  int i, y1 = y - 1;

  if(crt[y1][sparkXs[y]] == BODY)
    *score += BODY_POINTS;
  else
    *score += HEAD_POINTS;

  showScore(*score);

  for(i = 0; i < 6; i++)  // find centipede part that matches spark location
    if(centipedes[i].x == sparkXs[y] && centipedes[i].y == y1)
      break;

  centipedes[i++].icon = NO_BODY;

  if(i < 6 && centipedes[i].icon == BODY)  // next part
  {
    if(centipedes[i].direction == RIGHT)
      centipedes[i].icon = RIGHT_HEAD;
    else
      centipedes[i].icon = LEFT_HEAD;

    putChar(centipedes[i].icon, centipedes[i].x, centipedes[i].y);
  } // if next body part is not a head

  putChar(MUSHROOM, sparkXs[y], y1);
  counts[y1][sparkXs[y]] = 4;
  --(*bodyCount);
} // hitCentipede()


void hitMushroom(int sparkXs[], int counts[14][38], int *score, int y)
{
  int y1 = y - 1;
  --counts[y1][sparkXs[y]];

  if(counts[y1][sparkXs[y]] == 0) // destroyed mushroom
  {
    *score += MUSHROOM_POINTS;
    putChar(' ', sparkXs[y], y1); // remove mushroom
  } // if destroyed a mushroom

  showScore(*score);
} // hitMushroom()

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

void intermission(Centipede centipedes[], int *bodyCount, int wandX, int lives,
  char *key)
{
  char str[20];
  int i;

  sprintf(str, "Lives: %d", lives);
  for(i = 0; str[i]; i++)
    putChar(str[i], i + 10, TOP);

  refresh();
  while(advanceTime(5000)); // show Lives for 5 seconds

  for(i = 0; str[i]; i++)
    putChar(' ', i + 10, TOP);

  putChar(WAND, wandX, BOTTOM);

  if(lives > 0)
  {
    for(i = 0; i < 6; i++)
      if(centipedes[i].icon != NO_BODY)
        putChar(' ', centipedes[i].x, centipedes[i].y);

    startCentipede(centipedes, bodyCount);
  }
  else  // lives = 0
  {
    mvaddstr(1,0, "            Game Over                ");
    refresh();
    while(advanceTime(10000)); // show Game Over for 5 seconds
    *key = QUIT;
  } // else lives = 0
}  // intermission()

void moveCentipede(Centipede centipedes[], int *lives)
{
  int i, x, nextIcon;
  Centipede *cptr;

  for(i = 0; i < 6; i++)
  {
    cptr = &centipedes[i];

    if(cptr->icon != NO_BODY)
    {
      if(cptr->direction == RIGHT)
        x = cptr->x + 1;
      else  // moving left
        x = cptr->x - 1;

      if(x > MAX_RIGHT)
      {
        cptr->direction = LEFT;
          if(cptr->icon == RIGHT_HEAD)
            cptr->icon = LEFT_HEAD;
        moveDown(cptr, lives);
      }  // if
      else  // reached the right border
        if(x < 0)
        {
          cptr->direction = RIGHT;
          if(cptr->icon == LEFT_HEAD)
            cptr->icon = RIGHT_HEAD;
          moveDown(cptr, lives);
        }  // reached the left border
        else // not at a border
        {
          nextIcon = crt[cptr->y][x];

          if(nextIcon != LEFT_HEAD && nextIcon != RIGHT_HEAD
            && nextIcon != BODY && nextIcon != MUSHROOM) //move into a space
          {
            putChar(cptr->icon_below, cptr->x, cptr->y);
            cptr->x = x;
            cptr->icon_below = crt[cptr->y][cptr->x];
            
            if(cptr->icon_below == '*')
              cptr->icon_below = ' ';

            putChar(cptr->icon, cptr->x, cptr->y);  // place centipede part
            if(nextIcon == WAND)
              --(*lives);
          } // if moving into a space or WAND
          else // moving into a MUSHROOM or another CENTIPEDE
            moveDown(cptr, lives);
        } // not a border
    } // if a body part
  } // for each body part
}  // moveCentipede()


void moveDown(Centipede *centipede, int *lives)
{
  int y = centipede->y + 1;
  int nextIcon;

  if(y > BOTTOM) 
  {
    if(centipede->direction == RIGHT && centipede->x > 0)
    {
      centipede->direction = LEFT;

      if(centipede->icon == RIGHT_HEAD)
        centipede->icon = LEFT_HEAD;
    } // if moving right
    else  // direction == LEFT
      if(centipede->direction == LEFT && centipede->x < MAX_RIGHT)
      {
        centipede->direction = RIGHT;

        if(centipede->icon == LEFT_HEAD)
        centipede->icon = RIGHT_HEAD;
      }    // else trying to move left

    putChar(centipede->icon, centipede->x, centipede->y);
  } // if trying to move below bottom
  else // not on bottom row
  {
    nextIcon = crt[y][centipede->x];
    if(nextIcon == MUSHROOM || nextIcon == ' ' || nextIcon == WAND)
    {
      putChar(centipede->icon_below, centipede->x, centipede->y);
      centipede->y = y;
      centipede->icon_below = crt[centipede->y][centipede->x];

      if(centipede->icon_below == '*')
              centipede->icon_below = ' ';

      putChar(centipede->icon, centipede->x, centipede->y);  // place centipede part

      if(nextIcon == WAND)
        --(*lives);
    } // if not another centipede
  }  // else not on bottom row
} // moveDown()


void moveSpark(Centipede centipedes[], int sparkXs[], int counts[14][38], int *score,
  int *bodyCount, int y)
{
  int y1 = y - 1;
  if(y > 0)
  {
    if(crt[y1][sparkXs[y]] == ' ') // empty space above
    {
      sparkXs[y1] = sparkXs[y];  // move up spark
      putChar(SPARK, sparkXs[y], y1);  // print spark
    } // if an empty space above then move spark up
    else // not empty space above
      if(crt[y1][sparkXs[y]] == MUSHROOM)
        hitMushroom(sparkXs, counts, score, y);
      else // hit a centipede
        if(crt[y1][sparkXs[y]] == BODY || crt[y1][sparkXs[y]] == LEFT_HEAD
          || crt[y1][sparkXs[y]] == RIGHT_HEAD)
          hitCentipede(centipedes, sparkXs, counts, score, bodyCount, y);
  } // if y > 0

  if(crt[y][sparkXs[y]] == SPARK)
    putChar(' ', sparkXs[y], y);  // remove original spark from screen

  sparkXs[y] = NO_SPARK;  // remove original spark
} // moveSpark()

void moveWand(char key, int *wandX)
  {
    putChar(' ', *wandX, BOTTOM);

    if ((key == LEFT) && (*wandX > 0) && crt[BOTTOM][*wandX - 1] == ' ')
      *wandX = *wandX - 1;
    else // { key is RIGHT }
      if  ((key == RIGHT) &&  (*wandX < MAX_RIGHT) && crt[BOTTOM][*wandX + 1] == ' ')
        *wandX = *wandX + 1;

    putChar(WAND, *wandX, BOTTOM);
  } // moveWand()


int play(Centipede centipedes[], int sparkXs[], int counts[14][38], int *score,
  int *lives, int *playCount, int *bodyCount)
{
  int y, startLives = *lives;

  for(y = 0; y < 14; y++)
    if(sparkXs[y] != NO_SPARK)
    {
      moveSpark(centipedes, sparkXs, counts, score, bodyCount, y);
      if(*bodyCount == 0)
        return 0;  // time to leave play because the centipede is gone
    } // if a spark in this row

  moveCentipede(centipedes, lives);

  return *lives == startLives;
}  // play


void putChar(char c, int x, int y)
{
  crt[y][x] = c;
  mvaddch(y + 1, x + 1, c);  // allow room for top and left border
  move(200,200);  // move cursor off screen
}  // putChar()


int random2(int max)
{
  static int seed = 1;
  seed = (seed * 2789 + 1997) % 2048;
  return seed % (max + 1);
}  // random2()


void showIntro()
{
  char key;
  
  mvaddstr(0, 5, "Centipede Introduction");
  mvaddstr(2, 0, "This is the minimal version of");
  mvaddstr(3, 0, "centipede.  The game starts when the");
  mvaddstr(4, 0, "enter key is pressed.  The game");
  mvaddstr(5, 0, "ends when either the user presses x,");
  mvaddstr(6, 0, "or their lives reaches zero.");
  mvaddstr(7, 0, "Instead of this introduction, students");
  mvaddstr(8, 0, "should place a description of their");
  mvaddstr(9, 0, "options here.");
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
    putChar(str[i], i, TOP);  // print score

  refresh();
}  // showScore()


void startCentipede(Centipede centipedes[], int *bodyCount)
{
  int direction, x, i;

  direction = LEFT + random2(1);

  x = random2(30) + 1; // 1-31, so starts minimum of one from edge
  if(direction == RIGHT)
    x += 5; // guarantee room for body to left of mouth

  for(i = 0; i < 6; i++)
  {
    centipedes[i].direction = direction;
    centipedes[i].icon = BODY;
    centipedes[i].y = TOP;

    if(direction == LEFT)
      centipedes[i].x = x + i;
    else // moving RIGHT
      centipedes[i].x = x - i;
    
    centipedes[i].icon_below = crt[TOP][centipedes[i].x];
    putChar(centipedes[i].icon, centipedes[i].x, TOP);
  } // for each centipede body part

  if(direction == RIGHT)
    centipedes[0].icon = '{';
  else
    centipedes[0].icon = '}';

  putChar(centipedes[0].icon, centipedes[0].x, TOP);
  *bodyCount = 6;
}  // startCentipede()

void startNewScreen(Centipede centipedes[], int counts[14][38], int sparkXs[14],
  int score, int wandX, int *bodyCount)
{
  int i, j, direction, x, y;
  char str[10];

  erase();  // clear screen completely
  for(x = 0; x < 38; x++)
    for(y = 0; y < 14; y++)
    {
      crt[y][x] = ' ';
      counts[y][x] = 0;
    } // for y

  mvaddstr(0,0,"----------------------------------------");

  for(y = 1; y < 15; y++)
    mvaddstr(y, 0, "|                                      |");

  mvaddstr(15,0,"----------------------------------------");
  mvaddstr(16,0,"Borders are not in CUSP");
  showScore(score);

  for(i = 0; i < 14; i++)
    sparkXs[i] = NO_SPARK;  // no spark at each row

  startCentipede(centipedes, bodyCount);

  for(i = 0; i < 20; i++)
  {
    y = random2(11) + 1;
    x = random2(37);
    random2(5); // to avoid having all y's even and x's odd
    putChar(MUSHROOM, x, y);
    counts[y][x] = 4;
  }

  putChar(WAND, wandX, BOTTOM);
  refresh();
} // StartNewScreen()


int main() {
  char key = 'n', str[10];
  int i, score, lives, wandX, playCount, bodyCount, speed;
  int counts[14][38];
  Centipede centipedes[6];
  int sparkXs[14];

  printf("Speed for demo (1 = fast, 9 = slow): ");
  scanf("%d", &speed);
  speed *= 100;
  init();
  bodyCount = score = 0;
  lives = 3;
  wandX = 17;
  showIntro();
  startNewScreen(centipedes, counts, sparkXs, score, wandX, &bodyCount);

  while (key != QUIT)
  {
      if(play(centipedes, sparkXs, counts, &score, &lives, &playCount,
        &bodyCount))
      {
        do
        {
          key = getKey();

          if (key == RIGHT || key == LEFT)
            moveWand(key, &wandX);
          else
            if(key == ' ')
              fireWand(wandX, sparkXs);
        } while(advanceTime(speed) && key != QUIT);
      } // if not killed and still a centipede after moving spark and centipede
      else  // life lost or last centipede part shot
        intermission(centipedes, &bodyCount, wandX, lives, &key);

  } //  while key != QUIT

  return 0;
} // main()

