#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXlength 10

typedef int status;
typedef int KeyType;
typedef struct {
    KeyType  key;
    char others[20];
} TElemType; //������������Ͷ���

typedef struct BiTNode{  //����������Ķ���
    TElemType  data;
    struct BiTNode *lchild,*rchild;
} BiTNode, *BiTree;

typedef struct{  //���Ա�ļ������Ͷ���
    struct { char name[30];
        BiTree L;
    } elem[11];
    int length;
}TREES;
TREES trees;      //���Ա��ϵĶ���TREES

BiTree T;
int result = 0, count = 0;
BiTree BiTreestack[100];
int top;
BiTree BiTreequeue[100];
int l, r;
int i, k, flag1[1000], flag2 = 0;

status checkKey(TElemType definition[]) {
    i = 0;
    while(definition[i++].key != -1);
    for (int j = 0; j < i; j++) {
        for (int k = j + 1; k < i; k++) {
            if (definition[k].key == definition[j].key && definition[k].key != 0)
                return 0;
        }
    }
    return 1;
}

status CreateBiTree(BiTree& T, TElemType definition[]) {
//�������Ĵ�����ͨ����������ķ�ʽ����
    if(result == 0) result = checkKey(definition);
    if(result == 0) return ERROR;
    if(definition[count].key != -1) {
        if(definition[count].key != 0) {
            T = (BiTree)malloc(sizeof(BiTNode));
            T->data.key = definition[count].key;
            strcpy(T->data.others, definition[count].others);
            count++;
            CreateBiTree(T->lchild, definition);
            CreateBiTree(T->rchild, definition);
        }
        else {
            T = NULL;
            count++;
        }
    }
    return OK;

}

status ClearBiTree(BiTree &T)
//�����������óɿգ���ɾ�����н�㣬�ͷŽ��ռ�
{
    if(!T) return OK;
    ClearBiTree(T->lchild);
    ClearBiTree(T->rchild);
    free(T);
    T = NULL;
    return OK;

}

status DestroyBiTree(BiTree &T)
//������������
{
    if(!T) return OK;
    DestroyBiTree(T->lchild);
    DestroyBiTree(T->rchild);
    free(T);
    T = NULL;
    return OK;

}

int MAX(int a, int b){
    if(a >= b) return a;
    else return b;
}

int BiTreeDepth(BiTree T)
//�������T�����
{
    if(!T) return  ERROR;
    return MAX(BiTreeDepth(T->lchild), BiTreeDepth(T->rchild)) + 1;

}



BiTNode* LocateNode(BiTree T,KeyType e)
//���ҽ��
{
    if(!T) return NULL;
    if(T->data.key == e) return T;
    if(LocateNode(T->lchild, e))
        return LocateNode(T->lchild, e);
    else
        return LocateNode(T->rchild, e);

}

status Assign(BiTree &T,KeyType e,TElemType value)
//ʵ�ֽ�㸳ֵ��
{
    BiTree p = LocateNode(T, value.key);
    BiTree t = LocateNode(T, e);
    if(t == NULL || (value.key!= e && p != NULL)) return ERROR;
    t->data.key = value.key;
    strcpy(t->data.others, value.others);
    return OK;

}

BiTNode* GetSibling(BiTree T,KeyType e)
//ʵ�ֻ���ֵܽ��
{
    if(!T) return NULL;
    if(T->lchild && T->lchild->data.key == e) return T->rchild;
    if(T->rchild && T->rchild->data.key == e) return T->lchild;
    if(GetSibling(T->lchild, e))
        return GetSibling(T->lchild, e);
    else
        return GetSibling(T->rchild, e);

}

BiTNode* GetFabling(BiTree T,KeyType e)
//ʵ�ֻ�ø��׽��
{
    if(!T) return NULL;
    if(T->lchild && T->lchild->data.key == e) return T;
    if(T->rchild && T->rchild->data.key == e) return T;
    if(GetFabling(T->lchild, e))
        return GetFabling(T->lchild, e);
    else
        return GetFabling(T->rchild, e);

}

