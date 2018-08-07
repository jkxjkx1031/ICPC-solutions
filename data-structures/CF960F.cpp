/*
CF960F: 单调set     复杂度O(mlogm)

对于每个顶点，维护一个单调set即可。
对于边(u,v)，查询顶点u，并更新顶点v。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>

using namespace std;
const int INF=int(1e6);
typedef pair<int,int> P;
int n,m;
set<P> s[100010];

bool operator<(const P &lhs,const P &rhs)
{
    if(lhs.first==rhs.first)
        return lhs.second>rhs.second;
    else return lhs.first<rhs.first;
}

int main()
{
    freopen("pathwalks.in","r",stdin);
    freopen("pathwalks.out","w",stdout);
    scanf("%d%d",&n,&m);
    int i;
    for(i=1;i<=n;i++)
        s[i].insert(P(-1,0));
    int u,v,w,t,ans=0;
    while(m--)
    {
        scanf("%d%d%d",&u,&v,&w);
        auto p=--s[u].lower_bound(P(w,INF));
        t=p->second+1;
        ans=max(ans,t);
        auto q=s[v].upper_bound(P(w,t)),qq=--s[v].upper_bound(P(w,t));
        if(t<=qq->second) continue;
        while(q!=s[v].end())
            if(q->second<=t)
            {
                s[v].erase(q);
                q=++qq;  --qq;
            }
            else break;
        s[v].insert(q,P(w,t));
    }
    printf("%d\n",ans);
    return 0;
}