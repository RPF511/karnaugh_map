#include <stdio.h>
#include <stdlib.h>

int stat[8] = {0,1,2,4,5,7,11,15};
typedef struct node{
    unsigned int bit;
    int stat;
    int dk;
}node;

//function pow
int mypow(int b, int n,int res){
    if(n==0)return res;
    else return mypow(b,n-1,res*b);
}
int getbit(unsigned int num, int i){return (num & (0x1 << i)) >> i;}
void *ptrbinary(unsigned int num,int nov) {
    char *s;
    s = (char *)malloc((nov+1)*sizeof(char));
    int count = nov-1;
    s[nov]=0;
    for(int i = nov - 1;i>-1;num = num >> 1)s[i--] = 0b0110000+(char) (num & 1);

    printf("%s",s);
    free(s);
}

//function that makes node and put number in bit[]
void initstat(node **controller, int stat[],int stn){
    for(int i=0;i<stn;i++)controller[stat[i]]->stat = 1;
    //printf("\ninitstat done\n\n");
}
void initdk(node **controller, int dk[],int dkn){
    for(int i=0;i<dkn;i++)controller[dk[i++]]->dk = 1;
    //printf("\ninitdk done\n\n");
}

//making structure - make node with initnode and put each of them in array of node pointer
void mkmap(node **controller,int nov){
    int p = mypow(2,nov,1);
    for(int i=0;i<p;i++){
        struct node *nd = malloc(sizeof(struct node));
        nd -> bit = i;
        controller[i] = nd;
    }
}

unsigned int chplus(unsigned int ch,int *dif,int index,int nod){
    int a = 0x1<<dif[index];
    int b = ch & (0x1<<dif[index]);
    if(index == nod){
        return 0;
    }
    else if(b){
        return chplus(ch&(~a),dif,index+1,nod);
    }
    else{
        return ch + a;
    }
}
int chcheck(node *controller[],unsigned int ch,int *dif,int nod,unsigned int *cl,int index){
    if(controller[ch]->dk || controller[ch] -> stat){
        if(!chplus(ch,dif,0,nod)){
            cl[index] = ch;
            return 1;
        }
        else{
            cl[index] = ch;
            return chcheck(controller,chplus(ch,dif,0,nod),dif,nod,cl,index+1);
        }
    }
    else{
        return 0;
    }
}
int checkepi(node *controller[], int index, int nov,char *s,int *stat,int *cur,int *nost){
    unsigned int *same = (unsigned int*)malloc((nov)*sizeof(unsigned int));
    for(int i=0;i<nov;same[i++]=1);
    int nos = 0;
    int * dif = (int*)malloc(nov*sizeof(int));
    int nod = 0;
    unsigned int ch = index;
    for(int i=0;i<nov;i++){
        int a = ((index)^(0x1<<i));
        if(controller[a]->stat){
            ch = ch & controller[((index)^(0x1<<i))]->bit;
            for(int j=0;j<nov;j++){
                if((ch&(0x1<<j))^(a&(0x1<<j))){
                    same[j]=0;
                    ch&(~(0x1<<j));
                }
            }
        }
    }
    for(int j=0;j<nov;j++){
        if(getbit(ch,j)!=getbit(index,j)){
            same[j]=0;
            ch&(~(0x1<<index));
        }
    }
    for(int i=0;i<nov;i++){
        if(!same[i]){
            dif[nod]=i;
            nod++;
        }
    }
    for(int i=0;i<nov;i++){
        if(same[i])nos++;
    }
    int p2nos = (mypow(2,nod,1));
    unsigned int *cl = (unsigned int*)malloc(p2nos*sizeof(unsigned int));
    if(chcheck(controller,ch,dif,nod,cl,0)){
        for(int i=0;i<p2nos;i++){
        }
        for(int i=*nost - 1;i>-1;i--){
            for(int k=0;k<p2nos;k++){
                if(stat[i]==cl[k]){
                    for(int l=i;l<*nost-1;l++){
                        stat[l]=stat[l+1];
                    }
                    stat[*nost-1]=0;
                    *nost -= 1;
                    *cur = i;
                }
            }
        }
        int j = 0;
        int count = nov*2;
        s[count--]=0;
        for(int i=nov-1;i>-1;i--){
            if(same[i]){
                s[j++]=0b1000001+(char)(nov-1-i);
                if(!(ch&(0x1<<i))){
                    s[j++]=0b0100111;
                }
            }
        }
        s[j]=0;
        free(same);
        free(dif);
        free(cl);
        return 1;
    }
    
    else {
        free(same);
        free(dif);
        free(cl);
        return 0;
    }
}

int check(node *controller[],int *stat,int nov, int *nost,int *cur){
    char *s;
    s = (char *)malloc((nov*2+1)*sizeof(char));
    if(*nost<=0){printf("\n");free(s);return 0;}/*
    else if(*cur>=*nost){
        *cur = 0;
        return check(controller,stat,nov,nost,cur);
    }*/
    else if(checkepi(controller,stat[*cur],nov,s,stat,cur,nost)){
        printf("%s",s);
        if(*nost<1){
            *cur = 0;
            //*nost -= 1;
            printf("\n");
            return check(controller,stat,nov,nost,cur);

        }
        else if(*cur >= *nost - 1){
            *cur = 0;
            printf(" + ");
            return check(controller,stat,nov,nost,cur);
        }
        else{
            printf(" + ");
            return check(controller,stat,nov,nost,cur);
        }
    }
    else{
        *cur = *cur + 1;
        if(*cur>*nost){
            for(int i=nov-1;i>-1;i--){
                if(controller[stat[*cur]]->stat==controller[((stat[*cur])^(0x1<<i))]->stat){
                    controller[((stat[*cur])^(0x1<<i))]->stat=0;
                    controller[((stat[*cur])^(0x1<<i))]->dk=1;
                    *cur=0;
                
                }
            }
        }
        
        return check(controller,stat,nov,nost,cur);
    }
}

int main(void){
    int nov;
    printf("input the number of variables : ");
    scanf("%d",&nov);
    node *controller[32] ={NULL,};
    mkmap(controller,nov);
    int nost;
    printf("input the number of true cases : ");
    scanf("%d",&nost);
    int *stat = (int *)malloc(nost*sizeof(int));
    for(int i=0;i<nost;i++){
        printf("%-2d. input the true case : ",i);
        scanf("%d",stat+i);
    }
    int nodk;
    printf("input the number of don\'t care cases : ");
    scanf("%d",&nodk);
    int *dk = (int *)malloc(nodk*sizeof(int));
    for(int i=0;i<nodk;i++){
        printf("%-2d. input the don\'t care case : ",i);
        scanf("%d",dk+i);
    }
    initstat(controller,stat,nost);
    printf("init done\n");
    initdk(controller,dk,nodk);
    printf("init dk done\n");
    int cur=0;
    check(controller,stat,nov,&nost,&cur);

    return 0;
}