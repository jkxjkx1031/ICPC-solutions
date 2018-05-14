/*
Luogu2966: Floyd  复杂度: O(n^3 + qn)

需要了解 Floyd 算法的证明。
把点的编号按 c(i) 重新排序，每次询问时枚举需要使用前几个点作为中间点。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;
const int INF=0x3f3f3f3f;
int n,m,q,c[300],idx[300],rk[300],d[260][260][260];

int main()
{
    freopen("toll.in","r",stdin);
    freopen("toll.out","w",stdout);
    scanf("%d%d%d",&n,&m,&q);
    int i,j,k;
    for(i=1;i<=n;i++)
        scanf("%d",&c[i]);
    for(i=1;i<=n;i++) idx[i]=i;
    sort(idx+1,idx+n+1,[](int i,int j){return c[i]<c[j];});
    for(i=1;i<=n;i++)
        rk[idx[i]]=i;
    memset(d,0x3f,sizeof(d));
    int u,v,l;
    for(i=1;i<=m;i++)
    {
        scanf("%d%d%d",&u,&v,&l);
        d[0][rk[u]][rk[v]]=d[0][rk[v]][rk[u]]=min(d[0][rk[u]][rk[v]]=d[0][rk[v]][rk[u]],l);
    }
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            for(k=1;k<=n;k++)
                d[i][j][k]=min(d[i-1][j][k],d[i-1][j][i]+d[i-1][i][k]);
    int ans;
    while(q--)
    {
        scanf("%d%d",&u,&v);
        for(ans=INF,i=0;i<=n;i++)
            ans=min(ans,d[i][rk[u]][rk[v]]+max(c[idx[i]],max(c[u],c[v])));
        printf("%d\n",ans);
    }
    return 0;
}