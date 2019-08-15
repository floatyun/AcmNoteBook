#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/* it's too ugly!
// 时间复杂度sqrt(n)求phi(n) n最大1e12-1e14的级别
ll phi(ll n) {
        ll a = n;
        // a = n \prod {(p-1)/p}
        

        // p = 2
        if (n&1^1) a >>= 1;
        while (n&1^1) n >>= 1;
        // the odd prime factor p
        for (ll p = 3; p*p <= n; p += 2)
                if (n%p == 0) {
                        a /= p;
                        a *= p-1;
                        while (!(n%p)) n /= p;
                }
        // the rest n is a prime
        if (n > 1) {
                a /= n;
                a *= n-1;
        }
        return a;
}
*/

// 时间复杂度sqrt(n)求phi(n) n最大1e12-1e14的级别
// more beautiful version, but slower (just a little bit)
ll phi(ll n) {
  ll a = n;
  for (ll p = 2; p * p <= n; ++p)
    if (!(n % p)) {
      do
        n /= p;
      while (!(n % p));
      a = a / p * (p - 1);
    }
  if (n > 1) a = a / n * (n - 1);  // the rest n is a prime
  return a;
}

// 计算1--n的所有phi(i) 线性时空复杂度，n应该最大是1e7级别的
void get_all_phi(int n, vector<int>& phi) {
  phi.resize(n + 1);
  vector<bool> is_prime(n + 1, true);
  vector<int> prime;
  is_prime[1] = is_prime[0] = false;
  phi[1] = 1;
  for (int i = 2; i <= n; ++i) {
    if (is_prime[i]) {
      prime.push_back(i);
      phi[i] = i - 1;
    }
    for (auto p : prime) {
      if (i * p > n) break;
      is_prime[i * p] = false;
      if (i % p) {
        // i不具有素因子p，i*p对于素因子p来讲次数=1。贡献是(p-1)
        phi[i * p] = phi[i] * (p - 1);
      } else {
        phi[i * p] = phi[i] * p;  // i具有素因子p,则i*p对于欧拉函数值来讲乘以p
        break;  // 保证每个数只被最小素因子访问到。
      }
    }
  }
}

void linear_sieve(int n, vector<int>& f) {
  f.resize(n + 1);
  vector<bool> is_prime(n + 1, true);
  vector<int> prime;
  is_prime[1] = is_prime[0] = false;
  f[1] = 1;
  for (int i = 2; i <= n; ++i) {
    if (is_prime[i]) {
      prime.push_back(i);
      // code here for i 当i为素数
    }
    for (auto p : prime) {
      if (i * p > n) break;
      is_prime[i * p] = false;
      if (i % p) {
        // code here for (i * p), 当(i * p)关于素因子p的次数大于等于2
      } else {
        // code here for (i * p), 当(i * p)关于素因子p的次数仅仅为1
        break;  // 保证每个数只被最小素因子访问到。保证线性。
      }
    }
  }
}

int main() {
  vector<int> phi_0(102);
  vector<int> phi_1;
  int n = 100;
  get_all_phi(100, phi_1);
  for (int i = 1; i <= n; ++i) {
    phi_0[i] = phi(i);
    cout << "phi(" << i << ") = " << phi_0[i] << "\t" << phi_1[i] << "\t"
         << (phi_0[i] == phi_1[i]) << endl;
  }
  return 0;
}