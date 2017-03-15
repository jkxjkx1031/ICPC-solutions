/*
BZOJ1069: 凸包+旋转卡壳    复杂度O(n^2)

首先，容易证明，最优方案下选取的4个点一定在凸包上。
枚举的时候，要按对角线枚举，然后分别寻找这条对角线两侧距离对角线最远的点。
由于当对角线向一个方向旋转时，两侧对应的“最远点”也向同一个方向旋转，因此可以用旋转卡壳法求出固定一个顶点时的最大四边形面积。

有一个小细节：为了方便，求凸包时可以用相同的算法正、反分别跑一遍，以分别得到上、下包络。但这样做的缺点是，凸包中相邻的点可能重合，因此在找“最远点”的时候必须考虑点重合的情况。

还有一个关于在线测评的问题：本题在BZOJ上提交时，如果调用了随机数函数，会runtime error。
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))

int n,sz,h[4010];
double x[4010],y[4010],area[4010][2];
void sort(int l,int r);
void swap(int i,int j);
double det(int i,int j,int k);

int main(void)
{
    freopen("ground.in","r",stdin);
    freopen("ground.out","w",stdout);
    scanf("%d",&n);
    int i,j,k;
    for(i=1;i<=n;i++)
        scanf("%lf%lf",&x[i],&y[i]);
    sort(1,n);
    for(sz=0,i=1;i<=n;i++)
    {
        while( sz>=2 && (y[i]-y[h[sz]])*(x[h[sz]]-x[h[sz-1]])>=(y[h[sz]]-y[h[sz-1]])*(x[i]-x[h[sz]]) ) sz--;
        h[++sz]=i;
    }
    int s=0;
    for(i=n;i>=1;i--)
    {
        while( s>=2 && (y[i]-y[h[sz]])*(x[h[sz]]-x[h[sz-1]])>=(y[h[sz]]-y[h[sz-1]])*(x[i]-x[h[sz]]) )
        {
            sz--;  s--;
        }
        h[++sz]=i;
        s++;
    }
    double ans=0;
    int p,q;
    for(i=1;i<=sz;i++)
    {
        memset(area,0,sizeof(area));
        p=(i+1)%sz+1;  q=i%sz+1;
        for(j=1;j<=sz-3;j++)
        {
            while( q%sz+1!=p && fabs(det(i,p,q))<=fabs(det(i,p,q%sz+1)) )
                q=q%sz+1;
            area[p][0]=fabs(det(i,p,q))/2;
            p=p%sz+1;
        }
        p=(i-3+sz)%sz+1;  q=(i-2+sz)%sz+1;
        for(j=1;j<=sz-3;j++)
        {
            while( (q-2+sz)%sz+1!=p && fabs(det(i,p,q))<=fabs(det(i,p,(q-2+sz)%sz+1)) )
                q=(q-2+sz)%sz+1;
            area[p][1]=fabs(det(i,p,q))/2;
            p=(p-2+sz)%sz+1;
        }
        for(j=1;j<=sz;j++)
            ans=MAX(ans,area[j][0]+area[j][1]);
    }
    printf("%.3f\n",ans);
    return 0;
}

void sort(int l,int r)
{
    if(l>=r) return;
    int i=l,j=r-1;
    while(i<=j)
    {
        while( i<=j && ( x[i]<x[r] || x[i]==x[r]&&y[i]<y[r] ) ) i++;
        if(i<=j) swap(i,j--);
        while( i<=j && ( x[j]>x[r] || x[j]==x[r]&&y[j]>y[r] ) ) j--;
        if(i<=j) swap(i++,j);
    }
    swap(i,r);
    sort(l,i-1);  sort(i+1,r);
}

void swap(int i,int j)
{
    double t;
    t=x[i];  x[i]=x[j];  x[j]=t;
    t=y[i];  y[i]=y[j];  y[j]=t;
}

double det(int i,int j,int k)
{
    return x[h[i]]*y[h[j]]+x[h[k]]*y[h[i]]+x[h[j]]*y[h[k]]-x[h[k]]*y[h[j]]-x[h[j]]*y[h[i]]-x[h[i]]*y[h[k]];
}
