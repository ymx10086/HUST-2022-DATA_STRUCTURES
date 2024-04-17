#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int status;
typedef int ElemType; //数据元素类型定义
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
#define MAXlength 10
typedef struct{  //顺序表（顺序结构）的定义
    ElemType * elem;
    int length;
    int listsize;
}SqList;
SqList L;

typedef struct{  //线性表的集合类型定义
    struct { char name[30];
        SqList L;
    } elem[11];
    int length;
}LISTS;
LISTS Lists;      //线性表集合的定义Lists

status InitList(SqList& L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
    if(L.elem) return INFEASIBLE;
    L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if(!L.elem)
        exit(OVERFLOW);
    L.length = 0;
    L.listsize = LIST_INIT_SIZE;
    return OK;

}

status DestroyList(SqList& L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    free(L.elem);
    L.elem = NULL;
    return OK;

}

status ClearList(SqList& L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    L.length = 0;
    return OK;

}

status ListEmpty(SqList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    if(!L.length) return TRUE;
    else return FALSE;

}

status ListLength(SqList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    return L.length;
}

status GetElem(SqList L,int i,ElemType &e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    if(i < 1 || i > L.length) return ERROR;
    e = L.elem[i - 1];
    return OK;

}

int compare(SqList L, int i, ElemType e){
	if(L.elem[i] == e) return 1;
	return 0;
}

int LocateElem(SqList L,ElemType e, int (*compare)(SqList, int, ElemType))
// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；如果e不存在，返回0；当线性表L不存在时，返回INFEASIBLE（即-1）。
{
    if(!L.elem) return INFEASIBLE;
    for(int i = 0; i < L.length; i++){
        if(compare(L, i, e)) return i + 1;
    }
    return 0;

}

status PriorElem(SqList L,ElemType e,ElemType &pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    int i = LocateElem(L, e, compare) - 1;
    if(i <= 0 || i == L.length) return ERROR;
    else {
        pre = L.elem[i - 1];
        return OK;
    }

}

status NextElem(SqList L,ElemType e,ElemType &next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    int i = LocateElem(L, e, compare) - 1;
    if(i >= L.length - 1) return ERROR;
    else {
        next = L.elem[i + 1];
        return OK;
    }

}

status ListInsert(SqList &L,int i,ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    if(i < 1 || i > L.length + 1) return ERROR;
    if(L.length == L.listsize){
        ElemType* newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
        L.elem = newbase;
        L.listsize += LISTINCREMENT;
    }
    int j;
    for(j = L.length; j >= i - 1; j--)
        L.elem[j + 1] = L.elem[j];
    L.elem[j + 1] = e;
    L.length++;
    return OK;

}

status ListDelete(SqList &L,int i,ElemType &e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    if(i < 1 || i > L.length) return ERROR;
    int j;
    e = L.elem[i - 1];
    for(j = i - 1; j <= L.length ; j++)
        L.elem[j] = L.elem[j + 1];
    L.length--;
    return OK;

}

int visit(SqList L, int i){
	printf("%d", L.elem[i]);
}

status ListTraverse(SqList L, int (*visit)(SqList, int))
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L.elem) {
        printf("线性表未创建！\n");
        return INFEASIBLE;
    }
    printf("\n-----------all elements -----------------------\n");
    for(int i = 0; i < L.length; i++){
        visit(L, i);
        if(i != L.length - 1) printf(" ");
    }
    printf("\n------------------ end ------------------------\n");
    return L.length;

}


status MaxSubArray(SqList L){
// 返回线性表中的连续数组和的最大值 
    int l = 0, r, max = 0, temp = L.elem[0];
    for(r = 0; r < L.length; r++){
        while(max < 0 && l <= r){
            max -= L.elem[l++];
        }
        max += L.elem[r];
        if(max > temp) temp = max;
    }
    return temp;

}

status SubArrayNum(SqList L, int k){
// 返回线性表中连续数组和为k的连续数组数目 	
    int sum = 0, count = 0;
    int *l = (int *)malloc(sizeof(int) * (L.length + 1));
    l[0] = 0;
    //获取前缀和 
    for(int i = 0; i < L.length; i++){
        sum += L.elem[i];
        l[i + 1] = sum;
    }
    //通过前缀和之间的差值计算连续数组和 
    for(int i = 0; i < L.length; i++)
        for(int j = i + 1; j < L.length + 1; j++)
            count += (l[j] - l[i] == k);
    return count;

}

