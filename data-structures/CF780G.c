/*
CF780G: 线段树+栈    复杂度O((n+w)logw)

这道题的难点在于需要维护的不是障碍物的状态，而是每一列正在下落的小球的状态。（虽然官方解答也给出了维护障碍物状态的借助STL_set的算法）

对于每一列维护一个栈，栈中结点存放一“捆”小球：若干个从相同高度下落的小球。记录每一“捆”小球的高度和数量。
从高到低处理障碍物。遇到一个障碍物时，遍历所有被它覆盖的列（栈），将栈中处于该障碍物控制范围内（高度<=u[i]+s[i]）的结点弹出，并统计弹出的小球的数量和。
全部弹出后，在障碍物两侧各插入一个结点，小球数量为弹出小球的数量总和。
由于是按障碍物高度从高到低处理的，因此每个栈中小球高度也是单调递减的。

由于每个障碍物都只会插入2个新结点，所以在栈中出现过的结点总数最多只有w+2n。如果可以保证每个结点只被处理常数次，就可以通过本题。
因此，我们需要快速找到“下一个被当前障碍物控制的结点”。这可以用线段树实现：用线段树维护一段连续的列中具有最小高度的结点（点修改段查询）。
因为每个节点只会被插入一次、弹出一次，插入和弹出的时间均为O(logw)，所以总复杂度是O((n+w)logw)。
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define M 1000000007
#define INF 2000000010

typedef long long LL;
int h,w,n,m,u[100010],l[100010],r[100010],s[100010],first[100010],next[300010],hei[300010],sz,minp[400010];
LL num[300010];
void build(int k,int l,int r);
void update(int pos,int k,int l,int r);
void query(int p,int q,int k,int l,int r,int *ans);
void maintain(int k);
void push(int x,int y,LL s);
void sort(int l,int r);
void swap(int i,int j);
inline int info(int k)
{
    if( k && k<=w && first[k] ) return hei[first[k]];
    else return INF;
}

int main(void)
{
    freopen("barrier.in","r",stdin);
    freopen("barrier.out","w",stdout);
    scanf("%d%d%d",&h,&w,&n);
    int i,j;
    for(i=1;i<=n;i++)
        scanf("%d%d%d%d",&u[i],&l[i],&r[i],&s[i]);
    memset(first,0,sizeof(first));
    for(sz=1;sz<w;sz*=2);
    build(1,1,sz);
    for(m=0,i=1;i<=w;i++) push(i,h+1,1);
    srand((unsigned int)time(NULL));
    sort(1,n);
    int ans;
    LL t;
    for(i=1;i<=n;i++)
    {
        t=0;
        ans=0;
        query(l[i],r[i],1,1,sz,&ans);
        while(info(ans)<=u[i]+s[i])
        {
            t = ( t + num[first[ans]] )%M;
            first[ans]=next[first[ans]];
            update(ans,1,1,sz);
            ans=0;
            query(l[i],r[i],1,1,sz,&ans);
        }
        push( l[i]>1 ? l[i]-1 : r[i]+1 , u[i] , t );
        push( r[i]<w ? r[i]+1 : l[i]-1 , u[i] , t );
    }
    LL sum=0;
    for(i=1;i<=w;i++)
        for(j=first[i];j;j=next[j])
            sum = ( sum + num[j] )%M;
    printf("%I64d\n",sum);
    return 0;
}

void build(int k,int l,int r)
{
    if(l==r)
    {
        minp[k]=l;
        return;
    }
    int m=(l+r)/2;
    build(k*2,l,m);  build(k*2+1,m+1,r);
    maintain(k);
}

void update(int pos,int k,int l,int r)
{
    if(l==r) return;
    int m=(l+r)/2;
    if(pos<=m) update(pos,k*2,l,m);
    else update(pos,k*2+1,m+1,r);
    maintain(k);
}

void query(int p,int q,int k,int l,int r,int *ans)
{
    if(p<=l&&q>=r)
    {
        if(info(minp[k])<info(*ans)) *ans=minp[k];
        return;
    }
    int m=(l+r)/2;
    if(p<=m) query(p,q,k*2,l,m,ans);
    if(q>m) query(p,q,k*2+1,m+1,r,ans);
}

void maintain(int k)
{
    minp[k] = info(minp[k*2])<info(minp[k*2+1]) ? minp[k*2] : minp[k*2+1];
}

void push(int x,int y,LL s)
{
    m++;
    hei[m]=y;  num[m]=s;
    next[m]=first[x];  first[x]=m;
    update(x,1,1,sz);
}

void sort(int l,int r)
{
    if(l>=r) return;
    int i=l,j=r-1,x=rand()%(r-l+1)+l;
    swap(x,r);
    while(i<=j)
    {
        while(i<=j&&u[i]>u[r]) i++;
        if(i<=j) swap(i,j--);
        while(i<=j&&u[j]<u[r]) j--;
        if(i<=j) swap(i++,j);
    }
    swap(i,r);
    sort(l,i-1);  sort(i+1,r);
}

void swap(int i,int j)
{
    int t;
    t=u[i];  u[i]=u[j];  u[j]=t;
    t=l[i];  l[i]=l[j];  l[j]=t;
    t=r[i];  r[i]=r[j];  r[j]=t;
    t=s[i];  s[i]=s[j];  s[j]=t;
}
