/*
HDU6053: 莫比乌斯反演

利用莫比乌斯反演，先算出gcd为2的倍数,3的倍数,4的倍数,...的方案数，从而求出gcd等于2,3,4,...的方案数。

求gcd为x的倍数的方案数(f(x))时，可以利用前缀和技术，先预处理出不小于k的a[i]共有多少个(s[k])。
之后计算f(x)时，分别查询s[x],s[x*2],s[x*3],...，则f[x]=(1^(s[x]-s[x*2]))*(2^(s[x*2]-s[x*3]))*...。
上述计算f[]数组的过程复杂度为O(AlogA)。A为a[]中的最大值。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>

typedef long long LL;
const LL A=100000,M=1000000007;
LL T,n,a[100010],s[100010],erat[100010],mu[100010],f[100010],g[100010];
LL power(LL x,LL e);

int main()
{
    freopen("gcd.in","r",stdin);
    freopen("gcd.out","w",stdout);
    scanf("%lld",&T);
    LL cas;
    for(cas=1;cas<=T;cas++)
    {
        scanf("%lld",&n);
        LL i,j;
        for(i=1;i<=n;i++)
            scanf("%lld",&a[i]);
        memset(s,0,sizeof(s));
        for(i=1;i<=n;i++) s[a[i]]++;
        for(i=A-1;i>=1;i--) s[i]+=s[i+1];
        LL amin=A;
        for(i=1;i<=n;i++)
            amin=std::min(amin,a[i]);
        for(i=2;i<=amin;i++)
            for(f[i]=1,j=1;j*i<=A;j++)
                f[i]=f[i]*power(j,j*i+i<=A ? s[j*i]-s[j*i+i] : s[j*i])%M;
        memset(erat,0,sizeof(erat));
        for(i=2;i<=A;i++)
            if(!erat[i])
                for(j=i*i;j<=A;j+=i) erat[j]=1;
        for(i=1;i<=amin;i++) mu[i]=1;
        for(i=2;i<=amin;i++)
            if(!erat[i])
                for(j=1;j*i<=amin;j++)
                    if(j%i==0) mu[j*i]=0;
                    else mu[j*i]*=-1;
        for(i=2;i<=amin;i++)
            for(g[i]=0,j=1;j*i<=amin;j++)
                g[i]=(g[i]+(mu[j]+M)*f[j*i]%M)%M;
        LL ans=0;
        for(i=2;i<=amin;i++)
            ans=(ans+g[i])%M;
        printf("Case #%lld: %lld\n",cas,ans);
    }
    return 0;
}

LL power(LL x,LL e)
{
    if(!e) return 1;
    LL t=power(x,e/2);
    return (e%2 ? t*t%M*x%M : t*t%M);
}
