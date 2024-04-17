#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define error -3
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int status;
typedef int ElemType; //数据元素类型定义
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
#define MAXlength 10
typedef struct LNode{  //单链表（链式结构）结点的定义
    ElemType data;
    struct LNode *next;
}LNode,*LinkList;

LinkList L;

typedef struct{  //线性表的集合类型定义
    struct { char name[30];
        LinkList L;
    } elem[11];
    int length;
}LISTS;
LISTS Lists;      //线性表集合的定义Lists

status InitList(LinkList &L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
    if(L) return INFEASIBLE;
    L = (LinkList)malloc(sizeof(LinkList));
    L->next = NULL;
    return OK;

}

status DestroyList(LinkList &L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    LinkList p = L, q = L->next;
    while(q != NULL){
        free(p);
        p = q;
        q = q->next;
    }
    free(p);
    L = NULL;
    return OK;
}


status ClearList(LinkList &L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    if(!L->next) return ERROR;
    LinkList p = L->next, q = p->next;
    while(q != NULL){
        free(p);
        p = q;
        q = q->next;
    }
    if(p) free(p);
    L->next = NULL;
    return OK;

}


status ListEmpty(LinkList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    if(L->next) return FALSE;
    else return TRUE;

}


int ListLength(LinkList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    int length = 0;
    LinkList t = L->next;
    while(t) {
        length++;
        t = t->next;
    }
    return length;
}


status GetElem(LinkList L,int i,ElemType &e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    LinkList p = L->next;
    int j = 1;
    while(p && j < i){
        p = p->next;
        j++;
    }
    if(!p || j > i) return ERROR;
    e = p->data;
    return OK;

}


status LocateElem(LinkList L,ElemType e)
// 如果线性表L存在，查找元素e在线性表L中的位置序号；如果e不存在，返回ERROR；当线性表L不存在时，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    int j = 1, flag = 0;
    LinkList p = L->next;
    while(p){
        if(p->data == e){
            flag = 1;
            break;
        }
        p = p->next;
        j++;
    }
    if(flag) return j;
    else return ERROR;

}


status PriorElem(LinkList L,ElemType e,ElemType &pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    LinkList p = L, q = L->next;
    int flag = 0;
    while(q != NULL){
        if(q->data == e){
            flag = 1;
            pre = p->data;
            break;
        }
        p = q;
        q = q->next;
    }
    if(flag && p != L)
        return OK;
    if(!flag) return ERROR;
    if(p == L) return error;

}

status NextElem(LinkList L,ElemType e,ElemType &next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    LinkList q = L->next;
    while(q != NULL){
        if(q->data == e){
            break;
        }
        q = q->next;
    }
    if(q && q->next){
        next = q->next->data;
        return OK;
    }
    if(!q) return ERROR;
    if(!q->next) return error;

}


status ListInsert(LinkList &L,int i,ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    LinkList p = L;
    int j = 1;
    while(p && j < i){
        p = p->next;
        ++j;
    }
    if(!p || j > i) return ERROR;
    LinkList s = (LinkList)malloc(sizeof(LinkList));
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;

}


status ListDelete(LinkList &L,int i,ElemType &e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    LinkList p = L;
    int j = 1;
    while(p && j < i){
        p = p->next;
        ++j;
    }
    if(!p || !p->next || j > i) return ERROR;
    LinkList s = p->next;
    e = s->data;
    p->next = p->next->next;
    free(s);
    return OK;

}


status ListTraverse(LinkList L)
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    LinkList p = L;
    while(p->next){
        printf("%d",p->next->data);
        if(p->next->next != NULL)
            printf(" ");
        p = p->next;
    }
    return OK;

}


status MaxSubArray(LinkList L){
// 返回线性表中的连续数组和的最大值
    LinkList p = L->next;
    int i = 0;
    int elem[LIST_INIT_SIZE];
    for( ; p; p = p->next, i++){
        elem[i] = p->data;
    }
    int l = 0, r, max = 0, temp = elem[0];
    for(r = 0; r < i; r++){
        while(max < 0 && l <= r){
            max -= elem[l++];
        }
        max += elem[r];
        if(max > temp) temp = max;
    }
    return temp;

}

