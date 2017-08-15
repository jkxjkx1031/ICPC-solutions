/*
Aizu2164: burnside引理+dp

本题与burnside模板题唯一的区别在于，限制了最多连续k个位置可以填入相同字符。
先不考虑环状结构，用dp算出直线型的情况下排列的方案数。
然后减去首位相连后会产生超过“k连”的方案数。方法是再次dp，算出首尾均为某字符（如'A'）的方案数。

值得注意的是，环上全为A或全为B是两种非常特殊的情况。处理方法是在之前的dp和求和过程中，一律不考虑这两种情况，最后再判断是否接受这两种方案。
*/


#include <stdio.h>
#include <string.h>
#define M 1000003

typedef long long LL;
int n,k,dp[1010][1010][2],f[1010],g[1010],cp,cd,prime[30],e[30],div[1000];
void search(int i,int prod);
int phi(int x);
LL power(int n,int e);

int main(void)
{
    freopen("revenge.in","r",stdin);
    freopen("revenge.out","w",stdout);
    while(~scanf("%d%d",&n,&k))
    {
        if(!n) break;
        int i,j;
        for(i=n;i>=0;i--)
            for(j=0;j<=k;j++)
                if(i==n) dp[i][j][0]=dp[i][j][1]=1;
                else if(j==k)
                {
                    dp[i][j][0]=dp[i+1][1][1];
                    dp[i][j][1]=dp[i+1][1][0];
                }
                else
                {
                    dp[i][j][0]=(dp[i+1][j+1][0]+dp[i+1][1][1])%M;
                    dp[i][j][1]=(dp[i+1][j+1][1]+dp[i+1][1][0])%M;
                }
        for(i=0;i<=n;i++)
            f[i]=dp[n-i][0][0]-(i>k ? 0 : 2);
        for(i=n;i>=0;i--)
            for(j=0;j<=k;j++)
                if(i==n) dp[i][j][0]=0,dp[i][j][1]=1;
                else if(j==k)
                {
                    dp[i][j][0]=dp[i+1][1][1];
                    dp[i][j][1]=dp[i+1][1][0];
                }
                else
                {
                    dp[i][j][0]=(dp[i+1][j+1][0]+dp[i+1][1][1])%M;
                    dp[i][j][1]=(dp[i+1][j+1][1]+dp[i+1][1][0])%M;
                }
        for(i=1;i<=n;i++)
            g[i]=dp[n-i+1][1][1];
        memset(e,0,sizeof(e));
        LL t=n;
        for(cp=0,i=2;i*i<=n;i++)
            if(t%i==0)
            {
                prime[++cp]=i;
                while(t%i==0)
                {
                    t/=i;
                    e[cp]++;
                }
            }
        if(t>1)
        {
            prime[++cp]=t;
            e[cp]=1;
        }
        cd=0;
        search(1,1);
        int ans=0,d,sig;
        for(i=1;i<=cd;i++)
        {
            d=div[i];
            t=f[d];
            for(j=k+1;j<=k*2;j++)
                if(d>j) t=(t-g[d-j]*(k*2-j+1)%M*2%M+M)%M;
            ans=(ans+t*phi(n/d)%M)%M;
        }
        ans=((LL)ans+M)*power(n,M-2)%M;
        if(k>=n) ans=(ans+2)%M;
        printf("%d\n",ans);
    }
    return 0;
}

void search(int i,int prod)
{
    if(i>cp)
    {
        div[++cd]=prod;
        return;
    }
    search(i+1,prod);
    int j,t=prod;
    for(j=1;j<=e[i];j++)
    {
        t*=prime[i];
        search(i+1,t);
    }
}

int phi(int x)
{
    int i,ret=x;
    for(i=1;i<=cp;i++)
        if(x%prime[i]==0)
            ret=ret/prime[i]*(prime[i]-1);
    return ret;
}

LL power(int n,int e)
{
    if(!e) return 1;
    LL t=power(n,e/2);
    return (e%2 ? t*t*n : t*t)%M;
}
