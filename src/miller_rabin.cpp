#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
struct mod_sys{
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

// 如果只是int范围内，可以将pow_v2改为pow，mlt改为普通乘法
bool miller_rabin(ll a, ll n, ll q, ll m, mod_sys& mod) {
    a = mod.pow_v2(a, m);
    bool is_ordinary = true;
    for (int i = 0; i < q; ++i) {
        if (a == 1) {
            return is_ordinary;
        } else {
            is_ordinary = (a == n-1);
            a = mod.mlt(a,a);
        }
    }
    return (a==1)&&(is_ordinary); // 最后一项
}

// 使用miller_rabin检测是否是素数
const int kCheckCnt = 8;
// 为了随机数
random_device rd;
mt19937_64 gen(rd());
bool miller_rabin(ll n) {
    if (n == 2) return true;
    if ((n <= 2) || (n&1^1)) return false;
    // 2^q×m表示原本输入的n-1
    ll m = n, q = 0;
    do { m >>= 1; ++q; } while(m&1^1);
    // 随机数生成，[1,n-1] 均匀分布
    uniform_int_distribution<> dis(1, n-1);
    mod_sys mod;
    mod.set_mod(n);
    for (int i = 0; i < kCheckCnt; ++i)
        if (!miller_rabin(dis(gen), n, q, m, mod))
            return false;
    return true;
}

int main() {
    int n;
    while (scanf("%d",&n)!= EOF) {
        int cnt = 0;
        ll a;
        while (n--) {
            scanf("%lld",&a);
            cnt += miller_rabin(a);
        }
        printf("%d\n",cnt);
    }
}