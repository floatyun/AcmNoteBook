#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
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

void prepare(ll p, vector<ll>&fac, vector<ll>&inv_fac) {
    fac.resize(p); inv_fac.resize(p);
    mod_sys mod;
    mod.set_mod(p);
    fac[0] = 1;
    inv_fac[0] = 1;
    for (int i = 1; i < p; ++i) {
        fac[i] = (fac[i-1]*i)%p;
        inv_fac[i] = mod.pow(fac[i], p-2); // 既然能枚举一遍，p*p不应该爆ll
    }
}

// 输入预设0=<n,m<p
inline ll combination(ll n, ll m, ll p, vector<ll>&fac, vector<ll>&inv_fac) {
    if (n < m) return 0;
    return fac[n]*inv_fac[m]%p*inv_fac[n-m]%p;
}

ll lucas(ll n, ll m, ll p, vector<ll>&fac, vector<ll>&inv_fac) {
    if (n < m) return 0;
    ll ans = 1;
    while(true) {
        if (m == 0) return ans;
        if (n < p && m < p) return ans*combination(n,m,p,fac,inv_fac)%p;
        ans = ans * combination(n%p,m%p,p,fac,inv_fac)%p;
        n/=p; m/=p;
    }
}

int main()
{
    ll n,m,p;
    int t;
    vector<ll>fac,inv_fac;
    scanf("%d",&t);
    while (t--) {
        scanf("%lld%lld%lld",&n,&m,&p);
        prepare(p,fac,inv_fac);
        n += m;
        printf("%lld\n",lucas(n,m,p,fac,inv_fac));
    }
}