/*
CF825E: 拓扑排序+堆

一个很容易想到的（错误）做法是：仿照传统的拓扑排序，只是改用优先队列维护当前入度为0的点，每次取出的点是index最小的。
但这样做有一个问题。为了让一开始入度为0的点中index较小的尽快被编号，可能导致index更小但入度非0的点被推迟编号。

正确的做法其实与之类似，只是将顺序颠倒即可：用优先队列维护出度为0的点，每次取出堆中index最大的点，标最大的号。
可以用反证法证明：设最优解中出度为0的所有点中index最大的点（设为u）编号为x，而整个点集中的最大编号是n(n>x)，对应的结点是v。
易证v也是出度为0的点。因此，u的index一定比v的index大。
如果把u编号为n，而把原来编号是x+1,x+2,...,n的点分别重新编号为x,x+1,...,n-1，由于仅v（原来编号为n）的index就小于u的index，因此该调整必定是有利的。从而证明假设错误。

可以把类似的反证法应用于第一种（错误）做法上，可以找到之所以不能升序编号而只能降序编号的原因。
*/


#include <stdio.h>
#include <string.h>

int n,m,sz,first[100010],next[100010],tail[100010],deg[100010],h,hp[100010],lab[100010];
void addedge(int u,int v);
void push(int k);
int pop(void);
void up(int u);
void down(int u);
void swap(int u,int v);

int main(void)
{
    freopen("label.in","r",stdin);
    freopen("label.out","w",stdout);
    scanf("%d%d",&n,&m);
    int i,u,v;
    memset(first,0,sizeof(first));
    for(sz=0,i=1;i<=m;i++)
    {
        scanf("%d%d",&u,&v);
        addedge(v,u);
    }
    for(h=0,i=1;i<=n;i++)
        if(!deg[i]) push(i);
    int cur=n,e;
    while(h)
    {
        u=pop();
        lab[u]=cur--;
        for(e=first[u];e;e=next[e])
        {
            v=tail[e];
            deg[v]--;
            if(!deg[v]) push(v);
        }
    }
    for(i=1;i<=n;i++)
        printf("%d ",lab[i]);
    putchar('\n');
    return 0;
}

void addedge(int u,int v)
{
    tail[++sz]=v;
    next[sz]=first[u];  first[u]=sz;
    deg[v]++;
}

void push(int k)
{
    hp[++h]=k;
    up(h);
}

int pop(void)
{
    int ret=hp[1];
    swap(1,h--);
    down(1);
    return ret;
}

void up(int u)
{
    if(u>1 && hp[u]>hp[u/2])
    {
        swap(u,u/2);
        up(u/2);
    }
}

void down(int u)
{
    if(u*2>h) return;
    int v=u*2;
    if(v+1<=h && hp[v+1]>hp[v]) v++;
    if(hp[v]>hp[u])
    {
        swap(u,v);
        down(v);
    }
}

void swap(int u,int v)
{
    int t=hp[u];  hp[u]=hp[v];  hp[v]=t;
}