void merge(int *elem, int l, int r){
	if(r - l <= 1){
		if(elem[l] > elem[r]){
			int t = elem[l];
			elem[l] = elem[r];
			elem[r] = t;
		}
		return; 
	}	
	int mid = (l + r) >> 1;
	merge(elem, l, mid);
	merge(elem, mid + 1, r);
	int p1 = l, p2 = mid + 1, k = 0;
	int *temp = (int *)malloc(sizeof(int)*(r - l + 1));
	while(p1 <= mid || p2 <= r){
		if(p2 > r || (p1 <= mid && elem[p1] < elem[p2]))
		temp[k++] = elem[p1++];
		else
		temp[k++] = elem[p2++];
	}
	memcpy(elem + l, temp, sizeof(int) * (r - l + 1));
	free(temp);
	return;
}

status sortList(SqList& L){
// 如果线性表L存在，将线性表中的元素排序；如果线性表L不存在，返回INFEASIBLE。	
    if(!L.elem) {
        printf("线性表未创建！\n");
        return INFEASIBLE;
    }
    merge(L.elem, 0, L.length - 1);
    return L.length;
}

status SaveList(SqList L,char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
    if(!L.elem) return INFEASIBLE;
    FILE *fp;
    char ch;
    if ((fp = fopen(FileName,"rb")) == NULL){
        printf("File open error\n ");
        exit(-1);
    } 
	ch = fgetc(fp); 
    if(ch != EOF){
    	printf("该文件已有内容，不能读入！\n");
    	return ERROR;
	} 
    if ((fp = fopen(FileName,"wb")) == NULL){
        printf("File open error\n ");
        exit(-1);
    } 
   
    fwrite(L.elem, sizeof(ElemType), L.length, fp);
    fclose(fp);
    return OK;

}

