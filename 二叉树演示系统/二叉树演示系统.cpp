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
} TElemType; //二叉树结点类型定义

typedef struct BiTNode{  //二叉链表结点的定义
    TElemType  data;
    struct BiTNode *lchild,*rchild;
} BiTNode, *BiTree;

typedef struct{  //线性表的集合类型定义
    struct { char name[30];
        BiTree L;
    } elem[11];
    int length;
}TREES;
TREES trees;      //线性表集合的定义TREES

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
//二叉树的创建，通过先序遍历的方式创建
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
//将二叉树设置成空，并删除所有结点，释放结点空间
{
    if(!T) return OK;
    ClearBiTree(T->lchild);
    ClearBiTree(T->rchild);
    free(T);
    T = NULL;
    return OK;

}

status DestroyBiTree(BiTree &T)
//将二叉树销毁
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
//求二叉树T的深度
{
    if(!T) return  ERROR;
    return MAX(BiTreeDepth(T->lchild), BiTreeDepth(T->rchild)) + 1;

}



BiTNode* LocateNode(BiTree T,KeyType e)
//查找结点
{
    if(!T) return NULL;
    if(T->data.key == e) return T;
    if(LocateNode(T->lchild, e))
        return LocateNode(T->lchild, e);
    else
        return LocateNode(T->rchild, e);

}

status Assign(BiTree &T,KeyType e,TElemType value)
//实现结点赋值。
{
    BiTree p = LocateNode(T, value.key);
    BiTree t = LocateNode(T, e);
    if(t == NULL || (value.key!= e && p != NULL)) return ERROR;
    t->data.key = value.key;
    strcpy(t->data.others, value.others);
    return OK;

}

BiTNode* GetSibling(BiTree T,KeyType e)
//实现获得兄弟结点
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
//实现获得父亲结点
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
//插入结点。
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
//删除结点。
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
//先序遍历二叉树T
{
    if(!T) return INFEASIBLE;
    visit(T);
    PreOrderTraverse(T->lchild, visit);
    PreOrderTraverse(T->rchild, visit);
    return OK;

}

status InOrderTraverse(BiTree T,void (*visit)(BiTree))
//中序遍历二叉树T
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
//后序遍历二叉树T
{
    if(!T) return ERROR;
    PostOrderTraverse(T->lchild, visit);
    PostOrderTraverse(T->rchild, visit);
    visit(T);
    return OK;

}

status LevelOrderTraverse(BiTree T,void (*visit)(BiTree))
//按层遍历二叉树T
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
//将二叉树的结点数据写入到文件FileName中
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
        printf("该文件不能读入！\n");
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
//读入文件FileName的结点数据，创建二叉树
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
// 需要在TREES中增加一个名称为ListName的空线性表
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
// TREES中删除一个名称为ListName的线性表
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
// 在TREES中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回0
{
    if(!trees.elem) return INFEASIBLE;//疑问未解决
    for(int i = 0; i < trees.length; i++)
        if(!strcmp(ListName, trees.elem[i].name))
            return i + 1;
    return 0;

}

status TraverseList(TREES trees){
// 如果多线性表不为空，依次显示多线性表的名称，每个名称间空一格，返回OK；如果多线性表为空，返回INFEASIBLE。
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
// 进行线性表的选择
    if(trees.length == 0) return INFEASIBLE;
    if(i < 1 || i > trees.length) return ERROR;
    T = trees.elem[i - 1].L;
    return OK;
}

int MaxPathSum(BiTree T){
//返回最大路径和	
    if(!T) return  ERROR;
    return MAX(MaxPathSum(T->lchild), MaxPathSum(T->rchild)) + T->data.key;
}

BiTree LowestCommonAncestor(BiTree T, int e1, int e2){
//返回公共祖先	
    if(!T || T->data.key == e1 || T->data.key == e2) return T;
    BiTree l = LowestCommonAncestor(T->lchild, e1, e2);
    BiTree r = LowestCommonAncestor(T->rchild, e1, e2);
    if(l && !r) return l;
    if(!l && r) return r;
    if(!l && !r) return NULL;
    if(l && r) return T;
}

