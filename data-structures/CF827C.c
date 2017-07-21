/*
CF827C: 树状数组  复杂度: O(10*|s|log|s|)

注意数据范围的两个特点：第一，模式串e非常短；第二，字符串只包含4种字符。

做法是每次匹配的时候，对于串e中的每个字符c，查询原串s中将要与c比较的所有位置当中，与c匹配的字符总数。
于是可以维护一棵树状数组，其中每个位置记录其前缀中A,T,G,C分别出现的总次数。
并且，树状数组维护的前缀和不是连续的，而是分别记录1间隔,2间隔([当前位置]+[当前位置-2]+[当前位置-4]+...),...,10间隔的前缀和。
（因此空间复杂度是O(10*4*|s|)）

树状数组求和的时候，可以先“连续化”，即把 当前位置 转化为 “在当前的间隔下，当前位置在所有待求和的位置中排第几个（序号）”。
然后根据当前序号的lowbit求出下一个序号，再转化为下一个位置。

待思考：本题可否用线段树实现？
*/


#include <stdio.h>
#include <string.h>

int q,len,bit[100010][20][4];
char s[100010];
int trans(char ch);
void update(int pos,int d,int c,int val);
int query(int pos,int d,int c);

int main(void)
{
    freopen("evolution.in","r",stdin);
    freopen("evolution.out","w",stdout);
    scanf("%s%d",s,&q);
    len=strlen(s);
    int i,j;
    for(i=len;i>=1;i--)
        s[i]=trans(s[i-1]);
    memset(bit,0,sizeof(bit));
    for(i=1;i<=len;i++)
        for(j=1;j<=10;j++)
            update(i,j,s[i],1);
    int opt,l,r,x;
    char str[20];
    while(q--)
    {
        scanf("%d",&opt);
        if(opt==1)
        {
            scanf("%d%s",&x,str);
            for(i=1;i<=10;i++)
            {
                update(x,i,s[x],-1);
                update(x,i,trans(str[0]),1);
            }
            s[x]=trans(str[0]);
        }
        else
        {
            scanf("%d%d%s",&l,&r,str);
            int ans=0,le=strlen(str);
            for(i=0;i<le && l+i<=r;i++)
            {
                ans+=query(r-i,le,trans(str[(r-i-l)%le]));
                ans-=query(l+i-le,le,trans(str[i]));
            }
            printf("%d\n",ans);
        }
    }
    return 0;
}

int trans(char ch)
{
    if(ch=='A') return 0;
    else if(ch=='T') return 1;
    else if(ch=='G') return 2;
    else return 3;
}

void update(int pos,int d,int c,int val)
{
    int p=(pos-1)/d+1,os=(pos-1)%d+1,i;
    for(i=p;i<=len;i+=i&(-i))
        if((i-1)*d+os<=len)
            bit[(i-1)*d+os][d][c]+=val;
}

int query(int pos,int d,int c)
{
    if(pos<=0) return 0;
    int p=(pos-1)/d+1,os=(pos-1)%d+1,s=0,i;
    for(i=p;i;i-=i&(-i))
        if((i-1)*d+os<=len)
            s+=bit[(i-1)*d+os][d][c];
    return s;
}
