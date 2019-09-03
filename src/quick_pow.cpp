#include <bits/stdc++.h>
using namespace std;
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
};

