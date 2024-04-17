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
typedef int ElemType; //����Ԫ�����Ͷ���
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
#define MAXlength 10
typedef struct LNode{  //��������ʽ�ṹ�����Ķ���
    ElemType data;
    struct LNode *next;
}LNode,*LinkList;

LinkList L;

typedef struct{  //���Ա�ļ������Ͷ���
    struct { char name[30];
        LinkList L;
    } elem[11];
    int length;
}LISTS;
LISTS Lists;      //���Ա��ϵĶ���Lists

status InitList(LinkList &L)
// ���Ա�L�����ڣ�����һ���յ����Ա�����OK�����򷵻�INFEASIBLE��
{
    if(L) return INFEASIBLE;
    L = (LinkList)malloc(sizeof(LinkList));
    L->next = NULL;
    return OK;

}

status DestroyList(LinkList &L)
// ������Ա�L���ڣ��������Ա�L���ͷ�����Ԫ�صĿռ䣬����OK�����򷵻�INFEASIBLE��
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
// ������Ա�L���ڣ�ɾ�����Ա�L�е�����Ԫ�أ�����OK�����򷵻�INFEASIBLE��
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
// ������Ա�L���ڣ��ж����Ա�L�Ƿ�Ϊ�գ��վͷ���TRUE�����򷵻�FALSE��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(!L) return INFEASIBLE;
    if(L->next) return FALSE;
    else return TRUE;

}


int ListLength(LinkList L)
// ������Ա�L���ڣ��������Ա�L�ĳ��ȣ����򷵻�INFEASIBLE��
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
// ������Ա�L���ڣ���ȡ���Ա�L�ĵ�i��Ԫ�أ�������e�У�����OK�����i���Ϸ�������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ�����Ԫ��e�����Ա�L�е�λ����ţ����e�����ڣ�����ERROR�������Ա�L������ʱ������INFEASIBLE��
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
// ������Ա�L���ڣ���ȡ���Ա�L��Ԫ��e��ǰ����������pre�У�����OK�����û��ǰ��������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ���ȡ���Ա�LԪ��e�ĺ�̣�������next�У�����OK�����û�к�̣�����ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ���Ԫ��e���뵽���Ա�L�ĵ�i��Ԫ��֮ǰ������OK��������λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ�ɾ�����Ա�L�ĵ�i��Ԫ�أ���������e�У�����OK����ɾ��λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ�������ʾ���Ա��е�Ԫ�أ�ÿ��Ԫ�ؼ��һ�񣬷���OK��������Ա�L�����ڣ�����INFEASIBLE��
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
// �������Ա��е���������͵����ֵ
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
// �������Ա������������Ϊk������������Ŀ
    int sum = 0, count = 0;
    int l[LIST_INIT_SIZE];
    l[0] = 0;
    int h;
    LinkList p = L->next;
    //��ȡǰ׺��
    for(h = 0; p; p = p->next, h++){
        sum += p->data;
        l[h + 1] = sum;
    }
    //ͨ��ǰ׺��֮��Ĳ�ֵ�������������
    for(int i = 0; i < h; i++)
        for(int j = i + 1; j < h + 1; j++)
            if(l[j] - l[i] == k)
                count++;
    return count;

}

