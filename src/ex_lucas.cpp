// luogu p4720
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 求解不定方程ax+by=(a,b)的一组特解并返回a,b最大公约数
// x,y存储返回的一组特解。
ll ex_gcd(ll a, ll b, ll &x, ll &y) {
	if (b) {
		auto d = ex_gcd(b, a%b, y, x); // 注意x和y位置互换了。
		// x是后，无需赋值，y是 前-a/b*后 即 y -= a/b*x
		y -= a/b*x;
		return d;
	} else {
		x = 1; y = 0;
		return a;
	}
}

// 求解不定方程ax+by=c.
// 返回值表示是否有解
// d存储是(a,b)
// 当有解的情况下
// x,y存储一组特解,并且确保x是最小的非负整数。
// 通解是X=x+(b/d)*t,Y=y-(a/d)*t t是整数。
bool binary_linear_indefinite_equation(ll a, ll b, ll c, ll &x, ll &y, ll &d) {
	d = ex_gcd(a, b, x, y); // solve: ax+by=(a,b)
	if (c%d) return false;
	x *= c/d;
	y *= c/d;
	auto k = b/d;
	x = (x%k+k)%k; // 调为最小非负整数
	y = (d-a*x)/b;
	return true;
}

// 线性同余方程 Linear congruence equation
// ax = c (mod m) <===> ax+my=c
// x存储最小非负解，通解X=x+kt t为整数
// 有解的情况下，最小非负解x肯定在[0,m)范围内
bool linear_congruence_equation(ll a, ll c, ll m, ll &x, ll &k) {
	ll y, d;
	auto ans = binary_linear_indefinite_equation(a, m, c, x, y, d);
	k = m/d;
	return ans;
}

// 求a在Zm<+,*>中的乘法逆元x
// 返回逆元是否存在,x存储逆元
// ax = 1 (mod m)
bool multiplicative_inverse(ll a, ll m, ll &x) {
	ll k;
	return linear_congruence_equation(a, 1, m, x, k);
	// assert(k == m);
}

// 线性同余方程组 Linear congruence equations
// a_ix = c_i (mod m_i) 共n个
// 可能存在的问题，由于迭代过程中k一直在求最小公倍数，所以可能会爆long long，这个，最佳的方法是直接暴力把ll的定义改为__int128
// 但是要注意__int128的输入输出
// 如果还是爆，我没法子了
bool linear_congruence_equations(int n, ll a[], ll c[], ll m[], ll &x, ll &k) {
	ll x_i, k_i, t, t_i, d;
	x = 0; k = 1;
	for (int i = 0; i < n; ++i) {
		if (!linear_congruence_equation(a[i], c[i], m[i], x_i, k_i))
			return false; 
		// kt+x
		// k_it_i+x_i
		if (!binary_linear_indefinite_equation(
			k, k_i, x_i-x, t, t_i, d))
			return false;
		x += k*t;
		k *= k_i/d;
	}
	return true;
}

ll pow(ll a, ll n)
{   
	if (n == 0) return 1;
	// 始终维持要求的数可以表示为(a)^n*t
	ll t = 1;
	while (n > 1)
	{
		if (n&1) t = t*a;
		n >>= 1; a = a*a;
	}
	return a*t; // now n = 1
}

// 极端情况下i*i会爆ll，要改成n开根号（效率低）
// 质因数分解，p_i^{k_i} 共q项 返回q
int factor(ll n, vector<ll>&p, vector<int>&k) {
	p.clear(); k.clear();
	if (n <= 1) return 0;
	int q = 0;
	for (ll i = 2; i*i <= n; ++i) {
		if (!(n%i)) {
			p.push_back(i);
			k.push_back(0);
			do {n /= i; ++k[q];} while (!(n%i));
			++q;
		}
	}
	if (n > 1) {
		p.push_back(n);
		k.push_back(1);
		++q;
	}
	return q;
}

// 求C(n,m)%(p^k)
const ll kMaxPk = 1000000;
// f[i]表示1..i中不是p的倍数的数的乘积(%pk) inv_f则是相应的逆元
ll f[kMaxPk],inv_f[kMaxPk];
ll ex_lucas(ll n, ll m, ll p, ll k, ll pk) {
	ll k1,k2,k3,r1,r2,r3,u1,u2,u3,v1,v2,v3;
	ll ans = 1;
	f[0] = 1; inv_f[0] = 1;
	for (ll j = 1; j < pk; ++j) {
		if (j%p) {
			f[j] = (f[j-1]*j)%pk;
			multiplicative_inverse(f[j],pk,inv_f[j]); // 肯定存在逆元
		} else {
			f[j] = f[j-1];
			inv_f[j] = inv_f[j-1];
		}
	}
	while(1) {
		if (m == 0) return ans;
		k1 = n/p, r1 = n%p;
		k2 = m/p, r2 = m%p;
		k3 = (n-m)/p, r3 = (n-m)%p;
		u1 = n/pk, v1 = n%pk;
		u2 = m/pk, v2 = m%pk;
		u3 = (n-m)/pk, v3 = (n-m)%pk;
		if (k1-k2-k3) { // == 1
			ans = (ans*p)%pk;
		} // else == 0
		ans = (ans*f[v1])%pk;
		ans = (ans*inv_f[v2])%pk;
		ans = (ans*inv_f[v3])%pk;
		if (u1-u2-u3) { // == 1
			ans = (ans*f[pk-1])%pk;
		} // else == 0
		if (r1 < r2) ans = ans*((k1-k2)%pk)%pk;
		n = k1; m = k2;
	}
}

const int kMaxQ = 30;
ll a[kMaxQ];
ll c[kMaxQ];
ll mm[kMaxQ];

// 返回C(n,m)%N N的分解因式后最大的x=p^k
// 可以开x大小的数组
ll ex_lucas_N(ll n, ll m, ll N) {
	vector<ll>p;
	vector<int>k;
	int q = factor(N,p,k);
	for (int i = 0; i < q; ++i) {
		a[i] = 1;
		mm[i] = pow(p[i],k[i]);
		c[i] = ex_lucas(n,m, p[i], k[i], mm[i]);
	}
	ll ans,kk;
	linear_congruence_equations(q,a,c,mm,ans,kk);
	return ans;
}

int main()
{
	ll n,m,N;
	scanf("%lld%lld%lld",&n,&m,&N);
	ll ans = ex_lucas_N(n,m,N);
	printf("%lld\n",ans);
	return 0;
}