/*
CF785D: 组合数学

利用结论：对于输入中"("全部在")"左侧的情况，假设有x个"("和y个")"，则 满足题意的子序列 与 x个1和y个0组成的01序列 一一对应。（证明见CF官方题解）
先预处理出1~n的阶乘，以及阶乘对应的逆，然后扫一遍就能求出答案。
*/


#include <stdio.h>
#include <string.h>
#define M 1000000007

typedef long long LL;
int n,a[200010];
LL fact[200010],inv[200010];
char str[200010];
LL power(int x,int p);
LL comb(int n,int k);

int main(void)
{
    freopen("school.in","r",stdin);
    freopen("school.out","w",stdout);
    gets(str);
    n=strlen(str);
    int i,s0,s1=0;
    for(i=1;i<=n;i++)
    {
        a[i] = str[i-1]=='(' ? 0 : 1;
        s1+=a[i];
    }
    s0=n-s1;
    for(fact[0]=1,i=1;i<=n;i++)
        fact[i]=fact[i-1]*i%M;
    for(i=0;i<=n;i++)
        inv[i]=power(fact[i],M-2);
    int t0=0,t1=0;
    LL ans=0;
    for(i=1;i<=n;i++)
    {
        if(a[i]) t1++;
        else t0++;
        if(!a[i])
            ans = ( ans + comb(t0+s1-t1-1,t0) )%M;
    }
    printf("%I64d\n",ans);
    return 0;
}

LL power(int x,int p)
{
    if(!p) return 1;
    LL ans=power(x,p/2);
    ans=ans*ans%M;
    if(p%2) ans=ans*x%M;
    return ans;
}

LL comb(int n,int k)
{
    if(n<k) return 0;
    return fact[n]*inv[k]%M*inv[n-k]%M;
}
