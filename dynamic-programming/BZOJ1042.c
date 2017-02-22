/*
BZOJ1042: DP+容斥原理

如果每种硬币的数量不加限制，是非常容易求解的（完全背包问题）。
设无限制时拼出s元的方案数是f[s]。当硬币数量有限制后，根据容斥原理，也可利用f数组求出答案。
设g(X)是集合X中硬币数量均超限，集合X以外硬币无限制时的方案数，则 总方案数=g({})-g({1})-g({2})-...-g({4})+g({1,2})+...-g({1,2,3})-...+g({1,2,3,4})。
而g(X)可以看作先对X中每种硬币ci各拿di+1枚，再在4种硬币中任意拿的方案数，即g(X)=f[s-sum(ci*(di+1))]。

参考：http://hzwer.com/5286.html
*/


#include <stdio.h>
#include <string.h>

typedef long long LL;
int tot,cnt[20];
LL c[4],d[4],dp[100010][4];

int main(void)
{
    freopen("coin.in","r",stdin);
    freopen("coin.out","w",stdout);
    int i,j;
    for(i=0;i<4;i++)
        scanf("%lld",&c[i]);
    memset(dp,0,sizeof(dp));
    for(i=0;i<4;i++) dp[0][i]=1;
    for(i=1;i<=100000;i++)
        for(j=0;j<4;j++)
        {
            if(j) dp[i][j]+=dp[i][j-1];
            if(i>=c[j]) dp[i][j]+=dp[i-c[j]][j];
        }
    for(i=0;i<1<<4;i++)
        for(cnt[i]=0,j=0;j<4;j++)
            if(i&(1<<j)) cnt[i]++;
    LL ans,s,t;
    scanf("%d",&tot);
    while(tot--)
    {
        for(i=0;i<4;i++)
            scanf("%lld",&d[i]);
        scanf("%lld",&s);
        for(ans=0,i=0;i<1<<4;i++)
        {
            for(t=s,j=0;j<4;j++)
            {
                if(i&(1<<j)) t-=c[j]*(d[j]+1);
                if(t<0) break;
            }
            if(t>=0)
                ans += dp[t][3]*(cnt[i]%2 ? -1 : 1);
        }
        printf("%lld\n",ans);
    }
    return 0;
}
