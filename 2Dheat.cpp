#include <bits/stdc++.h>
using namespace std;

const int NX = 10;           // Number of grid points in the x-direction
const int NY = 10;           // Number of grid points in the y-direction
const double LENGTH_X = 1.0; // Length of the domain in the x-direction
const double LENGTH_Y = 1.0; // Length of the domain in the y-direction
const double TIME_MAX = 1.0; // Maximum simulation time
const double K = 0.1;        // Thermal diffusivity coefficient
const double DX = LENGTH_X / (NX - 1); // Grid spacing in the x-direction
const double DY = LENGTH_Y / (NY - 1); // Grid spacing in the y-direction
const double DT = 0.001;               // Time step size

void initializeT(vector<vector<double>> &T) {
  for (int i = 0; i < NY; i++)
    for (int j = 0; j < NX; j++)
      T[i][j] = 0.0;
  for (int i = 0; i < NY; i++) {
    T[i][0] = 100.0;
    T[i][NX - 1] = 100.0;
  }
  for (int j = 0; j < NX; j++) {
    T[0][j] = 100.0;
    T[NY - 1][j] = 100.0;
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

void simulateHeatTransfer(vector<vector<double>> &T) {

  vector<vector<double>> newT(NY, vector<double>(NX, 0.0));
  initializeT(newT);

  int numSteps = TIME_MAX / DT;
  for (int step = 0; step < numSteps; ++step) {
    for (int i = 1; i < NX - 1; ++i)
      for (int j = 1; j < NY - 1; ++j) {
        double rX = (K * DT) / pow(DX, 2);
        double rY = (K * DT) / pow(DY, 2);
        double dX = rX * (T[i][j - 1] - (2 * T[i][j]) + T[i][j + 1]);
        double dY = rY * (T[i - 1][j] - (2 * T[i][j]) + T[i + 1][j]);
        newT[i][j] = T[i][j] + dX + dY;
      }
    T = newT;
    printT(T);
  }
}

int main() {

  cout << fixed << setprecision(2);
  vector<vector<double>> T(NY, vector<double>(NX, 0.0));
  initializeT(T);
  simulateHeatTransfer(T);

  return 0;
}
