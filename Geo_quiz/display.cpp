#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>
using namespace std;
//TODO:
//1: Linux double click to open
//2: Windows batch file
//2: Add Flags

const char *cantons[27] = { "Lake", "Geneve", "Vaud", "Fribourg","Neuchatel", "Valais", "Berne", "Jura","Soleure", "Lucerne", "Tessin", "Grissons","Obwald", "Nidwald", "Uri", "Glaris","Saint-Gall", "Appenzell Stadt", "Appenzell Campagne", "Schwyz","Zug", "Bale Ville", "Bale Campagne","Argovie", "Thurgovie", "Zurich","Schaffhouse"};
const char *cantonNums[27] = {"O","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q"};


//Run everytime to clear map of other junk
//Must find better method of clearing junk, this is way to resource intensive
void background(){
  string myText = "bruh";
  int i = 5;
  ifstream MyReadFile("LaSuisseBlanc.txt");
  while (getline (MyReadFile, myText)) {
    move(i,0);
    printw(myText.data());
    i++;
  }
  refresh();
}


class Player {
  public:
    string player;
    bool mode = true;
    int cantonSelect = 0;
    int playerY;
    int playerX;
    int playerDirection;//1up, 2right, 3down, 4left.
    Player(int py,int px){
      playerY = py;
      playerX = px;

    }

    void onMove(int playerDirection){
      background();
      //update player position
      if(mode){
        switch (playerDirection) {
          case 119:
            if(playerY != 5){
              playerY--;
            }
            break;
          case 100:
            if(playerX != 110){
              playerX+=2;
            }
                  break;
          case 115:
            if(playerY != 36){
              playerY++;
            }
                  break;
          case 97:
            if(playerX != 0){
              playerX-=2;
            }
                  break;
        }
      }else{
        switch(playerDirection){
          case 119:
            if(cantonSelect!=0){
              cantonSelect--;
            }
            break;
          case 115:
            if(cantonSelect!= 26){
              cantonSelect++;
            }
            break;
        }
      }
      int i = 0;
      string myText = "bruh";
      if(mode){
        //find what canton player is standing on



        ifstream Fileio("LaSuisse.txt");
        while (getline (Fileio, myText)) {
          if(i+5 == playerY){
            player = myText.at(playerX);
          }
          i++;
        }
        i = 0;
      }else{
        player = cantonNums[cantonSelect];
      }
      //highlight canton player is standing on
      i = 5;
      ifstream MyReadFile("LaSuisse.txt");
      while (getline (MyReadFile, myText)) {

        int location = 0;
        int prevLocation = -1;

        while(location != prevLocation){

          if(location<0){
            break;
          }

          prevLocation = location;
          location = myText.find(player,location+1);

          if(player != "."){
            attron(COLOR_PAIR(7));
            mvprintw(i,location,"#");
            attroff(COLOR_PAIR(7));
          }
        }
        i++;
      }
      i = 0;

      //draw player
      attron(COLOR_PAIR(7));
      mvprintw(playerY,playerX,"X");
      attroff(COLOR_PAIR(7));
      refresh();

    }
};
Player john(7,10);

int main(int argc, char ** argv) {
  int iterative = 0;//tracking cycles (dev)
  initscr();//beginning
  start_color();
  init_pair(7,COLOR_RED,COLOR_BLACK);
  init_pair(1,COLOR_BLACK,COLOR_BLACK);
  init_pair(2,COLOR_BLACK,COLOR_RED);
  init_pair(3,COLOR_BLACK,COLOR_GREEN);
  init_pair(4,COLOR_BLACK,COLOR_YELLOW);
  init_pair(5,COLOR_BLACK,COLOR_BLUE);
  curs_set(0);
  noecho();
  cbreak();
  int yMax;
  int xMax;
  getmaxyx(stdscr,yMax,xMax);
  move(10,floor(xMax/2)-3);
  printw("Welcome");
  move(15,floor(xMax/2)-17);
  if(has_colors()){
    printw("Colours are enabled on your terminal");
  }else{
    printw("Colours are disabled on your terminal");
  }
  for(int i = 1; i < 6; i++){
    attron(COLOR_PAIR(i));
    mvprintw(17,floor(xMax/2)-3+i, ".");
    attroff(COLOR_PAIR(i));
  }

  mvprintw(floor(yMax/2),floor(xMax/2)-4,"Controls:");
  mvprintw(floor(yMax/2)+2,floor(xMax/2)-20,"WASD to move, F to flip modes & X to exit");
  mvprintw(yMax-8,floor(xMax/2)-8,"by Timothy Gorst");


  getch();


  WINDOW * win = newwin(3,5,0,0);
  refresh();
  box(win,0,0);
  wrefresh(win);
  move(1,2);
  printw("x");
  move(1,6);
  printw("press x to exit");
  WINDOW * inputwin = newwin(29,30,5,xMax-32);
  refresh();
  box(inputwin,0,0);
  wrefresh(inputwin);

  background();

  while(true){

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    int input = getch();
    if(input == 'x'){
      break;
    }else if(input == 'f'){
      john.mode = !john.mode;
    }
    john.onMove(input);

    for(int i = 0; i < 27; i++){
      if(cantonNums[i] == john.player){
        attron(A_REVERSE);
      }
      mvprintw(i+6,xMax-30,cantons[i]);
      refresh();
      //wrefresh(inputwin);

      attroff(A_REVERSE);
    }


    refresh();

  }
  endwin();//end
  return 0;
}
