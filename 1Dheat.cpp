#include <bits/stdc++.h>
using namespace std;

const double K = 0.1;

const double LX = 1.0;
const double DX = 0.05;
const int NX = (LX / DX) + 1;

const double TF = 0.1;
double DT = 0.01;

int main() {

start: 
  int STEPS = TF / DT;

  double ** T = (double **) malloc(sizeof(double *) * (STEPS + 1));
  for (int t = 0; t <= STEPS; t++) {
    T[t] = (double *) malloc(sizeof(double) * NX);
    T[t][0] = 300, T[t][NX - 1] = 300;
  }
  
  for (int j = 1; j < NX - 1; j++)
    T[0][j] = 100;

  double r = (K * DT) / pow(DX, 2);
  for (int t = 0; t < STEPS; t++)
    for (int i = 1; i < NX - 1; i++) {
      T[t + 1][i] = T[t][i];
      T[t + 1][i] += r * (T[t][i - 1] - 2 * T[t][i] + T[t][i + 1]);
    }

  if (DT == 0.01) {
    cout << "a) \n\n";
  } else {
    cout << "b) \n\n";
  }

  cout << fixed << setprecision(5);
  for (int t = 0; t < STEPS; t++) {
    for (int i = 0; i < NX; i++)
      cout << T[t][i] << ' ';
    cout << '\n';
  }

  cout << "\nFinal Bar Temperature: \n";
  long double sum = 0;
  for (int i = 0; i < NX; i++) {
    cout << T[STEPS][i] << ' ';
    sum += fabs(T[STEPS][i]);
  }

  if (DT == 0.05) {
    cout << "\nMedium Temperature (Accounting for Negative Error): " << sum/NX << '\n';
  } else {
    cout << "\nMedium Temperature: " << sum/NX << '\n';
  }

  long double a = 0.1, t = DT, x = 0.05, L = 1.0;
  long double Ti = 100.0, Ts = 300.0;

  sum = 0;
  for (int m = 1; m < 1000000; m++)
      sum += exp(-pow(m * M_PI / L, 2) * a * t) * (1 - pow(-1, m)) / (m * M_PI) * sin(m*M_PI*x/L);

  long double Tf = Ts + 2 * (Ti - Ts) * sum;
  cout << "Medium Temperature Using Definition: " << Tf << '\n';

  if (DT == 0.01) {
    DT = 0.05;
    cout << "\n==================\n\n";
    goto start;
  }

  return 0;
}
