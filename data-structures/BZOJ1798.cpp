/*
BZOJ1798: 线段树

一道裸的线段树题。
每个节点维护三个信息：第一类操作的参数的积(f1)、第二类操作的参数的和(f2)、区间数值总和(sum)。
由于乘法和加法操作混合，更新某个节点前必须先pushdown，并且需要规定每个节点上乘法和加法的生效顺序（我代码中规定的是先乘再加，如果先加再乘可能会涉及除法并产生浮点数）。

实现线段树的注意事项：只要作了修改就一定要maintain。节点u pushdown之后，即使u的子节点v没有再作其他修改（add或multiply），也需要maintain v，因为pushdown u的时候已经修改了v。
*/


#include <cstdio>
#include <cstring>

typedef long long LL;
LL P,f1[400010],f2[400010],sum[400010];
int n,m,a[100010],sz;
void build(int k,int l,int r);
void multiply(int p,int q,LL val,int k,int l,int r);
void add(int p,int q,LL val,int k,int l,int r);
void query(int p,int q,int k,int l,int r,LL &ans,LL s1,LL s2);
void pushdown(int k);
void maintain(int k,int l,int r);

int main(void)
{
    freopen("seq.in","r",stdin);
    freopen("seq.out","w",stdout);
    scanf("%d%lld",&n,&P);
    int i;
    for(i=1;i<=n;i++)
        scanf("%d",&a[i]);
    for(sz=1;sz<n;sz*=2);
    build(1,1,sz);
    scanf("%d",&m);
    int op,t,g;
    LL ans,c;
    for(i=1;i<=m;i++)
    {
        scanf("%d",&op);
        if(op==1)
        {
            scanf("%d%d%lld",&t,&g,&c);
            multiply(t,g,c%P,1,1,sz);
        }
        else if(op==2)
        {
            scanf("%d%d%lld",&t,&g,&c);
            add(t,g,c%P,1,1,sz);
        }
        else
        {
            scanf("%d%d",&t,&g);
            ans=0;
            query(t,g,1,1,sz,ans,1,0);
            printf("%lld\n",ans);
        }
    }
    return 0;
}

void build(int k,int l,int r)
{
    f1[k] = 1;
    f2[k] = l==r&&l<=n ? a[l]%P : 0;
    if(l<r)
    {
        int m=(l+r)/2;
        build(k*2,l,m);  build(k*2+1,m+1,r);
    }
    maintain(k,l,r);
}

void multiply(int p,int q,LL val,int k,int l,int r)
{
    if(p<=l&&q>=r)
    {
        f1[k] = f1[k]*val%P;
        if(f2[k])
            f2[k] = f2[k]*val%P;
        maintain(k,l,r);
        return;
    }
    pushdown(k);
    int m=(l+r)/2;
    if(p<=m) multiply(p,q,val,k*2,l,m);
    else maintain(k*2,l,m);
    if(q>m) multiply(p,q,val,k*2+1,m+1,r);
    else maintain(k*2+1,m+1,r);
    maintain(k,l,r);
}

void add(int p,int q,LL val,int k,int l,int r)
{
    if(p<=l&&q>=r)
    {
        f2[k] = (f2[k]+val)%P;
        maintain(k,l,r);
        return;
    }
    pushdown(k);
    int m=(l+r)/2;
    if(p<=m) add(p,q,val,k*2,l,m);
    else maintain(k*2,l,m);
    if(q>m) add(p,q,val,k*2+1,m+1,r);
    else maintain(k*2+1,m+1,r);
    maintain(k,l,r);
}

void query(int p,int q,int k,int l,int r,LL &ans,LL s1,LL s2)
{
    if(p<=l&&q>=r)
    {
        ans = ( ans + s1*sum[k] + s2*(r-l+1) ) % P;
        return;
    }
    LL t1,t2;
    t1 = s1*f1[k]%P;
    t2 = ( s2 + s1*f2[k] )%P;
    int m=(l+r)/2;
    if(p<=m) query(p,q,k*2,l,m,ans,t1,t2);
    if(q>m) query(p,q,k*2+1,m+1,r,ans,t1,t2);
}

void pushdown(int k)
{
    f1[k*2] = f1[k*2]*f1[k]%P;
    f1[k*2+1] = f1[k*2+1]*f1[k]%P;
    f2[k*2] = ( f1[k]*f2[k*2] + f2[k] )%P;
    f2[k*2+1] = ( f1[k]*f2[k*2+1] + f2[k] )%P;
    f1[k]=1;  f2[k]=0;
}

void maintain(int k,int l,int r)
{
    sum[k] = k*2<=sz*2-1 ? (sum[k*2]+sum[k*2+1])%P : 0;
    sum[k] = ( f1[k]*sum[k] + f2[k]*(r-l+1) ) % P;
}
