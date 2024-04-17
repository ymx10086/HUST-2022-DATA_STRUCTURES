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
typedef int ElemType; //����Ԫ�����Ͷ���
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
#define MAXlength 10
typedef struct{  //˳���˳��ṹ���Ķ���
    ElemType * elem;
    int length;
    int listsize;
}SqList;
SqList L;

typedef struct{  //���Ա�ļ������Ͷ���
    struct { char name[30];
        SqList L;
    } elem[11];
    int length;
}LISTS;
LISTS Lists;      //���Ա��ϵĶ���Lists

status InitList(SqList& L)
// ���Ա�L�����ڣ�����һ���յ����Ա�����OK�����򷵻�INFEASIBLE��
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
// ������Ա�L���ڣ��������Ա�L���ͷ�����Ԫ�صĿռ䣬����OK�����򷵻�INFEASIBLE��
{
    if(!L.elem) return INFEASIBLE;
    free(L.elem);
    L.elem = NULL;
    return OK;

}

status ClearList(SqList& L)
// ������Ա�L���ڣ�ɾ�����Ա�L�е�����Ԫ�أ�����OK�����򷵻�INFEASIBLE��
{
    if(!L.elem) return INFEASIBLE;
    L.length = 0;
    return OK;

}

status ListEmpty(SqList L)
// ������Ա�L���ڣ��ж����Ա�L�Ƿ�Ϊ�գ��վͷ���TRUE�����򷵻�FALSE��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(!L.elem) return INFEASIBLE;
    if(!L.length) return TRUE;
    else return FALSE;

}

status ListLength(SqList L)
// ������Ա�L���ڣ��������Ա�L�ĳ��ȣ����򷵻�INFEASIBLE��
{
    if(!L.elem) return INFEASIBLE;
    return L.length;
}

status GetElem(SqList L,int i,ElemType &e)
// ������Ա�L���ڣ���ȡ���Ա�L�ĵ�i��Ԫ�أ�������e�У�����OK�����i���Ϸ�������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ�����Ԫ��e�����Ա�L�е�λ����Ų����ظ���ţ����e�����ڣ�����0�������Ա�L������ʱ������INFEASIBLE����-1����
{
    if(!L.elem) return INFEASIBLE;
    for(int i = 0; i < L.length; i++){
        if(compare(L, i, e)) return i + 1;
    }
    return 0;

}

status PriorElem(SqList L,ElemType e,ElemType &pre)
// ������Ա�L���ڣ���ȡ���Ա�L��Ԫ��e��ǰ����������pre�У�����OK�����û��ǰ��������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ���ȡ���Ա�LԪ��e�ĺ�̣�������next�У�����OK�����û�к�̣�����ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ���Ԫ��e���뵽���Ա�L�ĵ�i��Ԫ��֮ǰ������OK��������λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ�ɾ�����Ա�L�ĵ�i��Ԫ�أ���������e�У�����OK����ɾ��λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ�������ʾ���Ա��е�Ԫ�أ�ÿ��Ԫ�ؼ��һ�񣬷���OK��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(!L.elem) {
        printf("���Ա�δ������\n");
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
// �������Ա��е���������͵����ֵ 
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
// �������Ա������������Ϊk������������Ŀ 	
    int sum = 0, count = 0;
    int *l = (int *)malloc(sizeof(int) * (L.length + 1));
    l[0] = 0;
    //��ȡǰ׺�� 
    for(int i = 0; i < L.length; i++){
        sum += L.elem[i];
        l[i + 1] = sum;
    }
    //ͨ��ǰ׺��֮��Ĳ�ֵ������������� 
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
// ������Ա�L���ڣ������Ա��е�Ԫ������������Ա�L�����ڣ�����INFEASIBLE��	
    if(!L.elem) {
        printf("���Ա�δ������\n");
        return INFEASIBLE;
    }
    merge(L.elem, 0, L.length - 1);
    return L.length;
}