status sortList(LinkList &L){
//ð�ݷ����е���������
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
// ������Ա�L���ڣ������Ա�L�ĵ�Ԫ��д��FileName�ļ��У�����OK�����򷵻�INFEASIBLE��
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
        printf("���ļ����ܶ��룡\n");
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
// ������Ա�L�����ڣ���FileName�ļ��е����ݶ��뵽���Ա�L�У�����OK�����򷵻�INFEASIBLE��
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
// ֻ��Ҫ��Lists������һ������ΪListName�Ŀ����Ա����Ա������ֺ�̨���Գ�����롣
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
// Lists��ɾ��һ������ΪListName�����Ա�
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

status reverseList(LinkList &L){
//������ת˼���ͷ��ʼ���η�ת����
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
//�Ƴ�������n��Ԫ�� 
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
        printf("    	  1. InitList    ���Ա��ʼ��         13. MaxSubArray      ���Ա������������ͻ�ȡ\n");
        printf("    	  2. DestroyList ���Ա�����           14. SubArrayNum      ���Ա�ָ�������������Ŀ\n");
        printf("    	  3. ClearList   ���Ա����           15. sortList         ���Ա�����\n");
        printf("    	  4. ListEmpty   ���Ա��п�           16. SaveList         ���Ա��ļ�����\n");
        printf("    	  5. ListLength  ���Ա��ȡ����       17. LoadList         ���Ա��ļ�¼��\n");
        printf("    	  6. GetElem     ���Ա�Ԫ�ػ�ȡ       18. AddList          �����Ա����\n");
        printf("          7. LocateElem  ���Ա�Ԫ�ز���       19. RemoveList       �����Ա�ɾ��\n");
        printf("          8. PriorElem   ���Ա�Ԫ��ǰ����ȡ   20. LocateList       �����Ա�λ�ò���\n");
        printf("          9. NextElem    ���Ա�Ԫ�غ�̻�ȡ   21. TraverseList     �����Ա����\n");
        printf("          10. ListInsert ���Ա�Ԫ�ز���       22. SelectList       ���Ա����ѡ��\n");
        printf("          11. ListDelete ���Ա�Ԫ��ɾ��       23. reverseList      ���Ա�ת\n");
        printf("          12. ListTraverse   ���Ա����       24. RemoveNthFromEnd �Ƴ�����Ԫ��\n");
        printf("    	  0. Exit        �˳�\n");
        printf("          ���ӹ��ܣ�25. ���Ա�Ѹ�����룡\n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("          ˵����ÿ�β������������ո�ȷ�ϲ��ܽ�����һ��������\n");
        printf("\n          ��ǰ���������Ա�Ϊ��");
        if(num < 1|| num > Lists.length){
            if(num > Lists.length){
                L = NULL;
                num = 0;
            }
            printf("Ĭ�����Ա�");
            if(!L)
                printf("(δ����)");
            printf("\n\n\n");
        }
        else{
        	printf("%s",Lists.elem[num - 1].name);
        	if(!L)
                printf("(δ����)");
            printf("\n\n\n");
		}
            
        if(op > 25 || op < 0)
            printf("��һ�������������ݲ˵���ȷ���룡\n\n\n");
        printf("��ѡ����Ĳ���[0~22]:");
        scanf("%d",&op);
        switch(op){
        	case 25:
				if(!L) {
					printf("���Ա�δ������\n"); 
					getchar();getchar();
                	break;
				}
				printf("�����룺"); 
				CreatList(L);
				getchar();getchar();
                break;
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
                flag = ClearList(L); 
                if(flag == OK) printf("���Ա���ճɹ���\n");
                else if (flag == ERROR) printf("���Ա����ʧ�ܣ�\n");
                else printf("���Ա�δ������\n");
                getchar();getchar();
                break;
            case 4:
                //printf("\n----ListEmpty���ܴ�ʵ�֣�\n");
                if(ListEmpty(L) == OK) printf("���Ա�Ϊ�գ�\n");
                else if(ListEmpty(L) == INFEASIBLE) printf("���Ա�δ������\n");
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
                else if(flag == OK) printf("��Ԫ��Ϊ��%d\n", y);
                else printf("λ�ò��Ϸ���\n");
                getchar();getchar();
                break;
            case 7:
                //printf("\n----LocateElem���ܴ�ʵ�֣�\n");
                int a;
                printf("��������Ҫ���ҵ�Ԫ�أ�");
                scanf("%d",&a);
                flag = LocateElem(L, a);
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
                else if(flag == ERROR)printf("��Ԫ�ز����ڣ�\n");
                else printf("��Ԫ�ز�����ǰ����\n");
                getchar();getchar();
                break;
            case 9:
                //printf("\n----NextElem���ܴ�ʵ�֣�\n");
                printf("��������Ҫ���ҵ�Ԫ��(��ȡ���)��");
                scanf("%d",&a);
                flag = NextElem(L, a, temp);
                if(flag == INFEASIBLE) printf("���Ա�δ����!\n");
                else if(flag == OK) printf("��Ԫ�ش����Һ��Ԫ��Ϊ��%d\n", temp);
                else if(flag == ERROR) printf("��Ԫ�ز����ڣ�\n");
                else printf("��Ԫ�ز����ں�̣�\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----ListInsert���ܴ�ʵ�֣�\n");
                int i, e;
                printf("������Ҫ�����Ԫ��λ�ã�");
                scanf("%d",&i);
                printf("������Ҫ�����Ԫ�أ�");
                scanf("%d",&e);
                if(ListInsert(L, i, e) == OK) printf("���Ա����ɹ���\n");
                else printf("���Ա����ʧ�ܣ�\n");
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
                if(!ListTraverse(L)) printf("���Ա��ǿձ�\n");
                getchar();getchar();
                break;
            case 13:
                //printf("\n----MaxSubArray���ܴ�ʵ�֣�\n");
                if(!L) printf("���Ա�δ������\n");
                else if (ListEmpty(L)) printf("���Ա�Ϊ�գ�\n");
                else printf("���������֮��Ϊ��%d\n",MaxSubArray(L));
                getchar();getchar();
                break;
            case 14:
                //printf("\n----SubArrayNum���ܴ�ʵ�֣�\n");
                if(!L){printf("���Ա�δ������\n");getchar();getchar();break;}
                else if (ListEmpty(L)) {printf("���Ա�Ϊ�գ�\n");getchar();getchar();break;}
                printf("������Ѱ�ҵ���������ĺͣ�");
                scanf("%d",&flag);
                printf("��Ϊ��%d������������ĿΪ��%d\n",flag,SubArrayNum(L,flag));
                getchar();getchar();
                break;
            case 15:
                //printf("\n----sortList���ܴ�ʵ�֣�\n");
                flag = sortList(L);
                if(flag == ERROR) printf("���Ա��ǿձ�\n");
                else if(flag == INFEASIBLE) printf("���Ա�δ������\n");
                else printf("���Ա�����ɹ���\n");
                getchar();getchar();
                break;
            case 16:
                //printf("\n----SaveList���ܴ�ʵ�֣�\n");
                printf("������Ҫ������ļ����ƣ�");
                scanf("%s",FileName);
                flag = SaveList(L, FileName);
                if(flag == INFEASIBLE) printf("�ļ�����ʧ�ܣ�\n");
                else if(flag == ERROR);
                else printf("�ļ�����ɹ���\n");
                getchar();getchar();
                break;
            case 17:
                //printf("\n----LoadList���ܴ�ʵ�֣�\n");
                printf("������Ҫ¼����ļ����ƣ�");
                scanf("%s",FileName);
                if(LoadList(L, FileName) == INFEASIBLE) printf("�ļ�¼��ʧ�ܣ�\n");
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
                    printf("��ѡȡ�ɹ���\n");
                    num = flag;
                }
                else printf("ѡȡʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 23:
                //printf("\n----reverseList���ܴ�ʵ�֣�\n");
                flag = reverseList(L);
                if(flag == INFEASIBLE) printf("���Ա�δ������\n");
                else if(flag == ERROR) printf("�����Ա�Ϊ�գ�\n");
                else printf("����ת�ɹ���\n");
                getchar();getchar();
                break;
            case 24://printf("\n----RemoveNthFromEnd���ܴ�ʵ�֣�\n");
                int n;
                printf("��������Ҫɾ�������ڼ���Ԫ�أ�");
                scanf("%d",&n);
                flag = RemoveNthFromEnd(L, n);
                if(flag == INFEASIBLE) printf("���Ա�δ������\n");
                else if(flag == ERROR) printf("λ�ò��Ϸ���\n");
                else printf("ɾ���ɹ���\n");
                getchar();getchar();
                break;
            case 0:
                break;
        }//end of switch
    }//end of while
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
    return 0;
}//end of main()

