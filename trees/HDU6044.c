/*
HDU6044: 笛卡尔树+树形dp（读入优化）  复杂度O(n)

根据条件中的不等关系，可以证明任意两个区间不会交叉。
以i为索引，区间[li,ri]的包含关系为关键字的偏序，可以构造出一棵笛卡尔树。
并且，如果题目条件不矛盾，则构造出的笛卡尔树结构是唯一的。

细节：
1.求1~n的阶乘的逆时，先求出n的阶乘的逆，然后逆序递推即可。复杂度是线性的。
2.为防止dfs栈溢出，因此改用bfs实现树形dp，按逆bfs序计算各节点值。
3.fread读入优化技术。如果从文件输入，必须要以"rb"模式打开文件。

另解：本题也可通过哈希建树((li,ri)->i)，但是不能用STL map，也不可每次对hash表memset，而必须在用完hash表后将其还原，否则很可能超时。
*/


#include <stdio.h>
#include <string.h>
#define M 1000000007
#define N 1000000

typedef long long LL;
int n,l[1000010],r[1000010],ls[1000010],rs[1000010],stk[1000010],q[1000010],idx[1000010];
LL fact[1000010],inv[1000010],f[1000010];
LL power(LL x,LL p);
#define BUF_SZ 100000
char buffer[BUF_SZ+10];
inline char nc(void)
{
    static char *pr=buffer,*pend=buffer;
    if(pr==pend)
    {
        pr=buffer;
        pend=buffer+fread(buffer,1,BUF_SZ,stdin);
        if(pr==pend) return EOF;
        else return *pr++;
    }
    return *pr++;
}
inline int readint(int *x)
{
    static char ch;
    ch=nc();
    while(ch!=EOF && (ch<'0' || ch>'9')) ch=nc();
    if(ch==EOF) return 0;
    for(*x=0;ch>='0' && ch<='9';ch=nc())
        *x=*x*10+ch-'0';
    return 1;
}

int main(void)
{
    freopen("perm.in","rb",stdin);
    freopen("perm.out","w",stdout);
    int i;
    for(fact[0]=1,i=1;i<=N;i++)
        fact[i]=fact[i-1]*i%M;
    for(inv[N]=power(fact[N],M-2),i=N-1;i>=0;i--)
        inv[i]=inv[i+1]*(i+1)%M;
    int cas=0;
    while(readint(&n))
    {
        printf("Case #%d: ",++cas);
        for(i=1;i<=n;i++) readint(&l[i]);
        for(i=1;i<=n;i++) readint(&r[i]);
        memset(ls,0,sizeof(ls));
        memset(rs,0,sizeof(rs));
        int rt=1,lv=1;
        stk[1]=1;
        for(i=2;i<=n;i++)
        {
            while(lv && (l[stk[lv]]>l[i] || r[stk[lv]]<r[i])) lv--;
            if(lv)
            {
                ls[i]=rs[stk[lv]];
                rs[stk[lv]]=i;
            }
            else
            {
                ls[i]=rt;
                rt=i;
            }
            stk[++lv]=i;
        }
        for(i=1;i<=n;i++)
            if(ls[i] && (l[ls[i]]!=l[i] || r[ls[i]]!=i-1)
               || rs[i] && (l[rs[i]]!=i+1 || r[rs[i]]!=r[i])
               || !ls[i] && l[i]!=i || !rs[i] && r[i]!=i)
                break;
        if(i<=n || l[rt]!=1 || r[rt]!=n)
        {
            printf("0\n");
            continue;
        }
        int front=1,rear=1,u,k=0;
        q[1]=rt;
        while(front<=rear)
        {
            u=q[front++];
            idx[++k]=u;
            if(ls[u]) q[++rear]=ls[u];
            if(rs[u]) q[++rear]=rs[u];
        }
        for(i=n;i>=1;i--)
        {
            u=idx[i];
            f[u]=1;
            f[u]=f[u]*(ls[u] ? f[ls[u]] : 1)%M;
            f[u]=f[u]*(rs[u] ? f[rs[u]] : 1)%M;
            f[u]=f[u]*fact[r[u]-l[u]]%M*inv[u-l[u]]%M*inv[r[u]-u]%M;
        }
        printf("%lld\n",f[rt]);
    }
    return 0;
}

LL power(LL x,LL p)
{
    if(!p) return 1;
    LL t=power(x,p/2);
    LL ret=t*t%M;
    if(p%2) ret=ret*x%M;
    return ret;
}
