/*
HDU6138: 后缀树组/AC自动机+fail树+dfs  复杂度O(m*字符串总长)

主要介绍AC自动机+fail树+dfs的解法：
题目要求查询的是：x,y的公共子串，在同时又作为某个串的前缀的情况下，最长为多少？
注意到AC自动机的fail树的性质与查询的性质完全吻合：fail树上任意两个节点的lca代表的子串，必定是这两个结点所属字符串的公共子串，且同时为某个字符串的前缀。
于是题目转化为求fail树上两组点之间lca的最大深度。
利用性质：如果树上结点a,b,c依次被dfs遍历，那么lca(a,c)必不深于lca(a,b)。对于每个询问dfs一遍即可求得答案。
拓展：据说本题的dfs非常类似于建虚树的dfs算法？
*/


#include <cstdio>
#include <cstring>
#include <algorithm>

int T,n,m,sz,son[100010][26],par[100010],alp[100010],dep[100010],idx[100010],fail[100010],que[100010],sn[100010],br[100010],ans,lv[2],stk[2][100010];
char str[100010];
void dfs(int u,int x,int y);

int main()
{
    freopen("throne.in","r",stdin);
    freopen("throne.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d",&n);
        memset(son,0,sizeof(son));
        int i,j,u,v;
        for(sz=1,idx[1]=0,i=1;i<=n;i++)
        {
            scanf("%s",str);
            for(u=1,j=0;str[j];j++)
            {
                if(!son[u][str[j]-'a'])
                {
                    son[u][str[j]-'a']=++sz;
                    par[sz]=u;
                    alp[sz]=str[j]-'a';
                    idx[sz]=i;  dep[sz]=j+1;
                }
                u=son[u][str[j]-'a'];
            }
        }
        int front=1,rear=1;
        que[1]=1;
        while(front<=rear)
        {
            u=que[front++];
            if(u==1 || par[u]==1) fail[u]=1;
            else
            {
                for(v=fail[par[u]];v>1;v=fail[v])
                    if(son[v][alp[u]])
                    {
                        fail[u]=son[v][alp[u]];
                        break;
                    }
                if(v<=1) fail[u]=(son[1][alp[u]] ? son[1][alp[u]] : 1);
            }
            for(j=0;j<26;j++)
                if(son[u][j]) que[++rear]=son[u][j];
        }
        memset(sn,0,sizeof(sn));
        for(i=2;i<=sz;i++)
        {
            br[i]=sn[fail[i]];
            sn[fail[i]]=i;
        }
        scanf("%d",&m);
        int x,y;
        while(m--)
        {
            scanf("%d%d",&x,&y);
            ans=0;  lv[0]=lv[1]=0;
            dfs(1,x,y);
            printf("%d\n",ans);
        }
    }
    return 0;
}

void dfs(int u,int x,int y)
{
    if(idx[u]==x)
    {
        if(lv[1]) ans=std::max(ans,dep[stk[1][lv[1]]]);
        stk[0][++lv[0]]=u;
    }
    else if(idx[u]==y)
    {
        if(lv[0]) ans=std::max(ans,dep[stk[0][lv[0]]]);
        stk[1][++lv[1]]=u;
    }
    int v;
    for(v=sn[u];v;v=br[v]) dfs(v,x,y);
    if(stk[0][lv[0]]==u)
    {
        stk[0][lv[0]]=fail[u];
        if(stk[0][lv[0]]==stk[0][lv[0]-1]) lv[0]--;
    }
    if(stk[1][lv[1]]==u)
    {
        stk[1][lv[1]]=fail[u];
        if(stk[1][lv[1]]==stk[1][lv[1]-1]) lv[1]--;
    }
}
