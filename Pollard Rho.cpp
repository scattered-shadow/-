#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <ctime>
#define ll long long
using namespace std;
const int S = 8;//随机算法判定次数，一般8~10次就够了  

//计算ret = (a*b)%c  a, b, c < 2^63  
ll mult_mod(ll a, ll b, ll c) {
    a %= c;
    b %= c;
    ll ret = 0;
    ll tem = a;
    while (b) {
        if (b & 1) {
            ret += tem;
            if (ret > c) {
                ret -= c;
            }
        }
        tem <<= 1;
        if (tem > c) {
            tem -= c;
        }
        b >>= 1;
    }
    return ret;
}

//计算 ret = (a^n) % mod  
ll pow_mod(ll a, ll n, ll mod) {
    ll ret = 1;
    ll tem = a % mod;
    while (n) {
        if (n & 1) {
            ret = mult_mod(ret, tem, mod);
        }
        tem = mult_mod(tem, tem, mod);
        n >>= 1;
    }
    return ret;
}

// 通过 a^(n-1)=1(mod n)来判断n是不是素数  
// n-1 = x * 2^t 中间使用二次判断  
// 是合数返回true，不一定是合数返回false  
bool check(ll a, ll n, ll x, ll t) {
    ll ret = pow_mod(a, x, n);//a^x % n  
    ll last = ret;
    for (int i = 1; i <= t; ++i)//进行t次(a^x % n)^2 % n  
    {
        ret = mult_mod(ret, ret, n);
        if (ret == 1 && last != 1 && last != n - 1) {
            return true;
        }
        last = ret;
    }
    if (ret != 1) {
        return true;
    }
    return false;
}

bool Miller_Rabin(ll n) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if ((n & 1) == 0) {
        return false;
    }
    ll x = n - 1;
    ll t = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        t++;
    }

    srand(time(NULL));

    for (int i = 0; i < S; ++i) {
        ll a = rand() % (n - 1) + 1;
        if (check(a, n, x, t)) {
            return false;
        }
    }
    return true;
}

int tol;//质因数的个数，编号为0~tol-1  
ll factor[100];//质因素分解结果(刚返回时是无序的)  

ll gcd(ll a, ll b) {
    ll t;
    while (b) {
        t = a;
        a = b;
        b = t % b;
    }
    if (a >= 0) {
        return a;
    }
    return -a;
}

//找出一个因子  
ll pollard_rho(ll x, ll c) {
    ll i = 1, k = 2;
    srand(time(NULL));
    ll x0 = rand() % (x - 1) + 1;//产生随机数x0(并控制其范围在1 ~ x-1之间)  
    ll y = x0;
    while (1) {
        i++;
        x0 = (mult_mod(x0, x0, x) + c) % x;
        ll d = gcd(y - x0, x);
        if (d != 1 && d != x) {
            return d;
        }
        if (y == x0) {
            return x;
        }
        if (i == k) {
            y = x0;
            k += k;
        }
    }
}

//对n进行素因子分解，存入factor。 k设置为107左右即可  
void findfac(ll n, int k) {
    if (n == 1) {
        return;
    }
    if (Miller_Rabin(n))//是素数就把这个素因子存起来  
    {
        factor[tol++] = n;
        return;
    }
    int c = k;
    ll p = n;
    while (p >= n) {
        p = pollard_rho(p, c--);
    }
    findfac(p, k);
    findfac(n / p, k);
}

int main() {
    int T;
    ll n;
    cout << "输入执行次数：";
    cin>>T;
    while (T--) {
        cout << "输入要分解的数：";
        cin>>n;
        if (Miller_Rabin(n)) cout << "Prime" << endl;
        else {
            tol = 0;
            findfac(n, 107);
            ll ans = factor[0];
            for (int i = 1; i < tol; ++i)
                ans = min(ans, factor[i]);
            cout << ans << endl;
        }
    }
    return 0;
}