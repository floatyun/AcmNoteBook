#include <bits/stdc++.h>
typedef long long ll;

// 求解不定方程ax+by=(a,b)的一组特解并返回a,b最大公约数
// x,y存储返回的一组特解。易懂version
ll ex_gcd1(ll a, ll b, ll &x, ll &y) {
	if (b) {
		auto d = ex_gcd1(b, a%b, x,  y);
		auto x_bac = x;
		x = y; // x设为后
		y = x_bac - a/b * y; // y设为前-a/b*后
		return d;
	} else {
		x = 1; y = 0;
		return a;
	}
}

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

int main()
{
}