status InsertNode(BiTree &T,KeyType e,int LR,TElemType c)
//�����㡣
{
    if(!T) return INFEASIBLE;
    BiTree t = LocateNode(T, e);
    BiTree p = (BiTree)malloc(sizeof(BiTNode));
    p->data.key = c.key;
    strcpy(p->data.others, c.others);
    if(LocateNode(T, c.key)) return ERROR;
    if(LR == -1){
        p->rchild = T;
        p->lchild = NULL;
        T = p;
        return OK;
    }
    if(!t) return ERROR;
    if(LR == 0){
        p->rchild = t->lchild;
        p->lchild = NULL;
        t->lchild = p;
        return OK;
    }
    else if(LR == 1){
        p->rchild = t->rchild;
        p->lchild = NULL;
        t->rchild = p;
        return OK;
    }
    free(p);

}

status DeleteNode(BiTree &T,KeyType e)
//ɾ����㡣
{
    BiTree tmp = (BiTree)malloc(sizeof(BiTNode));
    BiTree t = NULL, p = NULL, q = NULL, l = NULL;
    int flag = 0;
    tmp->lchild = T;
    tmp->rchild = NULL;
    t = GetFabling(tmp, e);
    // printf("%d",t->data.key);
    if(!t) return ERROR;
    if(t->lchild && t->lchild->data.key == e) {q = t->lchild; flag = 1;}
    else if(t->rchild && t->rchild->data.key == e) {q = t->rchild; flag = 0;}
    if(!q->lchild && !q->rchild){
        free(q);
        if(flag) t->lchild = NULL;
        else t->rchild = NULL;
    }
    else if(!q->lchild && q->rchild){
        if(flag) t->lchild = q->rchild;
        else t->rchild = q->rchild;
        free(q);
    }
    else if(q->lchild && !q->rchild){
        if(flag) t->lchild = q->lchild;
        else t->rchild = q->lchild;
        free(q);
    }
    else{
        l = q->lchild;
        while(1){
            if(l){
                BiTreestack[top++] = l;
                l = l->lchild;
            }
            else{
                l = BiTreestack[--top];
                if(!l->rchild && !top)
                    break;
                l = l->rchild;
            }
        }
        l->rchild = q->rchild;
        if(flag) t->lchild = q->lchild;
        else t->rchild = q->lchild;
        free(q);
    }
    T = tmp->lchild;
    free(tmp);
    return OK;

}

void visit(BiTree T)
{
    printf(" %d,%s",T->data.key,T->data.others);
}

status PreOrderTraverse(BiTree T,void (*visit)(BiTree))
//�������������T
{
    if(!T) return INFEASIBLE;
    visit(T);
    PreOrderTraverse(T->lchild, visit);
    PreOrderTraverse(T->rchild, visit);
    return OK;

}

status InOrderTraverse(BiTree T,void (*visit)(BiTree))
//�������������T
{
    if(!T) return INFEASIBLE;
    BiTree p = T;
    while(p || top){
        if(p){
            BiTreestack[top++] = p;
            p = p->lchild;
        }
        else{
            p = BiTreestack[--top];
            visit(p);
            p = p->rchild;
        }
    }

}

status PostOrderTraverse(BiTree T,void (*visit)(BiTree))
//�������������T
{
    if(!T) return ERROR;
    PostOrderTraverse(T->lchild, visit);
    PostOrderTraverse(T->rchild, visit);
    visit(T);
    return OK;

}

status LevelOrderTraverse(BiTree T,void (*visit)(BiTree))
//�������������T
{
    BiTree p = T;
    BiTreequeue[0] = p;
    l = 0, r = 1;
    while(l != r){
        p = BiTreequeue[l++];
        visit(p);
        if(p->lchild) BiTreequeue[r++] = p->lchild;
        if(p->rchild) BiTreequeue[r++] = p->rchild;
    }
    return OK;

}

status SaveBiTree(BiTree T, char FileName[])
//���������Ľ������д�뵽�ļ�FileName��
{
    if(!T) return INFEASIBLE;
    FILE *fp;
    char ch;
    if ((fp = fopen(FileName,"wb")) == NULL)
    {
        printf("File open error!\n ");
        return ERROR;
    }
    ch = fgetc(fp);
    if(ch != EOF){
        printf("���ļ����ܶ��룡\n");
        return ERROR;
    }
    TElemType t;
    t.key = 0;
    BiTree s;
    BiTreestack[top++] = T;
    while(top)
    {
        s = BiTreestack[--top];
        if(!s)
        {
            fwrite(&t, sizeof(TElemType), 1, fp);
            continue;
        }
        BiTreestack[top++] = s->rchild;
        BiTreestack[top++] = s->lchild;
        fwrite(&s->data, sizeof(TElemType), 1, fp);
    }
    fclose(fp);
    return OK;

}

