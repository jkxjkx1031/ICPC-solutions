/*
CF780F: DP+矩阵乘法+压位优化    复杂度O(log(10^18)*n^3/64)

根据输入可以构造出两个01邻接矩阵：矩阵A0记录P型边，矩阵B0记录B型边。
设A1=A0B0,B1=B0A0,...,Ak=A_{k-1}B_{k-1},Bk=B_{k-1}A_{k-1}. 
由矩阵乘法可知，若Ak(i,j)不为零，说明从i到j有一条长度为2^k的P型路径（PBBPBPPBBPPBP...）；若Bk(i,j)不为零，说明从i到j有一条长度为2^k的B型路径。
通过log2(10^18)次矩阵乘法，我们可以得知任意两点间存不存在长度为2^k的P/B型路径。

定义f[k][u],g[k][u]，分别表示以u为起点，最长且长度不超过2^k的P,B型路径的长度。
有转移方程：f[k][u] = max(v=1...n) { 2^(k-1)+g[k-1][v] (if A_{k-1}(u,v)=true) ; f[k-1][u] (if A_{k-1}(u,v)=false) }。求g同理。

前面两步中，求f和g的复杂度为O(log(10^18)*n^2)，但是矩阵乘法的复杂度为O(log(10^18)*n^3)，必定会超时。
注意到，矩阵Ak,Bk均为01矩阵，因此相乘的时候可以把若干行/列合并起来。
假设我们需要计算X*Y，其中X,Y都是n*n的01矩阵。如果把相邻的60位合并成一个long long int，可以把X压缩成n*9的矩阵，而把Y压缩成9*n的矩阵。这样矩阵乘法的复杂度就变为了原来的1/60。
*/


#include <stdio.h>
#include <string.h>
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))
#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))

typedef long long LL;
int n,m,map[2][70][510][510];
LL f[70][510],g[70][510],auxx[510][9],auxy[9][510];
void multiply(int x[][510],int y[][510],int z[][510]);

int main(void)
{
    freopen("bitland.in","r",stdin);
    freopen("bitland.out","w",stdout);
    scanf("%d%d",&n,&m);
    memset(map,0,sizeof(map));
    int i,j,k,u,v,t;
    for(i=1;i<=m;i++)
    {
        scanf("%d%d%d",&u,&v,&t);
        if(!t) map[0][0][u][v]=1;
        else map[1][0][u][v]=1;
    }
    for(i=1;i<=60;i++)
    {
        multiply(map[0][i-1],map[1][i-1],map[0][i]);
        multiply(map[1][i-1],map[0][i-1],map[1][i]);
    }
    for(i=1;i<=n;i++)
    {
        for(f[0][i]=0,j=1;j<=n;j++)
            f[0][i]|=map[0][0][i][j];
        for(g[0][i]=0,j=1;j<=n;j++)
            g[0][i]|=map[1][0][i][j];
    }
    for(i=1;i<=60;i++)
        for(j=1;j<=n;j++)
        {
            for(f[i][j]=f[i-1][j],k=1;k<=n;k++)
                if(map[0][i-1][j][k])
                    f[i][j] = MAX( f[i][j] , (1ll<<(i-1))+g[i-1][k] );
            for(g[i][j]=g[i-1][j],k=1;k<=n;k++)
                if(map[1][i-1][j][k])
                    g[i][j] = MAX( g[i][j] , (1ll<<(i-1))+f[i-1][k] );
        }
    if(f[60][1]>1000000000000000000)
        printf("-1\n");
    else printf("%I64d\n",f[60][1]);
    return 0;
}

void multiply(int x[][510],int y[][510],int z[][510])
{
    int i,j,k;
    for(i=1;i<=n;i++)
        for(j=0;j<9;j++)
        {
            auxx[i][j]=0;
            for( k=1 ; k<=60 && j*60+k<=n ; k++ )
                auxx[i][j] = auxx[i][j]*2 + x[i][j*60+k];
        }
    for(j=1;j<=n;j++)
        for(i=0;i<9;i++)
        {
            auxy[i][j]=0;
            for( k=1 ; k<=60 && i*60+k<=n ; k++ )
                auxy[i][j] = auxy[i][j]*2 + y[i*60+k][j];
        }
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            for(z[i][j]=0,k=0;k<9;k++)
                z[i][j] = (auxx[i][k]&auxy[k][j]) || z[i][j];
}
