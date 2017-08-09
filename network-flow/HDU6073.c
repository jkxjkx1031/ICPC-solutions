/*
HDU6073: 二分图匹配+dfs

一个重要结论：度为1的点的匹配情况可以直接确定，删去这些点后每个结点度均为2。如果一个连通图中每个点度均为2，这个图只存在两种可能的完美匹配。
利用这个结论，先dfs删点删边，再dfs找出两种完美匹配即可。
注意图可能不是联通的，即可能由多个独立连通块组成。
*/


#include <stdio.h>
#include <string.h>
#define M 998244353

typedef long long LL;
int T,n,sz,first[600010],next[1200010],tail[1200010],f[600010],deg[600010],link[600010][2],vis[600010];
LL len[1200010],prod[2],ans;
void addedge(int u,int v,LL l);
void cut(int u);
void dfs(int u);

int main(void)
{
    freopen("match.in","r",stdin);
    freopen("match.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d",&n);
        memset(first,0,sizeof(first));
        memset(deg,0,sizeof(deg));
        int i,j,v;
        LL w;
        for(sz=0,i=1;i<=n;i++)
            for(j=1;j<=2;j++)
            {
                scanf("%d%lld",&v,&w);
                addedge(i,n+v,w);  addedge(n+v,i,w);
            }
        memset(f,0,sizeof(f));
        for(ans=1,i=n+1;i<=n*2;i++)
            if(!f[i] && deg[i]==1) cut(i);
        memset(link,0,sizeof(link));
        int e;
        for(i=1;i<=n*2;i++)
        {
            if(f[i]) continue;
            for(e=first[i];e;e=next[e])
            {
                v=tail[e];
                if(f[v]) continue;
                if(!link[i][0]) link[i][0]=e;
                else link[i][1]=e;
            }
        }
        memset(vis,0,sizeof(vis));
        for(i=1;i<=n;i++)
            if(!f[i] && !vis[i])
            {
                prod[0]=prod[1]=1;
                dfs(i);
                ans=ans*(prod[0]+prod[1])%M;
            }
        printf("%lld\n",ans);
    }
    return 0;
}

void addedge(int u,int v,LL l)
{
    sz++;
    tail[sz]=v;  len[sz]=l;
    next[sz]=first[u];  first[u]=sz;
    deg[u]++;
}

void cut(int u)
{
    f[u]=1;
    int t,v,e;
    for(e=first[u];e;e=next[e])
        if(!f[tail[e]])
        {
            ans=ans*len[e]%M;
            t=tail[e];
            break;
        }
    f[t]=1;
    for(e=first[t];e;e=next[e])
    {
        v=tail[e];
        if(f[v]) continue;
        deg[v]--;
        if(deg[v]==1) cut(v);
    }
}

void dfs(int u)
{
    vis[u]=1;
    int v;
    if(!vis[tail[link[u][0]]])
    {
        prod[0]=prod[0]*len[link[u][0]]%M;
        prod[1]=prod[1]*len[link[u][1]]%M;
        v=tail[link[u][0]];
    }
    else
    {
        prod[0]=prod[0]*len[link[u][1]]%M;
        prod[1]=prod[1]*len[link[u][0]]%M;
        v=tail[link[u][1]];
    }
    vis[v]=1;
    if(!vis[tail[link[v][0]]])
        dfs(tail[link[v][0]]);
    else if(!vis[tail[link[v][1]]])
        dfs(tail[link[v][1]]);
}
