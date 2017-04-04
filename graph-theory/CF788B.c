/*
CF788B: 欧拉回路+组合

结论是：不用管走过两次的边，只统计两条走过一次的边的组合方式即可。

如果一个子图中每条边都走过两次，可以把每条边都拆分成两条走过一次的边，因此，这个子图中每个顶点的度都必定为偶数。
于是，根据欧拉回路的性质，从一个点出发经过所有（拆分后的）边一次，最后必定回到出发的点。因此所有走过两次的边都可以等效为不存在的边。

最后答案的组成：相连的两条普通边+一条任意的普通边和一条任意的自环+两条任意的自环。

细节：需要判断图的连通性，如果不连通就直接输出0。但是如果只是几个孤立的点不连通，可以无视掉这些点。
*/


#include <stdio.h>
#include <string.h>

typedef long long LL;
int n,m,cnt[1000010],f[1000010],e[1000010];
int find(int u);

int main(void)
{
    freopen("journey.in","r",stdin);
    freopen("journey.out","w",stdout);
    scanf("%d%d",&n,&m);
    memset(cnt,0,sizeof(cnt));
    memset(f,0,sizeof(f));
    memset(e,0,sizeof(e));
    int i,u,v,loop=0;
    for(i=1;i<=m;i++)
    {
        scanf("%d%d",&u,&v);
        e[u]=e[v]=1;
        if(u!=v)
        {
            cnt[u]++;  cnt[v]++;
            if(find(u)!=find(v))
                f[find(u)]=find(v);
        }
        else loop++;
    }
    int u0=u;
    for(i=1;i<=n;i++)
        if(e[i] && find(i)!=find(u0)) break;
    if(i<=n) printf("0\n");
    else
    {
        LL ans=0;
        for(i=1;i<=n;i++)
            ans+=(LL)cnt[i]*(cnt[i]-1)/2;
        ans+=(LL)loop*(m-1)-(LL)loop*(loop-1)/2;
        printf("%I64d\n",ans);
    }
    return 0;
}

int find(int u)
{
    if(!f[u]) return u;
    else return f[u]=find(f[u]);
}
