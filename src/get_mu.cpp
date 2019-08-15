#include <bits/stdc++.h>
using namespace std;

// 计算所有的\mu(x) x in [1..n]. 线性复杂度。
void get_all_mu(int n, vector<int>& mu) {
  mu.resize(n + 1);
  vector<bool> is_prime(n + 1, true);
  vector<int> prime;
  is_prime[1] = is_prime[0] = false;
  mu[1] = 1;
  for (int i = 2; i <= n; ++i) {
    if (is_prime[i]) {
      prime.push_back(i);
      mu[i] = -1;
    }
    for (auto p : prime) {
      if (i * p > n) break;
      is_prime[i * p] = false;
      if (i % p) {
        mu[i * p] = -mu[i];  // i不具有素因子p，具有积性
      } else {
        mu[i * p] = 0;  // i具有素因子p,则i*p具有素因子的平方的因子
        break;          // 保证每个数只被最小素因子访问到。
      }
    }
  }
}

int main() {}