status SaveList(SqList L,char FileName[])
// ������Ա�L���ڣ������Ա�L�ĵ�Ԫ��д��FileName�ļ��У�����OK�����򷵻�INFEASIBLE��
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
    	printf("���ļ��������ݣ����ܶ��룡\n");
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
// ������Ա�L�����ڣ���FileName�ļ��е����ݶ��뵽���Ա�L�У�����OK�����򷵻�INFEASIBLE��
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
// ֻ��Ҫ��Lists������һ������ΪListName�Ŀ����Ա����Ա������ֺ�̨���Գ�����롣
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
// Lists��ɾ��һ������ΪListName�����Ա�
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
// ��Lists�в���һ������ΪListName�����Ա��ɹ������߼���ţ����򷵻�0
{
    if(!Lists.elem) return INFEASIBLE;//����δ���
    for(int i = 0; i < Lists.length; i++)
        if(!strcmp(ListName, Lists.elem[i].name))
            return i + 1;
    return 0;

}

status TraverseList(LISTS Lists){
// ��������Ա�Ϊ�գ�������ʾ�����Ա�����ƣ�ÿ�����Ƽ��һ�񣬷���OK����������Ա�Ϊ�գ�����INFEASIBLE��
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
// �������Ա��ѡ��
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
        printf("    	  1. InitList    ���Ա��ʼ��         12. ListTraverse   ���Ա����\n");
        printf("    	  2. DestroyList ���Ա�����           13. MaxSubArray    ���Ա������������ͻ�ȡ\n");
        printf("    	  3. ClearList   ���Ա����           14. SubArrayNum    ���Ա�ָ�������������Ŀ\n");
        printf("    	  4. ListEmpty   ���Ա��п�           15. sortList       ���Ա�����\n");
        printf("    	  5. ListLength  ���Ա��ȡ����       16. SaveList       ���Ա��ļ�����\n");
        printf("    	  6. GetElem     ���Ա�Ԫ�ػ�ȡ       17. LoadList       ���Ա��ļ�¼��\n");
        printf("          7. LocateElem  ���Ա�Ԫ�ز���       18. AddList        �����Ա����\n");
        printf("          8. PriorElem   ���Ա�Ԫ��ǰ����ȡ   19. RemoveList     �����Ա�ɾ��\n");
        printf("          9. NextElem    ���Ա�Ԫ�غ�̻�ȡ   20. LocateList     �����Ա�λ�ò���\n");
        printf("          10. ListInsert ���Ա�Ԫ�ز���       21. TraverseList   �����Ա����\n");
        printf("          11. ListDelete ���Ա�Ԫ��ɾ��       22. SelectList     ���Ա����ѡ��\n");
        printf("    	  0. Exit        �˳�\n");
        printf("          ----------------------------------------------------------------------------\n");
		printf("          ˵����ÿ�β������������ո�ȷ�ϲ��ܽ�����һ��������\n");
		printf("\n          ��ǰ���������Ա�Ϊ��");
		if(num < 1|| num > Lists.length){
			if(num > Lists.length){
				L.elem = NULL;
				L.length = 0; 
				num = 0;
			}
			printf("Ĭ�����Ա�");
			if(!L.elem)
				printf("(δ����)");
			printf("\n\n\n");	
		}
		else
			printf("%s\n\n\n",Lists.elem[num - 1].name); 
		if(op > 22 || op < 0)
        	printf("��һ�������������ݲ˵���ȷ���룡\n\n\n");
        printf("��ѡ����Ĳ���[0~22]:");
        scanf("%d",&op);
        
        switch(op){
            case 1:
                //printf("\n----IntiList���ܴ�ʵ�֣�\n");
                if(InitList(L) == OK) printf("���Ա����ɹ���\n");
                else printf("���Ա���ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 2:
                //printf("\n----DestroyList���ܴ�ʵ�֣�\n");
                if(DestroyList(L) == OK) printf("���Ա����ٳɹ���\n");
                else printf("���Ա�����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 3:
                //printf("\n----ClearList���ܴ�ʵ�֣�\n");
                if(ClearList(L) == OK) printf("���Ա���ճɹ���\n");
                else printf("���Ա����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 4:
                //printf("\n----ListEmpty���ܴ�ʵ�֣�\n");
                if(ListEmpty(L) == OK) printf("���Ա�Ϊ�գ�\n");
                else printf("���Ա�ǿգ�\n");
                getchar();getchar();
                break;
            case 5:
                //printf("\n----ListLength���ܴ�ʵ�֣�\n");
                length = ListLength(L);
                if(length != INFEASIBLE) printf("���Ա�ĳ���Ϊ��%d\n", length);
                else printf("���Ա�δ����!\n");
                getchar();getchar();
                break;
            case 6:
                //printf("\n----GetElem���ܴ�ʵ�֣�\n");
                int x, y;
                printf("������Ҫ��ȡԪ�ص�λ�ã�");
                scanf("%d",&x);
                flag = GetElem(L, x, y);
                if(flag == INFEASIBLE) printf("���Ա�δ����!\n");
                else if(flag == OK) printf("���Ա��еĵ�%d��Ԫ��Ϊ%d\n", x, y);
                else printf("������߼��������Ϸ���\n");
                getchar();getchar();
                break;
            case 7:
                //printf("\n----LocateElem���ܴ�ʵ�֣�\n");
                int a;
                printf("��������Ҫ���ҵ�Ԫ�أ�");
                scanf("%d",&a);
                flag = LocateElem(L, a, compare);
                if(flag == INFEASIBLE) printf("���Ա�δ����!\n");
                else if(flag) printf("��Ԫ�ش�����Ԫ���߼�����Ϊ��%d\n", flag);
                else printf("��Ԫ�ز����ڣ�\n");
                getchar();getchar();
                break;
            case 8:
                //printf("\n----PriorElem���ܴ�ʵ�֣�\n");
                printf("��������Ҫ���ҵ�Ԫ��(��ȡǰ��)��");
                scanf("%d",&a);
                flag = PriorElem(L, a, temp);
                if(flag == INFEASIBLE) printf("���Ա�δ����!\n");
                else if(flag == OK) printf("��Ԫ�ش�����ǰ��Ԫ��Ϊ��%d\n", temp);
                else printf("��Ԫ�ز����ڻ򲻴���ǰ����\n");
                getchar();getchar();
                break;
            case 9:
                //printf("\n----NextElem���ܴ�ʵ�֣�\n");
                printf("��������Ҫ���ҵ�Ԫ��(��ȡ���)��");
                scanf("%d",&a);
                flag = NextElem(L, a, temp);
                if(flag == INFEASIBLE) printf("���Ա�δ����!\n");
                else if(flag == OK) printf("��Ԫ�ش����Һ��Ԫ��Ϊ��%d\n", temp);
                else printf("��Ԫ�ز����ڻ򲻴��ں�̣�\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----ListInsert���ܴ�ʵ�֣�\n");
                int i, e;
                printf("������Ҫ�����Ԫ��λ�ã�");
                scanf("%d",&i);
                printf("������Ҫ�����Ԫ�أ�");
                scanf("%d",&e);
                flag = ListInsert(L, i, e);
                if(flag == OK) printf("���Ա����ɹ���\n");
                else if(flag == INFEASIBLE) printf("���Ա����ڣ�����ʧ�ܣ�\n"); 
				else printf("����λ�ò��Ϸ������Ա����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 11:
                //printf("\n----ListDelete���ܴ�ʵ�֣�\n");
                printf("������Ҫɾ����Ԫ��λ�ã�");
                scanf("%d",&i);
                if(ListDelete(L, i, e) == OK){
                	printf("���Ա�ɾ���ɹ���\n");
                	printf("ɾ����Ԫ��Ϊ��%d\n",e); 
				} 
                else printf("���Ա�ɾ��ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 12:
                //printf("\n----ListTraverse���ܴ�ʵ�֣�\n");
                if(!ListTraverse(L, visit)) printf("���Ա��ǿձ�\n");
                getchar();getchar();
                break;
                //-2 1 -3 4 -1 2 1 -5 4 
            case 13:
                //printf("\n----MaxSubArray���ܴ�ʵ�֣�\n");
                if(!L.elem) printf("���Ա�δ������\n");
                else if (ListEmpty(L)) printf("���Ա�Ϊ�գ�\n");
                else printf("���������֮��Ϊ��%d\n",MaxSubArray(L));
                getchar();getchar();
                break;
                //6 
            case 14:
                //printf("\n----SubArrayNum���ܴ�ʵ�֣�\n");
                if(!L.elem){printf("���Ա�δ������\n");getchar();getchar();break;}
                else if (ListEmpty(L)) {printf("���Ա�Ϊ�գ�\n");getchar();getchar();break;}
                printf("������Ѱ�ҵ���������ĺͣ�");
                scanf("%d",&flag);
                printf("��Ϊ��%d������������ĿΪ��%d\n",flag,SubArrayNum(L,flag));
                getchar();getchar();
                break;
                //3 5
                //5 2
                //15 0
            case 15:
                //printf("\n----sortList���ܴ�ʵ�֣�\n");
                flag = sortList(L);
                if(!flag) printf("���Ա��ǿձ�\n");
                else if(flag != INFEASIBLE) printf("���Ա�����ɹ���\n");
                getchar();getchar();
                break;
            case 16:
                //printf("\n----SaveList���ܴ�ʵ�֣�\n");
                printf("������Ҫ������ļ����ƣ�");
                scanf("%s",FileName);
                flag = SaveList(L, FileName);
                if(flag == INFEASIBLE) printf("���Ա�����!�ļ�����ʧ�ܣ�\n");
                else if(flag == ERROR);
                else printf("�ļ�����ɹ���\n");
                getchar();getchar();
                break;
            case 17:
                //printf("\n----LoadList���ܴ�ʵ�֣�\n");
                printf("������Ҫ¼����ļ����ƣ�");
                scanf("%s",FileName);
                if(LoadList(L, FileName) == INFEASIBLE) printf("���Ա����!�ļ�¼��ʧ�ܣ�\n");
                else printf("�ļ�¼��ɹ���\n");
                getchar();getchar();
                break;
            case 18:
                //printf("\n----AddList���ܴ�ʵ�֣�\n");
                if(Lists.length == MAXlength) {
                	printf("�����Ա���������������ĳЩ���Ա���ٲ�����\n");
                	getchar();getchar();
                	break;
				}
                printf("�������������Ա�����ƣ�");
                scanf("%s",Name);
                flag = AddList(Lists, Name);
                if(flag == INFEASIBLE) printf("�����Ƶ����Ա��Ѿ����ڣ�\n");
                else printf("%s�ѳɹ���ӣ�\n",Name);
                getchar();getchar();
                break;
            case 19:
                //printf("\n---RemoveList���ܴ�ʵ�֣�\n");
                if(Lists.length == 0) {
					printf("�����Ա�����ѿգ������ĳЩ���Ա���ٲ�����\n");
					getchar();getchar();
                	break;
				}
				printf("������ɾ�����Ա�����ƣ�");
                scanf("%s",Name);
                flag = RemoveList(Lists, Name);
                if(flag == OK)printf("%s�ѳɹ�ɾ����\n",Name);
                else printf("���Ա����ڣ�\n"); 
                getchar();getchar();
                break;
            case 20:
                //printf("\n---LocateList���ܴ�ʵ�֣�\n");
                printf("������������Ա�����ƣ�");
                scanf("%s",Name);
                if(LocateList(Lists, Name)) printf("�����Ա���߼�����Ϊ��%d\n", LocateList(Lists, Name));
                else printf("���Ա����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 21:
                //printf("\n----TraverseList���ܴ�ʵ�֣�\n");
                if(TraverseList(Lists) == INFEASIBLE) printf("�����Ա�Ϊ�գ�\n");
                getchar();getchar();
                break;
            case 22:
                //printf("\n----SelectList���ܴ�ʵ�֣�\n");
                printf("��ѡ��Ҫ��������Ա���߼�������");
                scanf("%d",&flag);
                if(SelectList(Lists, flag) == OK) {
                	printf("���Ա���ѡȡ�ɹ���\n");
                	num = flag;
				}
                else printf("���Ա�ѡȡʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 0: 
                break;
        }//end of switch
    }//end of while
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
    return 0;
}//end of main()

