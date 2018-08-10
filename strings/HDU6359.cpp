/*
HDU6359: 后缀树组+哈夫曼树+分类讨论     复杂度O(n)

题目中的“conjecture”（猜测）是一个很fancy的东西。
有意思的是，题目没有给出一个合法猜测的限制，因此不妨可以理解得大胆一点，即对于任意两种不同子串，都存在一种猜测可以将它们区分开来。

在这种理解下，题目要求的其实是对所有子串建立一棵哈夫曼树。
根据哈夫曼树的建立算法（贪心），我们需要做的是不断找到出现频率最低的两个子串，并将它们合并。

每个子串的出现次数可以用后缀数组预处理（经典做法，单调栈）。
需要注意的是不要对同一个lcp重复计数，方法是记录每个长度的lcp上一次出现的位置。
结算当前lcp之前，需要先判断该lcp是否与同一长度的前一个lcp重合了。

最后合并哈夫曼树的结点的时候，对于出现次数不超过n的子串，用数组cnt[i]记录出现次数为i的子串个数，出现次数相同的子串统一批量合并即可。
出现次数超过n的子串（合并一些结点后可能出现这些“虚”子串），个数不会超过(n+1)/2，用队列维护即可；并且可以证明每次只需合并队首的两个子串，放入队尾，也就是说优先队列是不必要的。

注意快速读入。注意memset时size不要设定为整个数组。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cassert>

typedef long long LL;
int T,n,a[2000100],sa[2000100],typ[2000100],c[2000100],p[1000010],sbuc[2000100],lbuc[2000100],name[1000010],hei[1000010],rk[1000010],l[1000010],r[1000010],stk[1000010],last[1000010];
LL q[2000010],cnt[1000010];

inline int islms(int *typ,int i)
{
    return !typ[i] && (i==1 || typ[i-1]);
}

int cmp(int *s,int *typ,int p,int q)
{
    do
    {
        if(s[p]!=s[q]) return 1;
        p++;  q++;
    }
    while(!islms(typ,p) && !islms(typ,q));
    return (!islms(typ,p) || !islms(typ,q) || s[p]!=s[q]);
}

void isort(int *s,int *sa,int *typ,int *c,int n,int m)
{
    int i;
    for(lbuc[0]=sbuc[0]=c[0],i=1;i<=m;i++)
    {
        lbuc[i]=c[i-1]+1;
        sbuc[i]=c[i];
    }
    for(i=1;i<=n;i++)
        if(sa[i]>1 && typ[sa[i]-1])
            sa[lbuc[s[sa[i]-1]]++]=sa[i]-1;
    for(i=n;i>=1;i--)
        if(sa[i]>1 && !typ[sa[i]-1])
            sa[sbuc[s[sa[i]-1]]--]=sa[i]-1;
}

void build_sa(int *s,int *sa,int *typ,int *c,int *p,int n,int m)    // the last character of s[] must be 0
{
    int i;
    for(i=0;i<=m;i++) c[i]=0;
    for(i=1;i<=n;i++) c[s[i]]++;
    for(i=1;i<=m;i++) c[i]+=c[i-1];
    typ[n]=0;
    for(i=n-1;i>=1;i--)
        if(s[i]<s[i+1]) typ[i]=0;
        else if(s[i]>s[i+1]) typ[i]=1;
        else typ[i]=typ[i+1];
    int cnt=0;
    for(i=1;i<=n;i++)
        if(!typ[i] && (i==1 || typ[i-1])) p[++cnt]=i;
    for(i=1;i<=n;i++) sa[i]=0;
    for(i=0;i<=m;i++) sbuc[i]=c[i];
    for(i=1;i<=cnt;i++)
        sa[sbuc[s[p[i]]]--]=p[i];
    isort(s,sa,typ,c,n,m);
    int last=0,t=-1,x;
    for(i=1;i<=n;i++)
    {
        x=sa[i];
        if(!typ[x] && (x==1 || typ[x-1]))
        {
            if(!last || cmp(s,typ,x,last))
                name[x]=++t;
            else name[x]=t;
            last=x;
        }
    }
    for(i=1;i<=cnt;i++)
        s[n+i]=name[p[i]];
    if(t<cnt-1) build_sa(s+n,sa+n,typ+n,c+m+1,p+cnt,cnt,t);
    else
        for(i=1;i<=cnt;i++)
            sa[n+s[n+i]+1]=i;
    for(i=0;i<=m;i++) sbuc[i]=c[i];
    for(i=1;i<=n;i++) sa[i]=0;
    for(i=cnt;i>=1;i--)
        sa[sbuc[s[p[sa[n+i]]]]--]=p[sa[n+i]];
    isort(s,sa,typ,c,n,m);
}

void build_hei()
{
    LL h=0,i,j;
    for(i=1;i<=n;i++)
        rk[sa[i]]=i;
    for(i=1;i<=n;i++)
    {
        if(rk[i]==n)
        {
            hei[n]=0;
            continue;
        }
        j=sa[rk[i]+1];
        if(h) h--;
        for(;j+h<=n && i+h<=n;h++)
            if(a[j+h]!=a[i+h]) break;
        hei[rk[i]]=h;
    }
}

LL gcd(LL a,LL b)
{
    return b ? gcd(b,a%b) : a;
}

const int BUF_SZ=100000;
char buf[BUF_SZ+10];
inline char nc(void)
{
    static char *pr=buf,*pend=buf;
    if(pr==pend)
    {
        pr=buf;
        pend=pr+fread(buf,1,BUF_SZ,stdin);
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

int main()
{
    freopen("j.in","rb",stdin);
    freopen("j.out","w",stdout);
    readint(&T);
    while(T--)
    {
        readint(&n);
        int i,j;
        for(i=1;i<=n;i++) readint(&a[i]);
        for(i=1;i<=n;i++) a[i]++;
        a[++n]=0;
        build_sa(a,sa,typ,c,p,n,110);
        build_hei();
        int t;
        for(t=0,i=1;i<=n;i++)
        {
            while(t && hei[stk[t]]>=hei[i]) t--;
            l[i]=t ? stk[t] : 0;
            stk[++t]=i;
        }
        for(t=0,i=n;i>=1;i--)
        {
            while(t && hei[stk[t]]>=hei[i]) t--;
            r[i]=t ? stk[t] : n+1;
            stk[++t]=i;
        }
        memset(cnt,0,sizeof(int)*(n+1));
        memset(last,0,sizeof(int)*(n+1));
        for(i=1;i<=n;i++)
        {
            cnt[1]+=n-sa[i]-std::max(hei[i-1],hei[i]);
            if(hei[i] && l[i]>last[hei[i]])
            {
                cnt[r[i]-l[i]]+=hei[i]-std::max(hei[l[i]],hei[r[i]]);
                last[hei[i]]=i;
            }
        }
        LL ans=0;
        int front=2,rear=1;
        for(t=0,i=1;i<=n;i++)
        {
            if(!cnt[i]) continue;
            if(t)
            {
                ans+=t+i;
                cnt[i]--;
                if(t+i<=n) cnt[t+i]++;
                else q[++rear]=t+i;
            }
            ans+=cnt[i]/2*i*2;
            if(i*2<=n) cnt[i*2]+=cnt[i]/2;
            else
                for(j=1;j<=cnt[i]/2;j++)
                    q[++rear]=i*2;
            t=cnt[i]%2 ? i : 0;
            cnt[i]=0;
        }
        if(t) q[--front]=t;
        while(rear>front)
        {
            ans+=q[front]+q[front+1];
            q[++rear]=q[front]+q[front+1];
            front+=2;
        }
        LL d=(LL)n*(n-1)/2,g;
        if(ans%d==0) printf("%lld\n",ans/d);
        else
        {
            g=gcd(ans,d);
            printf("%lld/%lld\n",ans/g,d/g);
        }
    }
    return 0;
}