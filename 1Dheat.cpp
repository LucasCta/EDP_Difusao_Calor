#include <bits/stdc++.h>
using namespace std;

#define dT 1
#define K 10
#define dX 0.25
#define iterations 50

int main() {

  long double **temp =
      (long double **)malloc(sizeof(long double *) * (iterations + 1));
  for (int t = 0; t <= iterations; t++)
    temp[t] = (long double *)calloc(5, sizeof(long double));

  temp[0][1] = 36;
  temp[0][2] = 36;
  temp[0][3] = 36;

  for (int t = 0; t < iterations; t++) {
    long double r = (K * dT) / pow(dX, 2);
    for (int i = 1; i < 4; i++)
      temp[t + 1][i] = (r * temp[t][i - 1]) - ((1 + 2 * r) * temp[t][i]) +
                       (r * temp[t][i + 1]);
  }

  cout << "Matriz de Temperaturas\n";
  for (int t = 0; t <= 50; t++) {
    for (int i = 0; i < 5; i++)
      cout << temp[t][i] << ' ';
    cout << '\n';
  }

  return 0;
}