status dfs(BiTree &T, TElemType definition[])
{
    i++;
    if(definition[i].key == -1) return OK;
    if(definition[i].key == 0) T = NULL;
    else
    {
        T = (BiTNode*)malloc(sizeof(BiTNode));
        T->data = definition[i];
        if(flag1[definition[i].key]) flag2 = 1;
        flag1[T->data.key] = 1;
        dfs(T->lchild, definition);
        dfs(T->rchild, definition);
    }
    return OK;
}

status LoadBiTree(BiTree &T,  char FileName[])
//�����ļ�FileName�Ľ�����ݣ�����������
{
    if(T) return INFEASIBLE;
    FILE *fp;
    if ((fp = fopen(FileName,"rb")) == NULL)
    {
        printf("File open error!\n ");
        return ERROR;
    }
    i = 0;
    TElemType definition[100];
    while(fread(&definition[i++], sizeof(TElemType), 1, fp));
    definition[i].key = -1;
    i = -1;
    dfs(T, definition);
    fclose(fp);
    if(flag2) return ERROR;
    return OK;
  
}

status BiTreeEmpty(BiTree T){
    if(!T) return FALSE;
    else return TRUE;
}

status AddList(TREES &trees,char ListName[])
// ��Ҫ��TREES������һ������ΪListName�Ŀ����Ա�
{
	for(int i = 0; i < trees.length; i++){
		if(!strcmp(trees.elem[i].name,ListName)) return INFEASIBLE;
	}
    strcpy(trees.elem[trees.length].name,ListName);
    trees.elem[trees.length].L = NULL;
    trees.length++;
    return OK;
}

status DestoryList(TREES &trees,char ListName[])
// TREES��ɾ��һ������ΪListName�����Ա�
{
    for(int i = 0; i < trees.length; i++)
        if(!strcmp(ListName, trees.elem[i].name)){
            if(trees.elem[i].L)
                DestroyBiTree(trees.elem[i].L);
            for (int j = i; j < trees.length - 1; j++)
                trees.elem[j] = trees.elem[j + 1];
            trees.length--;
            return OK;
        }
    return ERROR;

}

int LocateList(TREES trees,char ListName[])
// ��TREES�в���һ������ΪListName�����Ա��ɹ������߼���ţ����򷵻�0
{
    if(!trees.elem) return INFEASIBLE;//����δ���
    for(int i = 0; i < trees.length; i++)
        if(!strcmp(ListName, trees.elem[i].name))
            return i + 1;
    return 0;

}

status TraverseList(TREES trees){
// ��������Ա�Ϊ�գ�������ʾ�����Ա�����ƣ�ÿ�����Ƽ��һ�񣬷���OK����������Ա�Ϊ�գ�����INFEASIBLE��
    if(trees.length == 0) return INFEASIBLE;
    printf("\n-----------all names -----------------------\n");
    for(int i = 0; i < trees.length; i++){
        printf("%s",trees.elem[i].name);
        if(i != trees.length - 1) printf(" ");
    }
    printf("\n------------------ end ------------------------\n");
    return OK;
}

status SelectList(TREES trees, int i){
// �������Ա��ѡ��
    if(trees.length == 0) return INFEASIBLE;
    if(i < 1 || i > trees.length) return ERROR;
    T = trees.elem[i - 1].L;
    return OK;
}

int MaxPathSum(BiTree T){
//�������·����	
    if(!T) return  ERROR;
    return MAX(MaxPathSum(T->lchild), MaxPathSum(T->rchild)) + T->data.key;
}

BiTree LowestCommonAncestor(BiTree T, int e1, int e2){
//���ع�������	
    if(!T || T->data.key == e1 || T->data.key == e2) return T;
    BiTree l = LowestCommonAncestor(T->lchild, e1, e2);
    BiTree r = LowestCommonAncestor(T->rchild, e1, e2);
    if(l && !r) return l;
    if(!l && r) return r;
    if(!l && !r) return NULL;
    if(l && r) return T;
}

status InvertTree(BiTree &T){
//��������ת	
    BiTree p = T, t;
    BiTreequeue[0] = p;
    l = 0, r = 1;
    while(l != r){
        p = BiTreequeue[l++];
        t = p->lchild;
        p->lchild = p->rchild;
        p->rchild = t;
        if(p->lchild) BiTreequeue[r++] = p->lchild;
        if(p->rchild) BiTreequeue[r++] = p->rchild;
    }
    return OK;
}

