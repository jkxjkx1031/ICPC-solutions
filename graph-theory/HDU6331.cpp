/*
HDU6331: 矩阵乘法+分块    复杂度O(sqrt(m)*n^3 + q*n)

如果需要计算min(a1+b1,a1+b2,...,a1+bn,a2+b1,a2+b2,...,a2+bn,...,am+bn)，计算min(a1,a2,...,am)+min(b1,b2,...,bn)即可。复杂度可以从n*m降为n+m。

利用类似的思想，本题要求走至少10000步的最小距离，可以转化为求走至少100*m(m<100)步的最小距离，和走至少n(n<100)步的最小距离。
两者分别用100次矩阵乘法求出，再利用矩阵乘法合并即可。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;
const int SZ=50,INF=0x3f3f3f3f;
struct MAT
{
    int v[SZ][SZ];
    MAT()
    {
        memset(v,0,sizeof(v));
    }
    MAT(int k)
    {
        int i;
        if(k==0) memset(v,0x3f,sizeof(v));
        else if(k==1)
        {
            memset(v,0x3f,sizeof(v));
            for(i=0;i<SZ;i++) v[i][i]=0;
        }
    }
    MAT operator*(const MAT &rhs) const
    {
        int i,j,k;
        MAT tmp(0);
        for(i=0;i<SZ;i++)
            for(j=0;j<SZ;j++)
                for(k=0;k<SZ;k++)
                    tmp.v[i][j]=min(tmp.v[i][j],v[i][k]+rhs.v[k][j]);
        return tmp;
    }
    MAT operator^(int e) const
    {
        int i;
        MAT tmp(1);
        for(i=1;i<=e;i++)
            tmp=tmp*(*this);
        return tmp;
    }
}
edge,e2,g[100],g2[210],f[100],f2[210];
int T,n,m,q;

MAT min(const MAT &p,const MAT &q)
{
    int i,j;
    MAT tmp=MAT(0);
    for(i=0;i<SZ;i++)
        for(j=0;j<SZ;j++)
            tmp.v[i][j]=min(p.v[i][j],q.v[i][j]);
    return tmp;
}

int main()
{
    freopen("m.in","r",stdin);
    freopen("m.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d",&n,&m);
        edge=MAT(0);
        int i,j,k,u,v,w;
        for(i=1;i<=m;i++)
        {
            scanf("%d%d%d",&u,&v,&w);
            edge.v[u-1][v-1]=min(edge.v[u-1][v-1],w);
        }
        e2=edge^100;
        for(g[0]=MAT(1),i=1;i<=99;i++)
            g[i]=g[i-1]*edge;
        for(g2[0]=MAT(1),i=1;i<=200;i++)
            g2[i]=g2[i-1]*e2;
        for(f[99]=g[99],i=98;i>=0;i--)
            f[i]=min(f[i+1],g[i]);
        for(f2[200]=g2[200],i=199;i>=0;i--)
            f2[i]=min(f2[i+1],g2[i]);
        scanf("%d",&q);
        int s,t,c,b,r,ans;
        while(q--)
        {
            scanf("%d%d%d",&s,&t,&c);
            s--;  t--;
            b=c/100;  r=c%100;
            ans=INF;
            for(i=0;i<SZ;i++)
                ans=min(ans,f2[b+1].v[s][i]+f[0].v[i][t]);
            for(i=0;i<SZ;i++)
                ans=min(ans,f2[b].v[s][i]+f[r].v[i][t]);
            printf("%d\n",ans<INF ? ans : -1);
        }
    }
    return 0;
}