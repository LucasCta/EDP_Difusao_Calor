#include <bits/stdc++.h>
using namespace std;

const double TIME_MAX = 0.5;
const double K = 0.1;
const double DX = 0.25;
const double DT = 0.001;

int main() {

  int numSteps = TIME_MAX / DT;
  double **T = (double **)malloc(sizeof(double *) * (numSteps + 1));
  for (int t = 0; t <= numSteps; t++)
    T[t] = (double *)calloc(5, sizeof(double));

  T[0][1] = 36;
  T[0][2] = 36;
  T[0][3] = 36;

  for (int t = 0; t < numSteps; t++) {
    double r = (K * DT) / pow(DX, 2);
    for (int i = 1; i < 4; i++) {
      T[t + 1][i] = T[t][i];
      T[t + 1][i] += r * (T[t][i - 1] - 2 * T[t][i] + T[t][i + 1]);
    }
  }

  cout << fixed << setprecision(5);
  for (int t = 0; t < numSteps; t++) {
    for (int i = 0; i < 5; i++)
      cout << T[t][i] << ' ';
    cout << '\n';
  }

  cout << fixed << setprecision(2);
  cout << "\nFinal Temperature after " << TIME_MAX << "t\n";
  for (int i = 0; i < 5; i++)
    cout << T[numSteps][i] << ' ';
  cout << '\n';

  return 0;
}
