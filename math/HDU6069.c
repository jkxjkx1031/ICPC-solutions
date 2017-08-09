/*
HDU6069: 素数筛

本题的关键是将[l,r]范围内的数全部质因数分解。
方法是使用素数筛：
1.筛出sqrt(r)范围内的质数。
2.对于每个质数p，使用素数筛的方法对[l,r]中所有p的倍数x，算出p对应的指数分别是多少(e)。
3.x^k中p的指数即为k*e。

为了优化，可以先只处理[l,r]中小于sqrt的质因数，大于sqrt的质因数（指数最多为1）留到最后再检查一遍即可。
复杂度是 [l,r]中每个数质因数分解后的指数之和（约等于(r-l)logr）。
*/


#include <stdio.h>
#include <string.h>
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))
#define R 1000000
#define M 998244353

typedef long long LL;
LL T,l,r,k,p[1000010],f[1000010],d[1000010],t[1000010];

int main(void)
{
    freopen("count.in","r",stdin);
    freopen("count.out","w",stdout);
    LL i,j,c=0;
    memset(f,0,sizeof(f));
    for(i=2;i<=R;i++)
        if(!f[i])
        {
            p[++c]=i;
            for(j=i;j*i<=R;j++)
                f[i*j]=1;
        }
    scanf("%d",&T);
    while(T--)
    {
        scanf("%lld%lld%lld",&l,&r,&k);
        for(i=0;i<=r-l;i++)
        {
            d[i]=1;
            t[i]=l+i;
        }
        LL e,n;
        for(i=1;i<=c;i++)
            for(j=MAX(p[i],(l+p[i]-1)/p[i]);j*p[i]<=r;j++)
            {
                n=j*p[i]-l;
                for(e=0;t[n]%p[i]==0;e++)
                    t[n]/=p[i];
                d[n]=d[n]*(k*e%M+1)%M;
            }
        LL ans=0;
        for(i=0;i<=r-l;i++)
            if(t[i]==1) ans=(ans+d[i])%M;
            else ans=(ans+d[i]*(k+1))%M;
        printf("%lld\n",ans);
    }
    return 0;
}