status SubArrayNum(LinkList L, int k){
// 返回线性表中连续数组和为k的连续数组数目
    int sum = 0, count = 0;
    int l[LIST_INIT_SIZE];
    l[0] = 0;
    int h;
    LinkList p = L->next;
    //获取前缀和
    for(h = 0; p; p = p->next, h++){
        sum += p->data;
        l[h + 1] = sum;
    }
    //通过前缀和之间的差值计算连续数组和
    for(int i = 0; i < h; i++)
        for(int j = i + 1; j < h + 1; j++)
            if(l[j] - l[i] == k)
                count++;
    return count;

}

status sortList(LinkList &L){
//冒泡法进行单链表排序
    if(!L) return INFEASIBLE;
    if(!L->next) return ERROR;
    LinkList pre = NULL, cur = NULL, next = NULL, end = NULL, temp = NULL;
    while(L->next != end){
        for(pre = L, cur = L->next, next = L->next->next; next != end ; pre = pre->next, cur = cur->next, next = next->next){
            if(cur->data > next->data){
                cur->next = next->next;
                pre->next = next;
                next->next = cur;
                temp = cur;
                cur = next;
                next = temp;
            }
        }
        end = cur;
    }
    return OK;
}

status SaveList(LinkList L,char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
    if(!L) return INFEASIBLE;
    FILE  *fp;
    int i;
    char ch;
    if ((fp = fopen(FileName,"wb")) == NULL){
        printf("File open error\n ");
        exit(-1);
    }
    ch = fgetc(fp);
    if(ch != EOF){
        printf("该文件不能读入！\n");
        return ERROR;
    }
    if ((fp = fopen(FileName, "wb")) == NULL) {
        printf("File open error\n ");
        exit(-1);
    }
    LinkList p;
    p = L->next;
    while (p)
    {
        fwrite(&p->data,sizeof(ElemType), 1, fp);
        p = p->next;
    }
    fclose(fp);
    return OK;

}

