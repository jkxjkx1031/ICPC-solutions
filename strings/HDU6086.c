/*
HDU6086: AC自动机+dp

把每个模式串和它的的反对称串一起加入AC自动机，于是dp放置字符的时候，只需要考虑前L个字符即可（后L个字符由前L个决定）。
dp记录三维状态：当前放置完哪一位i；当前位于AC自动机上哪个结点u；当前已经包含的串的集合b。

这道题的关键在于ok数组的记录和使用。ok[u]表示以AC自动机上u为中点，向两端延伸成反对称串，最多可以包含哪些串。
记录ok数组的原因是为了解决一些模式串横跨 前半部分长为L的串 和后半串的情况。
dp到达边界时，如果发现当前已包含的串集(b)和当前匹配到的结点反向延长后可以包含的串集(ok[u])的并集为全集的话，就可以计作一种有效方案。

关于AC自动机的实现细节：
1.建立fail指针的时候一定要按照层次顺序（bfs序）！不可按结点编号顺序！编号小的结点的fail指针有可能指向编号大的结点（与KMP不同）！
2.AC自动机上的模式串很可能相互包含！因此terminal标记尽可能使用集合形式（本题中的ok同理）！
  由于term[u]可以继承term[fail[u]]，故term[]可以和fail[]一起建立（按照bfs序）。（本题中ok[]由于会访问更深层次的term[]，所以必须进行两次bfs）
*/


#include <stdio.h>
#include <string.h>
#define M 998244353

typedef long long LL;
int T,n,l,sz,son[300][2],term[300],par[300],fail[300],trans[300][2],num[300],ok[300],qu[300];
LL dp[110][300][100];
char str[100],t[100];
LL solve(int i,int u,int b);
void addnode(char *str,int k);

int main(void)
{
    freopen("string.in","r",stdin);
    freopen("string.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d",&n,&l);
        memset(son,0,sizeof(son));
        memset(term,0,sizeof(term));
        int i,j,len;
        for(sz=1,i=1;i<=n;i++)
        {
            scanf("%s",str);
            addnode(str,i);
            len=strlen(str);
            for(j=0;j<len;j++) t[j]=str[len-j-1];
            for(j=0;j<len;j++)
                str[j]=1-(t[j]-'0')+'0';
            addnode(str,i);
        }
        memset(ok,0,sizeof(ok));
        int front=1,rear=1,p,q,u,v;
        qu[1]=1;
        while(front<=rear)
        {
            u=qu[front++];
            if(u==1 || par[u]==1) fail[u]=1;
            else
            {
                for(p=fail[par[u]];p>1;p=fail[p])
                    if(son[p][num[u]])
                    {
                        fail[u]=son[p][num[u]];
                        break;
                    }
                if(p==1)
                    fail[u]=son[1][num[u]] ? son[1][num[u]] : 1;
            }
            term[u]|=term[fail[u]];
            if(son[u][0]) qu[++rear]=son[u][0];
            if(son[u][1]) qu[++rear]=son[u][1];
        }
        front=rear=1;
        qu[1]=1;
        while(front<=rear)
        {
            u=qu[front++];
            ok[u]=ok[fail[u]];
            if(term[u]) ok[u]|=term[u];
            for(p=q=u;p>1;p=par[p])
            {
                if(son[q][1-num[p]]) q=son[q][1-num[p]];
                else break;
                if(term[q]) ok[u]|=term[q];
            }
            if(son[u][0]) qu[++rear]=son[u][0];
            if(son[u][1]) qu[++rear]=son[u][1];
        }
        for(i=1;i<=sz;i++)
            for(j=0;j<=1;j++)
            {
                for(u=i;u>1;u=fail[u])
                    if(son[u][j])
                    {
                        trans[i][j]=son[u][j];
                        break;
                    }
                if(u==1)
                    trans[i][j]=son[1][j] ? son[1][j] : 1;
            }
        memset(dp,-1,sizeof(dp));
        printf("%lld\n",solve(0,1,0));
    }
    return 0;
}

LL solve(int i,int u,int b)
{
    if(dp[i][u][b]>=0) return dp[i][u][b];
    else if(i==l && (ok[u]|b)==((1<<n)-1))
        return dp[i][u][b]=1;
    else if(i==l) return dp[i][u][b]=0;
    dp[i][u][b]=0;
    if(term[u])
        dp[i][u][b]=(solve(i+1,trans[u][0],b|term[u])+solve(i+1,trans[u][1],b|term[u]))%M;
    else dp[i][u][b]=(solve(i+1,trans[u][0],b)+solve(i+1,trans[u][1],b))%M;
    return dp[i][u][b];
}

void addnode(char *str,int k)
{
    int u,j;
    for(u=1,j=0;str[j];j++)
    {
        if(!son[u][str[j]-'0'])
        {
            num[++sz]=str[j]-'0';
            son[u][str[j]-'0']=sz;
            par[sz]=u;
        }
        u=son[u][str[j]-'0'];
    }
    term[u]|=1<<(k-1);
}