status  LoadList(SqList &L,char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
{
    if(L.elem) return INFEASIBLE;
    FILE *fp;
    if ((fp = fopen(FileName,"rb")) == NULL){
        printf("File open error\n ");
        exit(-1);
    }
    L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    L.length = 0;
    L.listsize = LIST_INIT_SIZE;
    while(fread(&L.elem[L.length], sizeof(ElemType), 1, fp))
        L.length++;
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
    Lists.elem[Lists.length].L.length=0;
    Lists.elem[Lists.length].L.elem=(ElemType *)malloc(sizeof(ElemType)*LIST_INIT_SIZE);
    Lists.elem[Lists.length].L.listsize=LIST_INIT_SIZE;
    Lists.length++;
    return OK;
}

status RemoveList(LISTS &Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
{
    for(int i = 0; i < Lists.length; i++)
        if(!strcmp(ListName, Lists.elem[i].name)){
        	if(Lists.elem[i].L.elem)
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

int main(){
    int op=1;
    int length, flag, temp, num = 0;
    char FileName[100];
    char Name[20];
    Lists.length = 0;
    while(op){
        system("cls");
        printf("\n\n");
        printf("            Menu for Linear Table On Sequence Structure \n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("    	  1. InitList    线性表初始化         12. ListTraverse   线性表遍历\n");
        printf("    	  2. DestroyList 线性表销毁           13. MaxSubArray    线性表最大连续数组和获取\n");
        printf("    	  3. ClearList   线性表清空           14. SubArrayNum    线性表指定连续数组和数目\n");
        printf("    	  4. ListEmpty   线性表判空           15. sortList       线性表排序\n");
        printf("    	  5. ListLength  线性表获取长度       16. SaveList       线性表文件保存\n");
        printf("    	  6. GetElem     线性表元素获取       17. LoadList       线性表文件录入\n");
        printf("          7. LocateElem  线性表元素查找       18. AddList        多线性表添加\n");
        printf("          8. PriorElem   线性表元素前驱获取   19. RemoveList     多线性表删除\n");
        printf("          9. NextElem    线性表元素后继获取   20. LocateList     多线性表位置查找\n");
        printf("          10. ListInsert 线性表元素插入       21. TraverseList   多线性表遍历\n");
        printf("          11. ListDelete 线性表元素删除       22. SelectList     线性表操作选择\n");
        printf("    	  0. Exit        退出\n");
        printf("          ----------------------------------------------------------------------------\n");
		printf("          说明：每次操作过后请点击空格确认才能进行下一步操作！\n");
		printf("\n          当前操作的线性表为：");
		if(num < 1|| num > Lists.length){
			if(num > Lists.length){
				L.elem = NULL;
				L.length = 0; 
				num = 0;
			}
			printf("默认线性表");
			if(!L.elem)
				printf("(未创建)");
			printf("\n\n\n");	
		}
		else
			printf("%s\n\n\n",Lists.elem[num - 1].name); 
		if(op > 22 || op < 0)
        	printf("上一步命令出错！请根据菜单正确输入！\n\n\n");
        printf("请选择你的操作[0~22]:");
        scanf("%d",&op);
        
        switch(op){
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
                if(ClearList(L) == OK) printf("线性表清空成功！\n");
                else printf("线性表清空失败！\n");
                getchar();getchar();
                break;
            case 4:
                //printf("\n----ListEmpty功能待实现！\n");
                if(ListEmpty(L) == OK) printf("线性表为空！\n");
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
                else if(flag == OK) printf("线性表中的第%d个元素为%d\n", x, y);
                else printf("输入的逻辑索引不合法！\n");
                getchar();getchar();
                break;
            case 7:
                //printf("\n----LocateElem功能待实现！\n");
                int a;
                printf("请输入想要查找的元素：");
                scanf("%d",&a);
                flag = LocateElem(L, a, compare);
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
                else printf("该元素不存在或不存在前驱！\n");
                getchar();getchar();
                break;
            case 9:
                //printf("\n----NextElem功能待实现！\n");
                printf("请输入想要查找的元素(获取后继)：");
                scanf("%d",&a);
                flag = NextElem(L, a, temp);
                if(flag == INFEASIBLE) printf("线性表未创建!\n");
                else if(flag == OK) printf("该元素存在且后继元素为：%d\n", temp);
                else printf("该元素不存在或不存在后继！\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----ListInsert功能待实现！\n");
                int i, e;
                printf("请输入要插入的元素位置：");
                scanf("%d",&i);
                printf("请输入要插入的元素：");
                scanf("%d",&e);
                flag = ListInsert(L, i, e);
                if(flag == OK) printf("线性表插入成功！\n");
                else if(flag == INFEASIBLE) printf("线性表不存在，插入失败！\n"); 
				else printf("插入位置不合法，线性表插入失败！\n");
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
                if(!ListTraverse(L, visit)) printf("线性表是空表！\n");
                getchar();getchar();
                break;
                //-2 1 -3 4 -1 2 1 -5 4 
            case 13:
                //printf("\n----MaxSubArray功能待实现！\n");
                if(!L.elem) printf("线性表未创建！\n");
                else if (ListEmpty(L)) printf("线性表为空！\n");
                else printf("最大子数组之和为：%d\n",MaxSubArray(L));
                getchar();getchar();
                break;
                //6 
            case 14:
                //printf("\n----SubArrayNum功能待实现！\n");
                if(!L.elem){printf("线性表未创建！\n");getchar();getchar();break;}
                else if (ListEmpty(L)) {printf("线性表为空！\n");getchar();getchar();break;}
                printf("请输入寻找的连续数组的和：");
                scanf("%d",&flag);
                printf("和为数%d的连续数组数目为：%d\n",flag,SubArrayNum(L,flag));
                getchar();getchar();
                break;
                //3 5
                //5 2
                //15 0
            case 15:
                //printf("\n----sortList功能待实现！\n");
                flag = sortList(L);
                if(!flag) printf("线性表是空表！\n");
                else if(flag != INFEASIBLE) printf("线性表排序成功！\n");
                getchar();getchar();
                break;
            case 16:
                //printf("\n----SaveList功能待实现！\n");
                printf("请输入要保存的文件名称：");
                scanf("%s",FileName);
                flag = SaveList(L, FileName);
                if(flag == INFEASIBLE) printf("线性表不存在!文件保存失败！\n");
                else if(flag == ERROR);
                else printf("文件保存成功！\n");
                getchar();getchar();
                break;
            case 17:
                //printf("\n----LoadList功能待实现！\n");
                printf("请输入要录入的文件名称：");
                scanf("%s",FileName);
                if(LoadList(L, FileName) == INFEASIBLE) printf("线性表存在!文件录入失败！\n");
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
                	printf("线性表已选取成功！\n");
                	num = flag;
				}
                else printf("线性表选取失败！\n");
                getchar();getchar();
                break;
            case 0: 
                break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    return 0;
}//end of main()