status LoadList(LinkList &L,char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
{
    if(L) return INFEASIBLE;
    FILE  *fp;
    L=(LinkList)malloc(sizeof(LNode));
    int temp;
    if ((fp = fopen(FileName, "rb")) == NULL){
        printf("File open error\n ");
        exit(-1);
    }
    LinkList t=L;
    while (fread(&temp, sizeof(ElemType), 1, fp))
    {
        LinkList n = (LinkList)malloc(sizeof(LNode));
        n->data = temp;
        t->next = n;
        t = n;
        t->next = NULL;
    }
    fclose(fp);
    return OK;

}

status AddList(LISTS &Lists,char ListName[])
// 只需要在Lists中增加一个名称为ListName的空线性表，线性表数据又后台测试程序插入。
{
	for(int i = 0; i < Lists.length; i++){
		if(!strcmp(Lists.elem[i].name,ListName)) return INFEASIBLE;
	}
    strcpy(Lists.elem[Lists.length].name,ListName);
    Lists.elem[Lists.length].L = NULL;
    Lists.length++;
    return OK;
}

status RemoveList(LISTS &Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
{
    for(int i = 0; i < Lists.length; i++)
        if(!strcmp(ListName, Lists.elem[i].name)){
        	if(Lists.elem[i].L)
            	DestroyList(Lists.elem[i].L);
            for (int j = i; j < Lists.length - 1; j++)
                Lists.elem[j] = Lists.elem[j + 1];
            Lists.length--;
            return OK;
        }
    return ERROR;

}

int LocateList(LISTS Lists,char ListName[])
// 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回0
{
    if(!Lists.elem) return INFEASIBLE;//疑问未解决
    for(int i = 0; i < Lists.length; i++)
        if(!strcmp(ListName, Lists.elem[i].name))
            return i + 1;
    return 0;

}

status TraverseList(LISTS Lists){
// 如果多线性表不为空，依次显示多线性表的名称，每个名称间空一格，返回OK；如果多线性表为空，返回INFEASIBLE。
    if(Lists.length == 0) return INFEASIBLE;
    printf("\n-----------all names -----------------------\n");
    for(int i = 0; i < Lists.length; i++){
        printf("%s",Lists.elem[i].name);
        if(i != Lists.length - 1) printf(" ");
    }
    printf("\n------------------ end ------------------------\n");
    return OK;
}

status SelectList(LISTS Lists, int i){
// 进行线性表的选择
    if(Lists.length == 0) return INFEASIBLE;
    if(i < 1 || i > Lists.length) return ERROR;
    L = Lists.elem[i - 1].L;
    return OK;
}

status reverseList(LinkList &L){
//迭代反转思想从头开始依次反转链表
    if(!L) return INFEASIBLE;
    if(!L->next) return ERROR;
    LinkList p = NULL, q = L->next, r = L->next->next;
    while(1){
        q->next = p;
        if(!r) break;
        p = q;
        q = r;
        r = r->next;
    }
    L->next = q;
    return OK;
}

status RemoveNthFromEnd(LinkList L, int n){
//移除倒数第n个元素 
    if(!L) return INFEASIBLE;
    int length = ListLength(L);
    if(n < 1 || n > length) return ERROR;
    int pre = length - n + 1;
    LinkList p = L, tmp = NULL;
    while(--pre){
        p = p->next;
    }
    tmp = p->next;
    p->next = tmp->next;
    free(tmp);
    return OK;
}

status CreatList(LinkList L){
	LinkList p = L;
	while(1){
		LinkList s= (LinkList)malloc(sizeof(LinkList));
		scanf("%d", &s->data);
		if(s->data == 0)
			break; 
		p->next = s;
		p = s;
		s->next = NULL;
	}
	return OK;
}

int main(){
    int op=1;
    int length, flag, temp, num = 0;
    char FileName[100];
    char Name[20];
    Lists.length = 0;
    while(op){
        system("cls");
        printf("\n\n");
        printf("            Menu for Linear Table On Chain Structure \n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("    	  1. InitList    线性表初始化         13. MaxSubArray      线性表最大连续数组和获取\n");
        printf("    	  2. DestroyList 线性表销毁           14. SubArrayNum      线性表指定连续数组和数目\n");
        printf("    	  3. ClearList   线性表清空           15. sortList         线性表排序\n");
        printf("    	  4. ListEmpty   线性表判空           16. SaveList         线性表文件保存\n");
        printf("    	  5. ListLength  线性表获取长度       17. LoadList         线性表文件录入\n");
        printf("    	  6. GetElem     线性表元素获取       18. AddList          多线性表添加\n");
        printf("          7. LocateElem  线性表元素查找       19. RemoveList       多线性表删除\n");
        printf("          8. PriorElem   线性表元素前驱获取   20. LocateList       多线性表位置查找\n");
        printf("          9. NextElem    线性表元素后继获取   21. TraverseList     多线性表遍历\n");
        printf("          10. ListInsert 线性表元素插入       22. SelectList       线性表操作选择\n");
        printf("          11. ListDelete 线性表元素删除       23. reverseList      线性表翻转\n");
        printf("          12. ListTraverse   线性表遍历       24. RemoveNthFromEnd 移除倒数元素\n");
        printf("    	  0. Exit        退出\n");
        printf("          附加功能：25. 线性表迅速输入！\n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("          说明：每次操作过后请点击空格确认才能进行下一步操作！\n");
        printf("\n          当前操作的线性表为：");
        if(num < 1|| num > Lists.length){
            if(num > Lists.length){
                L = NULL;
                num = 0;
            }
            printf("默认线性表");
            if(!L)
                printf("(未创建)");
            printf("\n\n\n");
        }
        else{
        	printf("%s",Lists.elem[num - 1].name);
        	if(!L)
                printf("(未创建)");
            printf("\n\n\n");
		}
            
        if(op > 25 || op < 0)
            printf("上一步命令出错！请根据菜单正确输入！\n\n\n");
        printf("请选择你的操作[0~22]:");
        scanf("%d",&op);
        switch(op){
        	case 25:
				if(!L) {
					printf("线性表未创建！\n"); 
					getchar();getchar();
                	break;
				}
				printf("请输入："); 
				CreatList(L);
				getchar();getchar();
                break;
            case 1:
                //printf("\n----IntiList功能待实现！\n");
                if(InitList(L) == OK) printf("线性表创建成功！\n");
                else printf("线性表创建失败！\n");
                getchar();getchar();
                break;
            case 2:
                //printf("\n----DestroyList功能待实现！\n");
                if(DestroyList(L) == OK) printf("线性表销毁成功！\n");
                else printf("线性表销毁失败！\n");
                getchar();getchar();
                break;
            case 3:
                //printf("\n----ClearList功能待实现！\n");
                flag = ClearList(L); 
                if(flag == OK) printf("线性表清空成功！\n");
                else if (flag == ERROR) printf("线性表清空失败！\n");
                else printf("线性表未创建！\n");
                getchar();getchar();
                break;
            case 4:
                //printf("\n----ListEmpty功能待实现！\n");
                if(ListEmpty(L) == OK) printf("线性表为空！\n");
                else if(ListEmpty(L) == INFEASIBLE) printf("线性表未创建！\n");
				else printf("线性表非空！\n");
                getchar();getchar();
                break;
            case 5:
                //printf("\n----ListLength功能待实现！\n");
                length = ListLength(L);
                if(length != INFEASIBLE) printf("线性表的长度为：%d\n", length);
                else printf("线性表未创建!\n");
                getchar();getchar();
                break;
            case 6:
                //printf("\n----GetElem功能待实现！\n");
                int x, y;
                printf("请输入要获取元素的位置：");
                scanf("%d",&x);
                flag = GetElem(L, x, y);
                if(flag == INFEASIBLE) printf("线性表未创建!\n");
                else if(flag == OK) printf("该元素为：%d\n", y);
                else printf("位置不合法！\n");
                getchar();getchar();
                break;
            case 7:
                //printf("\n----LocateElem功能待实现！\n");
                int a;
                printf("请输入想要查找的元素：");
                scanf("%d",&a);
                flag = LocateElem(L, a);
                if(flag == INFEASIBLE) printf("线性表未创建!\n");
                else if(flag) printf("该元素存在且元素逻辑索引为：%d\n", flag);
                else printf("该元素不存在！\n");
                getchar();getchar();
                break;
            case 8:
                //printf("\n----PriorElem功能待实现！\n");
                printf("请输入想要查找的元素(获取前驱)：");
                scanf("%d",&a);
                flag = PriorElem(L, a, temp);
                if(flag == INFEASIBLE) printf("线性表未创建!\n");
                else if(flag == OK) printf("该元素存在且前驱元素为：%d\n", temp);
                else if(flag == ERROR)printf("该元素不存在！\n");
                else printf("该元素不存在前驱！\n");
                getchar();getchar();
                break;
            case 9:
                //printf("\n----NextElem功能待实现！\n");
                printf("请输入想要查找的元素(获取后继)：");
                scanf("%d",&a);
                flag = NextElem(L, a, temp);
                if(flag == INFEASIBLE) printf("线性表未创建!\n");
                else if(flag == OK) printf("该元素存在且后继元素为：%d\n", temp);
                else if(flag == ERROR) printf("该元素不存在！\n");
                else printf("该元素不存在后继！\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----ListInsert功能待实现！\n");
                int i, e;
                printf("请输入要插入的元素位置：");
                scanf("%d",&i);
                printf("请输入要插入的元素：");
                scanf("%d",&e);
                if(ListInsert(L, i, e) == OK) printf("线性表插入成功！\n");
                else printf("线性表插入失败！\n");
                getchar();getchar();
                break;
            case 11:
                //printf("\n----ListDelete功能待实现！\n");
                printf("请输入要删除的元素位置：");
                scanf("%d",&i);
                if(ListDelete(L, i, e) == OK){
                    printf("线性表删除成功！\n");
                    printf("删除的元素为：%d\n",e);
                }
                else printf("线性表删除失败！\n");
                getchar();getchar();
                break;
            case 12:
                //printf("\n----ListTraverse功能待实现！\n");
                if(!ListTraverse(L)) printf("线性表是空表！\n");
                getchar();getchar();
                break;
            case 13:
                //printf("\n----MaxSubArray功能待实现！\n");
                if(!L) printf("线性表未创建！\n");
                else if (ListEmpty(L)) printf("线性表为空！\n");
                else printf("最大子数组之和为：%d\n",MaxSubArray(L));
                getchar();getchar();
                break;
            case 14:
                //printf("\n----SubArrayNum功能待实现！\n");
                if(!L){printf("线性表未创建！\n");getchar();getchar();break;}
                else if (ListEmpty(L)) {printf("线性表为空！\n");getchar();getchar();break;}
                printf("请输入寻找的连续数组的和：");
                scanf("%d",&flag);
                printf("和为数%d的连续数组数目为：%d\n",flag,SubArrayNum(L,flag));
                getchar();getchar();
                break;
            case 15:
                //printf("\n----sortList功能待实现！\n");
                flag = sortList(L);
                if(flag == ERROR) printf("线性表是空表！\n");
                else if(flag == INFEASIBLE) printf("线性表未创建！\n");
                else printf("线性表排序成功！\n");
                getchar();getchar();
                break;
            case 16:
                //printf("\n----SaveList功能待实现！\n");
                printf("请输入要保存的文件名称：");
                scanf("%s",FileName);
                flag = SaveList(L, FileName);
                if(flag == INFEASIBLE) printf("文件读入失败！\n");
                else if(flag == ERROR);
                else printf("文件读入成功！\n");
                getchar();getchar();
                break;
            case 17:
                //printf("\n----LoadList功能待实现！\n");
                printf("请输入要录入的文件名称：");
                scanf("%s",FileName);
                if(LoadList(L, FileName) == INFEASIBLE) printf("文件录入失败！\n");
                else printf("文件录入成功！\n");
                getchar();getchar();
                break;
            case 18:
                //printf("\n----AddList功能待实现！\n");
                if(Lists.length == MAXlength) {
                    printf("多线性表管理已满，请清除某些线性表后再操作！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入新增线性表的名称：");
                scanf("%s",Name);
                flag = AddList(Lists, Name);
                if(flag == INFEASIBLE) printf("该名称的线性表已经存在！\n");
                else printf("%s已成功添加！\n",Name);
                getchar();getchar();
                break;
            case 19:
                //printf("\n---RemoveList功能待实现！\n");
                if(Lists.length == 0) {
                    printf("多线性表管理已空，请添加某些线性表后再操作！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入删除线性表的名称：");
                scanf("%s",Name);
                flag = RemoveList(Lists, Name);
                if(flag == OK)printf("%s已成功删除！\n",Name);
                else printf("线性表不存在！\n");
                getchar();getchar();
                break;
            case 20:
                //printf("\n---LocateList功能待实现！\n");
                printf("请输入查找线性表的名称：");
                scanf("%s",Name);
                if(LocateList(Lists, Name)) printf("该线性表的逻辑索引为：%d\n", LocateList(Lists, Name));
                else printf("线性表查找失败！\n");
                getchar();getchar();
                break;
            case 21:
                //printf("\n----TraverseList功能待实现！\n");
                if(TraverseList(Lists) == INFEASIBLE) printf("多线性表为空！\n");
                getchar();getchar();
                break;
            case 22:
                //printf("\n----SelectList功能待实现！\n");
                printf("请选择要处理的线性表的逻辑索引：");
                scanf("%d",&flag);
                if(SelectList(Lists, flag) == OK) {
                    printf("已选取成功！\n");
                    num = flag;
                }
                else printf("选取失败！\n");
                getchar();getchar();
                break;
            case 23:
                //printf("\n----reverseList功能待实现！\n");
                flag = reverseList(L);
                if(flag == INFEASIBLE) printf("线性表未创建！\n");
                else if(flag == ERROR) printf("该线性表为空！\n");
                else printf("链表反转成功！\n");
                getchar();getchar();
                break;
            case 24://printf("\n----RemoveNthFromEnd功能待实现！\n");
                int n;
                printf("请输入想要删除倒数第几个元素：");
                scanf("%d",&n);
                flag = RemoveNthFromEnd(L, n);
                if(flag == INFEASIBLE) printf("线性表未创建！\n");
                else if(flag == ERROR) printf("位置不合法！\n");
                else printf("删除成功！\n");
                getchar();getchar();
                break;
            case 0:
                break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    return 0;
}//end of main()

