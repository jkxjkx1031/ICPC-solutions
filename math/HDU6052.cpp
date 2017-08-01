/*
HDU6052: 分类讨论+容斥原理+单调栈  复杂度O(n^4/10+n^2*(2^10))

统计每种颜色在多少个矩形中出现过。
对于出现次数少于10次的颜色，用容斥原理统计矩形数量；对于出现次数多于10次的颜色，用扫描+单调栈实现（可以乱搞一下，使复杂度在n^2以内即可）。

有关STL的实现细节：多组输入数据的情况下，vector使用前记得clear！
*/


#include <stdio.h>
#include <string.h>
#include <vector>
#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))
#define S0 10

using namespace std;
typedef long long LL;
int T,n,m,g[110][110],len[110][110],stk[110];
vector<int> x[10010],y[10010];
LL solve(int col);
LL solve2(int col);

int main(void)
{
    freopen("bf.in","r",stdin);
    freopen("bf.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d",&n,&m);
        int i,j;
        for(i=0;i<n*m;i++)
        {
            x[i].clear();  y[i].clear();
        }
        for(i=1;i<=n;i++)
            for(j=1;j<=m;j++)
            {
                scanf("%d",&g[i][j]);
                x[g[i][j]].push_back(i);
                y[g[i][j]].push_back(j);
            }
        LL ans=0;
        for(i=0;i<n*m;i++)
            if(x[i].size()<=S0) ans+=solve(i);
            else ans+=solve2(i);
        printf("%.9f\n",(double)ans/(n*(n+1)/2)/(m*(m+1)/2));
    }
    return 0;
}

LL solve(int col)
{
    int i,j,sig,u,d,l,r,s=x[col].size();
    LL ret=0;
    for(i=1;i<1<<s;i++)
    {
        for(sig=-1,u=n,d=1,l=m,r=1,j=0;j<s;j++)
            if(i&(1<<j))
            {
                sig*=-1;
                u=MIN(u,x[col][j]);  d=MAX(d,x[col][j]);
                l=MIN(l,y[col][j]);  r=MAX(r,y[col][j]);
            }
        ret+=sig*u*(n-d+1)*l*(m-r+1);
    }
    return ret;
}

LL solve2(int col)
{
    int i,j,last;
    for(i=1;i<=n;i++)
        for(last=m+1,j=m;j>=1;j--)
            if(g[i][j]==col)
            {
                len[i][j]=0;
                last=j;
            }
            else len[i][j]=last-j;
    LL ret=(n*(n+1)/2)*(m*(m+1)/2),sum;
    int top;
    for(j=1;j<=m;j++)
    {
        stk[top=1]=0;
        len[0][j]=-1;
        for(sum=0,i=1;i<=n;i++)
        {
            while(len[stk[top]][j]>=len[i][j])
            {
                sum-=len[stk[top]][j]*(stk[top]-stk[top-1]);
                top--;
            }
            sum+=len[i][j]*(i-stk[top]);
            stk[++top]=i;
            ret-=sum;
        }
    }
    return ret;
}
