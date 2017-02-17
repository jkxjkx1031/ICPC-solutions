/*
BZOJ1027: 凸包+Floyd求最小环    复杂度O(N^3)

设原材料中的三维点为ai，用户需要的三维点为bi。
所选择的原材料a1~as能满足用户需要，当且仅当对于任一bi，存在系数ki1~kis，使得Sigma(kij*aj)=bi。
因为题目保证每个点的三维坐标总和等于1，所以ki1+...+kis=1。并且，只要对于前两维坐标Sigma(kij*aj)=bi成立，那么Sigma(kij*aj)=bi对于三维坐标都成立。
综上，我们可以无视题目中所给的材料的第三个参量，于是原问题可以转化为二维空间上的凸包问题。

由于这道题要求凸包在能覆盖给定的点集的前提下，凸包上的点尽可能少，所以不能用传统的单调队列的方法求凸包。
这道题提供了另一种有意思的求凸包的想法：
对于用来覆盖的点集中的两点pi,pj，如果对于待覆盖的任意一点qk，qk都在pi-pj所在直线的左侧（或右侧，可用叉积判定），或在pi-pj连线上（不能在延长线上），那么可以从元素i向元素j连一条有向边。
每一对(i,j)都处理好后，跑一遍Floyd，可以求出有向图上的最小环。最小环的大小就是答案。

注意：m=1的情况需要特判。
*/


#include <stdio.h>
#include <string.h>
#include <math.h>
#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))
#define INF 10000
#define EPS 1e-8

int n,m,g[510][510];
double x[1010],y[1010];
double cross(int v1s,int v1t,int v2s,int v2t);
double dot(int v1s,int v1t,int v2s,int v2t);

int main(void)
{
    freopen("metal.in","r",stdin);
    freopen("metal.out","w",stdout);
    scanf("%d%d",&m,&n);
    int i,j,k;
    for(i=1;i<=m+n;i++)
        scanf("%lf%lf%*lf",&x[i],&y[i]);
    if(m==1)
    {
        for(i=1;i<=n;i++)
            if(x[m+i]!=x[1]||y[m+i]!=y[1]) break;
        if(i>n) printf("1\n");
        else printf("-1\n");
        return 0;
    }
    for(i=1;i<=m;i++)
        for(j=1;j<=m;j++)
            g[i][j] = i==j ? 0 : INF;
    for(i=1;i<=m;i++)
        for(j=1;j<=m;j++)
        {
            if(i==j) continue;
            for(k=1;k<=n;k++)
                if( cross(i,j,i,m+k)<-EPS*EPS
                   || fabs(cross(i,j,i,m+k))<EPS*EPS && dot(m+k,i,m+k,j)>EPS*EPS )
                    break;
            if(k>n) g[i][j]=1;
        }
    for(k=1;k<=m;k++)
        for(i=1;i<=m;i++)
            for(j=1;j<=m;j++)
                g[i][j]=MIN(g[i][j],g[i][k]+g[k][j]);
    int ans=INF;
    for(i=1;i<m;i++)
        for(j=i+1;j<=m;j++)
            ans=MIN(ans,g[i][j]+g[j][i]);
    if(ans<INF) printf("%d\n",ans);
    else printf("-1\n");
    return 0;
}

double cross(int v1s,int v1t,int v2s,int v2t)
{
    double x1,x2,y1,y2;
    x1=x[v1t]-x[v1s];  y1=y[v1t]-y[v1s];
    x2=x[v2t]-x[v2s];  y2=y[v2t]-y[v2s];
    return x1*y2-x2*y1;
}

double dot(int v1s,int v1t,int v2s,int v2t)
{
    double x1,x2,y1,y2;
    x1=x[v1t]-x[v1s];  y1=y[v1t]-y[v1s];
    x2=x[v2t]-x[v2s];  y2=y[v2t]-y[v2s];
    return x1*x2+y1*y2;
}
