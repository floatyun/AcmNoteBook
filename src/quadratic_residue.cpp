#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

//#define debugmode

// 为了随机数
random_device rd;
mt19937_64 gen(rd());
// uniform_int_distribution<> dis(1, n-1); //[1,n-1]均匀分布随机数 设置随机数范围
// dis(gen) 返回一个随机数


// 二次剩余模板，注意，各个函数都假设p^2即mod^2不会爆ll
// 域F_p的拓展域F_{p^2}
struct F_p2_node{ll a,b;}; // F_p2_node所有的运算都应该通过F_p2_sys来管理调用。
// 预设所有传入参数都是合法的。
struct F_p2_sys{
	ll p,w2;
	typedef F_p2_node nd;
	nd mlt(const nd&x, const nd&y) const {
		nd ans;
		//(x.a+x.bw)(y.a+y.bw)
		ans.a = (x.a*y.a%p+x.b*y.b%p*w2%p)%p;
		ans.b = (x.b*y.a%p+x.a*y.b%p)%p;
		return ans;
	}
	// pre n>=0 非递归版 不可改成引用！
	nd pow(nd a, ll n) const {
		if (n == 0) {return nd{1,0};}
        // 始终维持要求的数可以表示为(a)^n*t
        nd t{1,0};
        while (n > 1)
        {
            if (n&1) t = mlt(t,a);
            n >>= 1; a = mlt(a,a);
        }
        return mlt(t,a);
	}
};

struct mod_sys{
    typedef long long ll;
    ll mod;
    // mod_sys类初始化设置模数
    inline void set_mod(ll mod0) {mod = mod0;}
    // 返回a在[0,mod)内标准等价的数，即数学意义上的a%mod
    inline ll to_std(ll a) {return (a%mod+mod)%mod;}
    // 计算数学意义上的a*n%mod
    ll mlt(ll a, ll n) {
        a = to_std(a); n = to_std(n);
        if (0 == a || 0 == n) return 0;
        // 始终维持要求的数可以表示为n(a)+t
        ll t = 0;
        while (n > 1) {
            if (n&1) t = (t+a)%mod;
            n >>= 1; a = (a<<1)%mod;
        }
        return (a+t)%mod; // now n = 1
    }
    // 计算数学意义上的a^n%mod 输入应当a,n>=0
    ll pow(ll a, ll n)
    {   
        if (n == 0) return 1%mod;
        a = to_std(a);
        // 始终维持要求的数可以表示为(a)^n*t
        ll t = 1;
        while (n > 1)
        {
            if (n&1) t = t*a%mod;
            n >>= 1; a = a*a%mod;
        }
        return a*t%mod; // now n = 1
    }
    // 计算数学意义上的a^n%mod 输入应当a,n>=0
    // 此版本使用quick_mlt防止相乘爆ll
    ll pow_v2(ll a, ll n)
    {   
        if (n == 0) return 1%mod;
        a = to_std(a);
        // 始终维持要求的数可以表示为(a)^n*t
        ll t = 1;
        while (n > 1)
        {
            if (n&1) t = mlt(t,a);
            n >>= 1; a = mlt(a,a);
        }
        return mlt(t,a); // now n = 1
    }

	// 预设a \not\equiv 0 (mod mod) 返回a是否是二次剩余
	// 预设mod是奇素数
	// 预设p^2不会爆long long，使用pow 若爆则改用pow_v2或者__int128
	bool is_quadratic_residue(ll a) {
		return 1 == pow(a,mod-1>>1);
	}

	// 解方程x^2 = a (mod mod)  p = mod 是奇质数
	// 返回是否有解。如果有x0,x1(x0<=x1)存储解 仅解是0的时候取等号
	// 洋葱算法。
	// 预设p^2不会爆long long，使用pow 若爆则改用pow_v2或者__int128
	bool sqrt(ll a,ll& x0, ll& x1) { // 需要一个随机数生成器
		a = to_std(a);
		if (a == 0) {x0 = x1 = 0; return true;}
		if (!is_quadratic_residue(a)) return false;
		uniform_int_distribution<> dis(1, mod-1);
		ll b,w2;
		while(true) {
			b = dis(gen);
			w2 = to_std((b*b)%mod-a);
			if (!is_quadratic_residue(w2)) break;
		}
		// x = (b+w)^{(p+1)/2}
		F_p2_sys fp2{mod,w2}; // p,w2
		F_p2_node t{b,1}; // a,b(means a+bw)
		auto x = fp2.pow(t,mod+1>>1);
		x0 = to_std(x.a); // assert(x.b==0);
		x1 = mod-x.a;
		if (x0 > x1) swap(x0,x1);
		return true;
	}
};



inline ll read()
{
	ll x = 0;
	bool f = 0;
	char ch = getchar();
	while (ch < '0' || '9' < ch)
		f |= ch == '-', ch = getchar();
	while ('0' <= ch && ch <= '9')
		x = x * 10 + ch - '0', ch = getchar();
	return f ? -x : x;
}

void write(ll a)
{
	if (a < 0)
	{
		putchar('-');
		a = -a;
	}
	if (a >= 10)
	{
		write(a / 10);
	}
	putchar(a % 10 + '0');
}


int main()
{
	int t;
	ll a,p;
	ll x,y;
	mod_sys mod;
	t = (int)read(); ++t;
	ll tmp;
	while (--t) {
		a = read(); p = read();
		mod.set_mod(p);
		if (mod.sqrt(a,x,y)) {
			write(x);
			tmp = (x*x)%p;
			#ifdef debugmode
			  printf("\ncheck %lld %lld\n",x,tmp);
			#else
			#endif
			if (x != y) {
				putchar(' ');
				write(y);
			}
			putchar('\n');
		} else {
			puts("Hola!");
		}
	}
	return 0;
}