status ClearList(BiTree &T){
	ClearBiTree(T);
	return OK;
}

int main(){
    int op=1;
    int length, num = 0, LR;
    int ans, e, ee;
    char FileName[100];
    char Name[20];
    TElemType definition[100], value;
    BiTree t = NULL;
    trees.length = 0;
    while(op){
        system("cls");
        printf("\n\n");
        printf("                                    Menu for Binary Tree  \n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("    	  1. CreateBiTree  ����������                 14. LevelOrderTraverse    �������������\n");
        printf("    	  2. DestroyBiTree ����������                 15. MaxPathSum            ���������·����\n");
        printf("    	  3. ClearBiTree   ���������                 16. LowestCommonAncestor  �������������\n");
        printf("    	  4. BiTreeEmpty   �������п�                 17. InvertTree            ��������ת\n");
        printf("    	  5. BiTreeDepth   ��������ȡ���             18. SaveList              �������ļ�����\n");
        printf("    	  6. LocateNode    ���������ҽ��             19. LoadList              �������ļ�¼��\n");
        printf("          7. Assign        ��������㸳ֵ             20. AddList               ������������\n");
        printf("          8. GetFabling    ��������ȡ���׽��         21. DestroyList           �������������\n");
        printf("          9. InsertNode    ������������             22. LocateList            ���������λ�ò���\n");
        printf("          10. DeleteNode   ������ɾ�����             23. TraverseList          ������������\n");
        printf("          11. PreOrderTraverse   ������ǰ�����       24. SelectList            ����������ѡ��\n");
        printf("          12. InOrderTraverse    �������������       25. ClearList             ������������\n");
        printf("          13. PostOrderTraverse  �������������       26. GetSibling    ��������ȡ�ֵܽ��\n"); 
        printf("    	  0. Exit        �˳�\n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("          ˵����ÿ�β������������ո�ȷ�ϲ��ܽ�����һ��������\n");
        printf("\n          ��ǰ�����Ķ�����Ϊ��");
        if(num < 1|| num > trees.length){
            if(num > trees.length){
                T = NULL;
                num = 0;
            }
            printf("Ĭ�϶�����");
            if(!T)
                printf("(δ����)");
            printf("\n\n\n");
        }
        else{
        	printf("%s",trees.elem[num - 1].name);
        	if(!T)
                printf("(δ����)");
            printf("\n\n\n");
		}   
        if(op > 26 || op < 0)
            printf("��һ�������������ݲ˵���ȷ���룡\n\n\n");
        printf("��ѡ����Ĳ���[0~26]:");
        scanf("%d",&op);
        switch(op){
            case 1:
                //printf("\n----CreateBiTree���ܴ�ʵ�֣�\n");
                if(T){
                    printf("�ö������Ѵ��ڣ�\n");
                    getchar();getchar();
                    break;
                }
                i = 0;
                printf("������Ϸ��������У�ÿ������Ӧһ�����͵Ĺؼ��ֺ�һ���ַ��������ؼ���Ϊ0ʱ����ʾ��������Ϊ-1��ʾ�����������");
                do {
                    scanf("%d%s", &definition[i].key, definition[i].others);
                } while (definition[i++].key != -1);
                ans = CreateBiTree(T,definition);
                count = 0;
                if(ans == ERROR) printf("�ؼ��ֲ�Ψһ������ʧ�ܣ�\n");
                else printf("�����������ɹ���\n");
                getchar();getchar();
                break;
            case 2:
                //printf("\n----DestroyBiTree���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                ans = DestroyBiTree(T);
                if(ans == OK) printf("���������ٳɹ���\n");
                else printf("����������ʧ�ܣ�\n");
                getchar();getchar();
                break;    
            case 3:
                //printf("\n----ClearBiTree���ܴ�ʵ�֣�\n");
                ans = ClearBiTree(T);
                if(ans == OK) printf("��������ճɹ���\n");
                else printf("���������ʧ�ܣ�\n");
                getchar();getchar();
                break;
           	case 4:
                //printf("\n----BiTreeEmpty���ܴ�ʵ�֣�\n");
                ans = BiTreeEmpty(T);
                if(ans == FALSE) printf("������Ϊ�գ�\n");
                else printf("��������Ϊ�գ�\n");
                getchar();getchar();
                break;
            case 5:
                //printf("\n----BiTreeDepth���ܴ�ʵ�֣�\n");
                length = BiTreeDepth(T);
                if(length) printf("�ö����������Ϊ%d��\n", length);
                else printf("������Ϊ�գ�\n");
                getchar();getchar();
                break;
            case 6:
                //printf("\n----LocateNode���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������Ҫ��ѯ�Ľ��ؼ��֣�");
                scanf("%d",&e);
                t = LocateNode(T, e);
                if(t == NULL) printf("�ýڵ㲻���ڣ�\n");
                else {
                    printf("�ýڵ���ڣ������ϢΪ��%s\n",t->data.others);
                }
                t = NULL;
                getchar();getchar();
                break;
            case 7:
                //printf("\n----Assign���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������Ҫ��ֵ�Ľڵ�ؼ���:");
                scanf("%d", &e);
                printf("������ؼ��֣�");
                scanf("%d", &value.key);
                printf("����������Ϣ��");
                scanf("%s", value.others);
                ans = Assign(T, e, value);
                if(ans == OK) printf("��㸳ֵ�ɹ�!\n");
                else printf("��㸴��ʧ�ܣ�����ùؼ����Ƿ���ڻ��߸�ֵ�ؼ����Ƿ��ظ���!\n");
                getchar();getchar();
                break;
            case 8:
                //printf("\n----GetSibling���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("������Ҫ��ȡ�ֵܽ��Ĺؼ��֣�");
                scanf("%d",&e);
                t = GetSibling(T, e);
                if(t == NULL) printf("�ý�㲻���ڻ򲻴����ֵܽ��!\n");
                else{
                    printf("��Ԫ���ֵܽ���ȡ�ɹ���\n");
                    printf("�ý����ֵܽ��ؼ���Ϊ %d �����ϢΪ�� %s\n", t->data.key, t->data.others);
                }
                getchar();getchar();
                break;
            case 9:
                //printf("\n----InsertNode���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������Ҫ����Ľ��ؼ��֣�");
                scanf("%d", &e);
                printf("������ؼ��֣�");
                scanf("%d", &value.key);
                printf("����������Ϣ��");
                scanf("%s", value.others);
                printf("��������뷽ʽ��LRΪ0����1ʱ��Ϊ�ؼ���Ϊe�Ľ�������Һ��ӽ�㣬LRΪ-1ʱ����Ϊ�������룬ԭ�������Ϊc������������");
                scanf("%d", &LR);
                ans = InsertNode(T, e, LR, value);
                if(ans == ERROR) printf("������ʧ�ܣ�����ùؼ����Ƿ���ڻ��߲���ؼ����Ƿ��ظ���!\n");
                else printf("������ɹ���\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----DeleteNode���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������Ҫɾ���Ľ��ؼ��֣�");
                scanf("%d", &e);
                ans = DeleteNode(T, e);
                if(ans == ERROR) printf("���ɾ��ʧ�ܣ�����ùؼ����Ƿ���ڣ�!\n");
                else printf("���ɾ���ɹ���\n");
                getchar();getchar();
                break;
            case 11:
                //printf("\n----PreOrderTraverse���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������������Ľ����\n");
                PreOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 12:
                //printf("\n----InOrderTraverse���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������������Ľ����\n");
                InOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 13:
                //printf("\n----PostOrderTraverse���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������������Ľ����\n");
                PostOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 14:
                //printf("\n----LevelOrderTraverse���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������������Ľ����\n");
                LevelOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 15:
                //printf("\n----MaxPathSum���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                length = MaxPathSum(T);
                printf("���ڵ㵽Ҷ�ӽ������·����Ϊ��%d\n", length);
                getchar();getchar();
                break;
            case 16:
                //printf("\n----LowestCommonAncestor���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("�������һ����㣺");
                scanf("%d", &e);
                printf("������ڶ�����㣺");
                scanf("%d", &ee);
                t = LocateNode(T, e);
                if(!t){
                    printf("��һ����㲻���ڣ�\n");
                    getchar();getchar();
                    break;
                }
//                else if(t == T){
//                    printf("��һ�����Ϊ���ڵ㣬���������ȣ�\n");
//                    t = NULL;
//                    getchar();getchar();
//                    break;
//                }
                t = LocateNode(T, ee);
                if(!t){
                    printf("�ڶ�����㲻���ڣ�\n");
                    getchar();getchar();
                    break;
                }
//                else if(t == T){
//                    printf("�ڶ������Ϊ���ڵ㣬���������ȣ�\n");
//                    t = NULL;
//                    getchar();getchar();
//                    break;
//                }
                t = LowestCommonAncestor(T,e,ee);
                printf("���������������ȵĹؼ���Ϊ��%d�� �����ϢΪ��%s\n", t->data.key, t->data.others);
                t = NULL;
                getchar();getchar();
                break;
            case 17:
                //printf("\n----InvertTree���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                InvertTree(T);
                printf("��������ת�ɹ���\n");
                getchar();getchar();
                break;
            case 18:
                //printf("\n----SaveList���ܴ�ʵ�֣�\n");
                printf("������Ҫ������ļ����ƣ�");
                scanf("%s",FileName);
                ans = SaveBiTree(T, FileName);
                if(ans == INFEASIBLE) printf("������������!�ļ�����ʧ�ܣ�\n");
                else if(ans == ERROR);
                else printf("�ļ�����ɹ���\n");
                getchar();getchar();
                break;
            case 19:
                //printf("\n----LoadList���ܴ�ʵ�֣�\n");
                printf("������Ҫ¼����ļ����ƣ�");
                scanf("%s", FileName);
                if(LoadBiTree(T, FileName) == INFEASIBLE) printf("����������!�ļ�¼��ʧ��\n");
                else printf("�ļ�¼��ɹ���\n");
                getchar();getchar();
                break;
            case 20:
                //printf("\n----AddList���ܴ�ʵ�֣�\n");
                if(trees.length == MAXlength) {
                    printf("�����������������������ĳЩ���������ٲ�����\n");
                    getchar();getchar();
                    break;
                }
                printf("���������������������ƣ�");
                scanf("%s",Name);
                ans = AddList(trees, Name);
                if(ans == INFEASIBLE) printf("�����ƵĶ������Ѿ�����!\n"); 
                else printf("%s�ѳɹ���ӣ�\n",Name);
                getchar();getchar();
                break;
            case 21:
                //printf("\n---DestoryList���ܴ�ʵ�֣�\n");
                if(trees.length == 0) {
                    printf("�������������ѿգ������ĳЩ���������ٲ�����\n");
                    getchar();getchar();
                    break;
                }
                printf("���������ٶ����������ƣ�");
                scanf("%s",Name);
                ans = DestoryList(trees, Name);
                if(ans == OK)printf("%s�ѳɹ����٣�\n",Name);
                else printf("�����������ڣ�\n");
                getchar();getchar();
                break;
            case 22:
                //printf("\n---LocateList���ܴ�ʵ�֣�\n");
                printf("��������Ҷ����������ƣ�");
                scanf("%s",Name);
                if(LocateList(trees, Name)) printf("�ö��������߼�����Ϊ��%d\n", LocateList(trees, Name));
                else printf("����������ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 23:
                //printf("\n----TraverseList���ܴ�ʵ�֣�\n");
                if(TraverseList(trees) == INFEASIBLE) printf("���������Ϊ�գ�\n");
                getchar();getchar();
                break;
            case 24:
                //printf("\n----SelectList���ܴ�ʵ�֣�\n");
                printf("��ѡ��Ҫ����Ķ��������߼�������");
                scanf("%d", &ans);
                if(SelectList(trees, ans) == OK) {
                    printf("��������ѡȡ�ɹ���\n");
                    num = ans;
                }
                else printf("������ѡȡʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 25:
                //printf("\n----ClearBiTree���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                ClearList(T); 
                printf("��������ճɹ���\n");
                getchar();getchar();
                break; 
			case 26:
                //printf("\n----GetFabling���ܴ�ʵ�֣�\n");
                if(!T) {
                    printf("������Ϊ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("������Ҫ��ȡ���׽��Ĺؼ��֣�");
                scanf("%d",&e);
                if(T->data.key == e){
                    printf("�ý��Ϊ����㣡");
                }
                t = GetFabling(T, e);
                if(t == NULL) printf("�ý�㲻���ڸ��׽ڵ�!\n");
                else{
                    printf("��Ԫ�ظ��׽ڵ��ȡ�ɹ���\n");
                    printf("�ý��ĸ��׽��ؼ���Ϊ %d �����ϢΪ�� %s", t->data.key, t->data.others);
                }
                t = NULL;
                getchar();getchar();
                break;   
            case 0:
                break;
        }//end of switch
    }//end of while
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
    return 0;
}//end of main()





