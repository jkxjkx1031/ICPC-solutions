/*
BZOJ1025：置换群+LCM+DP

根据对应关系可以分解出若干个置换群。
对于每种给定的对应关系，需要的排数是 所有置换群大小的最小公倍数+1。
所以原问题就是求，把n分解成若干个整数的和，这些整数的最小公倍数有多少种可能的取值？
需要一个关键的结论：为了拼出想要的最小公倍数，应该让所有用到的数都是幂的形式（比如2^3,3^6，而不是2*3），这样它们的和最小。（可用局部调整法证明）
DP：f[n][k]表示用前k个质数的幂，它们的和不超过n，有多少种可行方案。
最后答案是f[n][s]（s是1000以内质数个数）。
*/


#include <stdio.h>
#include <string.h>

typedef long long LL;
int n,prime[200];
LL f[1010][200];

int main(void)
{
    freopen("game.in","r",stdin);
    freopen("game.out","w",stdout);
    int i,j,k,s=0,t;
    for(i=2;i<=1000;i++)
    {
        for(j=2;j<=i-1;j++)
            if(i%j==0) break;
        if(j>i-1) prime[++s]=i;
    }
    scanf("%d",&n);
    for(i=0;i<=n;i++) f[i][0]=1;
    for(i=0;i<=n;i++)
        for(j=1;j<=s;j++)
        {
            f[i][j]=f[i][j-1];
            for(t=prime[j],k=1;t<=i;t*=prime[j])
                f[i][j]+=f[i-t][j-1];
        }
    printf("%lld\n",f[n][s]);
    return 0;
}
