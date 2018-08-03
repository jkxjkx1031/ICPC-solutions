/*
HDU6326: 贪心+并查集+set

首先不考虑树上的限制，即对于任意一种打怪顺序，哪一种最优？
结论是a<b的排在a>b的之前，对于a<b的，按a升序排序；对于a>b的，按b降序排序。可以用局部调整法证明。

加上在树上的限制后，原本排在第一位的可能不能排在第一位了（如果它不是树根），但是访问它的父亲之后必定会立刻访问它。
因此可以将它和它的父亲合并。注意合并后的a和b需要修改，且不是被合并的两个点的相加。
合并关系可以用并查集维护，取第一位和它的父亲的操作可以用set实现。

需要注意自定义set的比较方法时，必须保证该比较关系是严格偏序，即不能存在相等关系，否则set会把本质不同的两者合并。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>

using namespace std;
typedef long long LL;
struct M
{
    LL a,b,k;
    bool operator<(const M &rhs) const
    {
        if(a<b && rhs.a<rhs.b)
            return a==rhs.a ? k<rhs.k : a<rhs.a;
        else if(a<b) return 1;
        else if(a>=b && rhs.a>=rhs.b)
            return b==rhs.b ? k<rhs.k : b>rhs.b;
        else return 0;
    }
};
LL T,n,a[100010],b[100010],sz,first[100010],nxt[200010],tail[200010],par[100010],f[100010];
set<M> s;

void addedge(LL u,LL v)
{
    tail[++sz]=v;
    nxt[sz]=first[u];  first[u]=sz;
}

void dfs(LL u)
{
    if(!par[u]) par[u]=-1;
    LL v,e;
    for(e=first[u];e;e=nxt[e])
    {
        v=tail[e];
        if(par[v]) continue;
        par[v]=u;
        dfs(v);
    }
}

LL find(LL u)
{
    return (f[u]==u ? u : (f[u]=find(f[u])));
}

int main()
{
    freopen("h.in","r",stdin);
    freopen("h.out","w",stdout);
    scanf("%lld",&T); 
    while(T--)
    {
        scanf("%lld",&n);
        LL i;
        for(a[1]=b[1]=0,i=2;i<=n;i++)
            scanf("%lld%lld",&a[i],&b[i]);
        memset(first,0,sizeof(first));
        LL u,v;
        for(sz=0,i=1;i<=n-1;i++)
        {
            scanf("%lld%lld",&u,&v);
            addedge(u,v);  addedge(v,u);
        }
        memset(par,0,sizeof(par));
        dfs(1);
        s.clear();
        for(i=1;i<=n;i++)
            s.insert(M{a[i],b[i],i});
        for(i=1;i<=n;i++) f[i]=i;
        LL t,p;
        while(s.size()>1)
        {
            t=s.begin()->k;
            if(t==1) t=(++s.begin())->k;
            p=find(par[t]);
            s.erase(M{a[t],b[t],t});
            s.erase(M{a[p],b[p],p});
            f[t]=p;
            if(a[p]-b[p]+a[t]>a[p])
                a[p]+=a[t]-b[p],b[p]=b[t];
            else b[p]+=b[t]-a[t];
            s.insert(M{a[p],b[p],p});
        }
        printf("%lld\n",s.begin()->a);
    }
    return 0;
}