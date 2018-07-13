/*
HDU4003: 树形DP    复杂度O(nk^2)

多叉转二叉。注意可以给一个子树分配0个机器人，即分配1个机器人遍历该子树后回到父节点。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const LL INF=2100000000;
LL n,s,k,sz,first[10010],nxt[20010],tail[20010],len[20010],w[10010],son[10010],bro[10010],sl[10010],f[10010][20];

void addedge(LL u,LL v,LL l)
{
    sz++;
    tail[sz]=v;  len[sz]=l;
    nxt[sz]=first[u];  first[u]=sz;
}

void build(LL u)
{
    LL v,e;
    for(son[u]=0,sl[u]=0,e=first[u];e;e=nxt[e])
    {
        v=tail[e];
        if(son[v]>=0) continue;
        build(v);
        bro[v]=son[u];  son[u]=v;
        w[v]=len[e];
        sl[u]+=len[e]+sl[v];
    }
}

LL dp(LL u,LL k)
{
    if(f[u][k]>=0) return f[u][k];
    else if(!u) return f[u][k]=0;
    LL i;
    for(f[u][k]=INF,i=1;i<=k;i++)
        f[u][k]=min(f[u][k],dp(son[u],i)+dp(bro[u],k-i)+w[u]*i);
    f[u][k]=min(f[u][k],dp(bro[u],k)+(sl[u]+w[u])*2);
    return f[u][k];
}

int main()
{
    freopen("mineral.in","r",stdin);
    freopen("mineral.out","w",stdout);
    while(~scanf("%lld%lld%lld",&n,&s,&k))
    {
        memset(first,0,sizeof(first));
        LL i,u,v,l;
        for(sz=0,i=1;i<=n-1;i++)
        {
            scanf("%lld%lld%lld",&u,&v,&l);
            addedge(u,v,l);  addedge(v,u,l);
        }
        memset(son,-1,sizeof(son));
        bro[s]=0;  w[s]=0;
        build(s);
        memset(f,-1,sizeof(f));
        printf("%lld\n",dp(s,k));
    }
    return 0;
}