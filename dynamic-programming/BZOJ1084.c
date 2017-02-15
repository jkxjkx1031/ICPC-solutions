/*
BZOJ1084: DP    复杂度O(k*n^3)

用到了一个辅助数组g[i][j][l]，表示第l列中，在第i行到第j行的范围内(i<=j)，以第i行元素开头的最大前缀和是多少。
DP: f[n1][n2][k0]表示在第1列的前n1个数和第2列的前n2个数中，选出k0个不重叠子矩阵，最大的分值和。
如果输入的m等于2，最后结果是f[n][n][k]；如果输入的m等于1，最后结果是f[n][0][k]。
*/


#include <stdio.h>
#include <string.h>
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))
#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))
#define INF 0x7f7f7f7f

int n,m,k,a[110][3],g[110][110][3],f[110][110][20];

int main(void)
{
    freopen("matrix.in","r",stdin);
    freopen("matrix.out","w",stdout);
    scanf("%d%d%d",&n,&m,&k);
    memset(a,0,sizeof(a));
    int i,j,l;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
            scanf("%d",&a[i][j]);
    for(i=1;i<=n;i++)
        a[i][0]=a[i][1]+a[i][2];
    int s;
    for(l=0;l<=2;l++)
        for(i=n;i>=1;i--)
            for(s=0,j=i;j>=1;j--)
                if(s>0) s=g[j][i][l]=s+a[j][l];
                else s=g[j][i][l]=a[j][l];
    memset(f,0x7f,sizeof(f));
    printf("%d\n",m==2 ? dp(n,n,k) : dp(n,0,k));
    return 0;
}

int dp(int n1,int n2,int k0)
{
    if(f[n1][n2][k0]<INF) return f[n1][n2][k0];
    else if(!k0) return f[n1][n2][k0]=0;
    else if(n1+n2<k0) return f[n1][n2][k0]=-INF;
    f[n1][n2][k0]=-INF;
    int i,t=MIN(n1,n2);
    for(i=1;i<=n1;i++)
        f[n1][n2][k0]=MAX(f[n1][n2][k0],dp(n1-i,n2,k0-1)+g[n1-i+1][n1][1]);
    for(i=1;i<=n2;i++)
        f[n1][n2][k0]=MAX(f[n1][n2][k0],dp(n1,n2-i,k0-1)+g[n2-i+1][n2][2]);
    for(i=1;i<=t;i++)
        f[n1][n2][k0]=MAX(f[n1][n2][k0],dp(t-i,t-i,k0-1)+g[t-i+1][t][0]);
    return f[n1][n2][k0];
}
