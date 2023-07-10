#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_render.h>
#include <bits/stdc++.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
using namespace std;

SDL_Window *win;
SDL_Renderer *ren;
int cellWidth, cellHeight;
#define TIME_INTERVAL 50;

const double MAX_TEMP = 300.0;
const double MIN_TEMP = 0.0;
const int TEMP_MEDIA = (MAX_TEMP - MIN_TEMP) / 2;
double BORDER_TEMP = 100.0;
double PLATE_TEMP = 0.0;

const int NX = 20;
const int NY = 20;
const double LENGTH_X = 1.0;
const double LENGTH_Y = 1.0;
// const double TIME_MAX = 1.0;
double K = 0.5;
const double DX = LENGTH_X / (NX - 1);
const double DY = LENGTH_Y / (NY - 1);
const double DT = 0.001;

int AUX_WaitEventTimeoutCount(SDL_Event *evt, int &ms) {
  Uint32 antes = SDL_GetTicks();
  SDL_FlushEvent(SDL_MOUSEMOTION);
  if (SDL_WaitEventTimeout(evt, ms)) {
    ms = MAX(0, ms - (int)(SDL_GetTicks() - antes));
    return 1;
  }
  return 0;
}

void drawMatrix(vector<vector<double>> &T) {

  SDL_Rect box = {0, 0, cellWidth, cellHeight};
  for (int i = 0; i < NX; i++) {
    for (int j = 0; j < NY; j++) {
      int temp = T[i][j] - TEMP_MEDIA;
      if (temp >= 0)
        SDL_SetRenderDrawColor(
            ren, 0xFF, 0x00, 0x00,
            (short)abs(2 * temp * (255.0 / (TEMP_MEDIA * 2))));
      else
        SDL_SetRenderDrawColor(
            ren, 0x00, 0x00, 0xFF,
            (short)abs(2 * temp * (255.0 / (TEMP_MEDIA * 2))));
      SDL_RenderFillRect(ren, &box);
      box.x += cellWidth;
    }
    box.y += cellHeight;
    box.x = 0;
  }
}

void initializeT(vector<vector<double>> &T) {
  for (int i = 0; i < NY; i++)
    for (int j = 0; j < NX; j++)
      T[i][j] = PLATE_TEMP;
  for (int i = 0; i < NY; i++) {
    T[i][0] = BORDER_TEMP;
    T[i][NX - 1] = BORDER_TEMP;
  }
  for (int j = 0; j < NX; j++) {
    T[0][j] = BORDER_TEMP;
    T[NY - 1][j] = BORDER_TEMP;
  }
}

void printT(const vector<vector<double>> &T) {
  for (int i = 0; i < NY; i++) {
    for (int j = 0; j < NX; j++) {
      if (T[i][j] < 10)
        cout << "  " << T[i][j] << ' ';
      else if (T[i][j] < 100)
        cout << ' ' << T[i][j] << ' ';
      else
        cout << T[i][j] << ' ';
    }
    cout << '\n';
  }
  cout << '\n';
}

void calculateEDP(vector<vector<double>> &T, vector<vector<double>> &newT) {
  for (int i = 1; i < NX - 1; i++) {
    for (int j = 1; j < NY - 1; j++) {
      double rX = (K * DT) / pow(DX, 2);
      double rY = (K * DT) / pow(DY, 2);
      double dX = rX * (T[i][j - 1] - (2 * T[i][j]) + T[i][j + 1]);
      double dY = rY * (T[i - 1][j] - (2 * T[i][j]) + T[i + 1][j]);
      newT[i][j] = T[i][j] + dX + dY;
    }
  }
  T = newT;
}

void simulateHeatTransfer(vector<vector<double>> &T) {

  vector<vector<double>> newT(NY, vector<double>(NX, 0.0));
  initializeT(newT);

  SDL_Event evt;
  SDL_Point mouse;
  int espera = TIME_INTERVAL;
  Uint32 antes = SDL_GetTicks();
  int mouseLPressed = false;
  int mouseRPressed = false;

  char s[50];
  // int numSteps = TIME_MAX / DT;
  // for (int step = 0; step < numSteps;) {
  for (int step = 0;;) {

    printT(T);
    cout << espera << "\n\n";
    sprintf(s, "Step: %d", step);

    SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderClear(ren);
    drawMatrix(T);
    stringRGBA(ren, 460, 20, s, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderPresent(ren);

    espera = MAX(0, espera - (int)(SDL_GetTicks() - antes));
    int isevt = AUX_WaitEventTimeoutCount(&evt, espera);
    antes = SDL_GetTicks();

    if (isevt) {
      switch (evt.type) {
      case SDL_WINDOWEVENT:
        if (SDL_WINDOWEVENT_CLOSE == evt.window.event)
          return;
        break;
      case SDL_KEYDOWN:
        for (int i = 48; i <= 53; i++)
          if (i == evt.key.keysym.sym)
            K = (i - 48) / 10.0;
        if (113 == evt.key.keysym.sym) {
          BORDER_TEMP = MIN_TEMP;
          initializeT(newT);
        } else if (119 == evt.key.keysym.sym) {
          BORDER_TEMP = TEMP_MEDIA;
          initializeT(newT);
        } else if (101 == evt.key.keysym.sym) {
          BORDER_TEMP = MAX_TEMP;
          initializeT(newT);
        } else if (97 == evt.key.keysym.sym) {
          PLATE_TEMP = MIN_TEMP;
          initializeT(T);
        } else if (115 == evt.key.keysym.sym) {
          PLATE_TEMP = TEMP_MEDIA;
          initializeT(T);
        } else if (100 == evt.key.keysym.sym) {
          PLATE_TEMP = MAX_TEMP;
          initializeT(T);
        }
        break;
      case SDL_MOUSEMOTION:
        SDL_GetMouseState(&mouse.x, &mouse.y);
        if (mouseLPressed)
          T[mouse.y / cellWidth][mouse.x / cellHeight] = MAX_TEMP;
        else if (mouseRPressed)
          T[mouse.y / cellWidth][mouse.x / cellHeight] = MIN_TEMP;
        break;
      case SDL_MOUSEBUTTONDOWN:
        SDL_GetMouseState(&mouse.x, &mouse.y);
        if (evt.button.button == SDL_BUTTON_LEFT) {
          T[mouse.y / cellWidth][mouse.x / cellHeight] = MAX_TEMP;
          mouseLPressed = true;
        } else if (evt.button.button == SDL_BUTTON_RIGHT) {
          T[mouse.y / cellWidth][mouse.x / cellHeight] = MIN_TEMP;
          mouseRPressed = true;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        mouseLPressed = false;
        mouseRPressed = false;
        break;
      }
    } else {
      espera = TIME_INTERVAL;
      calculateEDP(T, newT);
      step++;
    }
  }

  printT(T);
  drawMatrix(T);
  cout << "End of Iterations\n\n";
  SDL_Delay(1000);
}

int main() {

  SDL_Init(SDL_INIT_EVERYTHING);
  win = SDL_CreateWindow("Heat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         1000, 1000, SDL_WINDOW_SHOWN);
  ren = SDL_CreateRenderer(win, -1, 0);
  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_GetWindowSize(win, &cellWidth, &cellHeight);
  cellWidth /= NX;
  cellHeight /= NY;

  cout << fixed << setprecision(2);
  vector<vector<double>> T(NY, vector<double>(NX, 0.0));
  initializeT(T);
  simulateHeatTransfer(T);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