status InvertTree(BiTree &T){
//二叉树翻转	
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
        printf("    	  1. CreateBiTree  二叉树创建                 14. LevelOrderTraverse    二叉树层序遍历\n");
        printf("    	  2. DestroyBiTree 二叉树销毁                 15. MaxPathSum            二叉树最大路径和\n");
        printf("    	  3. ClearBiTree   二叉树清空                 16. LowestCommonAncestor  二叉树最近祖先\n");
        printf("    	  4. BiTreeEmpty   二叉树判空                 17. InvertTree            二叉树翻转\n");
        printf("    	  5. BiTreeDepth   二叉树获取深度             18. SaveList              二叉树文件保存\n");
        printf("    	  6. LocateNode    二叉树查找结点             19. LoadList              二叉树文件录入\n");
        printf("          7. Assign        二叉树结点赋值             20. AddList               多二叉树表添加\n");
        printf("          8. GetFabling    二叉树获取父亲结点         21. DestroyList           多二叉树表销毁\n");
        printf("          9. InsertNode    二叉树插入结点             22. LocateList            多二叉树表位置查找\n");
        printf("          10. DeleteNode   二叉树删除结点             23. TraverseList          多二叉树表遍历\n");
        printf("          11. PreOrderTraverse   二叉树前序遍历       24. SelectList            二叉树操作选择\n");
        printf("          12. InOrderTraverse    二叉树中序遍历       25. ClearList             多二叉树表清空\n");
        printf("          13. PostOrderTraverse  二叉树后序遍历       26. GetSibling    二叉树获取兄弟结点\n"); 
        printf("    	  0. Exit        退出\n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("          说明：每次操作过后请点击空格确认才能进行下一步操作！\n");
        printf("\n          当前操作的二叉树为：");
        if(num < 1|| num > trees.length){
            if(num > trees.length){
                T = NULL;
                num = 0;
            }
            printf("默认二叉树");
            if(!T)
                printf("(未创建)");
            printf("\n\n\n");
        }
        else{
        	printf("%s",trees.elem[num - 1].name);
        	if(!T)
                printf("(未创建)");
            printf("\n\n\n");
		}   
        if(op > 26 || op < 0)
            printf("上一步命令出错！请根据菜单正确输入！\n\n\n");
        printf("请选择你的操作[0~26]:");
        scanf("%d",&op);
        switch(op){
            case 1:
                //printf("\n----CreateBiTree功能待实现！\n");
                if(T){
                    printf("该二叉树已存在！\n");
                    getchar();getchar();
                    break;
                }
                i = 0;
                printf("请输入合法先序序列（每个结点对应一个整型的关键字和一个字符串，当关键字为0时，表示空子树，为-1表示输入结束）：");
                do {
                    scanf("%d%s", &definition[i].key, definition[i].others);
                } while (definition[i++].key != -1);
                ans = CreateBiTree(T,definition);
                count = 0;
                if(ans == ERROR) printf("关键字不唯一！创建失败！\n");
                else printf("二叉树创建成功！\n");
                getchar();getchar();
                break;
            case 2:
                //printf("\n----DestroyBiTree功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                ans = DestroyBiTree(T);
                if(ans == OK) printf("二叉树销毁成功！\n");
                else printf("二叉树销毁失败！\n");
                getchar();getchar();
                break;    
            case 3:
                //printf("\n----ClearBiTree功能待实现！\n");
                ans = ClearBiTree(T);
                if(ans == OK) printf("二叉树清空成功！\n");
                else printf("二叉树清空失败！\n");
                getchar();getchar();
                break;
           	case 4:
                //printf("\n----BiTreeEmpty功能待实现！\n");
                ans = BiTreeEmpty(T);
                if(ans == FALSE) printf("二叉树为空！\n");
                else printf("二叉树不为空！\n");
                getchar();getchar();
                break;
            case 5:
                //printf("\n----BiTreeDepth功能待实现！\n");
                length = BiTreeDepth(T);
                if(length) printf("该二叉树的深度为%d！\n", length);
                else printf("二叉树为空！\n");
                getchar();getchar();
                break;
            case 6:
                //printf("\n----LocateNode功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入想要查询的结点关键字：");
                scanf("%d",&e);
                t = LocateNode(T, e);
                if(t == NULL) printf("该节点不存在！\n");
                else {
                    printf("该节点存在！结点信息为：%s\n",t->data.others);
                }
                t = NULL;
                getchar();getchar();
                break;
            case 7:
                //printf("\n----Assign功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入想要赋值的节点关键字:");
                scanf("%d", &e);
                printf("请输入关键字：");
                scanf("%d", &value.key);
                printf("请输入结点信息：");
                scanf("%s", value.others);
                ans = Assign(T, e, value);
                if(ans == OK) printf("结点赋值成功!\n");
                else printf("结点复制失败（请检查该关键字是否存在或者赋值关键字是否重复）!\n");
                getchar();getchar();
                break;
            case 8:
                //printf("\n----GetSibling功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入要获取兄弟结点的关键字：");
                scanf("%d",&e);
                t = GetSibling(T, e);
                if(t == NULL) printf("该结点不存在或不存在兄弟结点!\n");
                else{
                    printf("该元素兄弟结点获取成功！\n");
                    printf("该结点的兄弟结点关键字为 %d 结点信息为： %s\n", t->data.key, t->data.others);
                }
                getchar();getchar();
                break;
            case 9:
                //printf("\n----InsertNode功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入想要插入的结点关键字：");
                scanf("%d", &e);
                printf("请输入关键字：");
                scanf("%d", &value.key);
                printf("请输入结点信息：");
                scanf("%s", value.others);
                printf("请输入插入方式（LR为0或者1时作为关键字为e的结点的左或右孩子结点，LR为-1时，作为根结点插入，原根结点作为c的右子树）：");
                scanf("%d", &LR);
                ans = InsertNode(T, e, LR, value);
                if(ans == ERROR) printf("结点插入失败（请检查该关键字是否存在或者插入关键字是否重复）!\n");
                else printf("结点插入成功！\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----DeleteNode功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入想要删除的结点关键字：");
                scanf("%d", &e);
                ans = DeleteNode(T, e);
                if(ans == ERROR) printf("结点删除失败（请检查该关键字是否存在）!\n");
                else printf("结点删除成功！\n");
                getchar();getchar();
                break;
            case 11:
                //printf("\n----PreOrderTraverse功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("先序遍历二叉树的结果：\n");
                PreOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 12:
                //printf("\n----InOrderTraverse功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("中序遍历二叉树的结果：\n");
                InOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 13:
                //printf("\n----PostOrderTraverse功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("后序遍历二叉树的结果：\n");
                PostOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 14:
                //printf("\n----LevelOrderTraverse功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("层序遍历二叉树的结果：\n");
                LevelOrderTraverse(T, visit);
                getchar();getchar();
                break;
            case 15:
                //printf("\n----MaxPathSum功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                length = MaxPathSum(T);
                printf("根节点到叶子结点的最大路径和为：%d\n", length);
                getchar();getchar();
                break;
            case 16:
                //printf("\n----LowestCommonAncestor功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入第一个结点：");
                scanf("%d", &e);
                printf("请输入第二个结点：");
                scanf("%d", &ee);
                t = LocateNode(T, e);
                if(!t){
                    printf("第一个结点不存在！\n");
                    getchar();getchar();
                    break;
                }
//                else if(t == T){
//                    printf("第一个结点为根节点，不存在祖先！\n");
//                    t = NULL;
//                    getchar();getchar();
//                    break;
//                }
                t = LocateNode(T, ee);
                if(!t){
                    printf("第二个结点不存在！\n");
                    getchar();getchar();
                    break;
                }
//                else if(t == T){
//                    printf("第二个结点为根节点，不存在祖先！\n");
//                    t = NULL;
//                    getchar();getchar();
//                    break;
//                }
                t = LowestCommonAncestor(T,e,ee);
                printf("两结点最近公共祖先的关键字为：%d， 结点信息为：%s\n", t->data.key, t->data.others);
                t = NULL;
                getchar();getchar();
                break;
            case 17:
                //printf("\n----InvertTree功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                InvertTree(T);
                printf("二叉树翻转成功！\n");
                getchar();getchar();
                break;
            case 18:
                //printf("\n----SaveList功能待实现！\n");
                printf("请输入要保存的文件名称：");
                scanf("%s",FileName);
                ans = SaveBiTree(T, FileName);
                if(ans == INFEASIBLE) printf("二叉树不存在!文件保存失败！\n");
                else if(ans == ERROR);
                else printf("文件保存成功！\n");
                getchar();getchar();
                break;
            case 19:
                //printf("\n----LoadList功能待实现！\n");
                printf("请输入要录入的文件名称：");
                scanf("%s", FileName);
                if(LoadBiTree(T, FileName) == INFEASIBLE) printf("二叉树存在!文件录入失败\n");
                else printf("文件录入成功！\n");
                getchar();getchar();
                break;
            case 20:
                //printf("\n----AddList功能待实现！\n");
                if(trees.length == MAXlength) {
                    printf("多二叉树表管理已满，请清除某些二叉树后再操作！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入新增二叉树的名称：");
                scanf("%s",Name);
                ans = AddList(trees, Name);
                if(ans == INFEASIBLE) printf("该名称的二叉树已经存在!\n"); 
                else printf("%s已成功添加！\n",Name);
                getchar();getchar();
                break;
            case 21:
                //printf("\n---DestoryList功能待实现！\n");
                if(trees.length == 0) {
                    printf("多二叉树表管理已空，请添加某些二叉树后再操作！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入销毁二叉树的名称：");
                scanf("%s",Name);
                ans = DestoryList(trees, Name);
                if(ans == OK)printf("%s已成功销毁！\n",Name);
                else printf("二叉树不存在！\n");
                getchar();getchar();
                break;
            case 22:
                //printf("\n---LocateList功能待实现！\n");
                printf("请输入查找二叉树的名称：");
                scanf("%s",Name);
                if(LocateList(trees, Name)) printf("该二叉树的逻辑索引为：%d\n", LocateList(trees, Name));
                else printf("二叉树查找失败！\n");
                getchar();getchar();
                break;
            case 23:
                //printf("\n----TraverseList功能待实现！\n");
                if(TraverseList(trees) == INFEASIBLE) printf("多二叉树表为空！\n");
                getchar();getchar();
                break;
            case 24:
                //printf("\n----SelectList功能待实现！\n");
                printf("请选择要处理的二叉树的逻辑索引：");
                scanf("%d", &ans);
                if(SelectList(trees, ans) == OK) {
                    printf("二叉树已选取成功！\n");
                    num = ans;
                }
                else printf("二叉树选取失败！\n");
                getchar();getchar();
                break;
            case 25:
                //printf("\n----ClearBiTree功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                ClearList(T); 
                printf("二叉树清空成功！\n");
                getchar();getchar();
                break; 
			case 26:
                //printf("\n----GetFabling功能待实现！\n");
                if(!T) {
                    printf("二叉树为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入要获取父亲结点的关键字：");
                scanf("%d",&e);
                if(T->data.key == e){
                    printf("该结点为根结点！");
                }
                t = GetFabling(T, e);
                if(t == NULL) printf("该结点不存在父亲节点!\n");
                else{
                    printf("该元素父亲节点获取成功！\n");
                    printf("该结点的父亲结点关键字为 %d 结点信息为： %s", t->data.key, t->data.others);
                }
                t = NULL;
                getchar();getchar();
                break;   
            case 0:
                break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    return 0;
}//end of main()





