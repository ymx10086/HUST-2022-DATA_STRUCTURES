#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAX_VERTEX_NUM 20
#define MAXlength 10

typedef int status;
typedef int KeyType; 
typedef enum {DG,DN,UDG,UDN} GraphKind;
typedef struct {
     KeyType  key;
     char others[20];
} VertexType; //顶点类型定义


typedef struct ArcNode {         //表结点类型定义
	 int adjvex;              //顶点位置编号 
	 struct ArcNode  *nextarc;	   //下一个表结点指针
} ArcNode;

typedef struct VNode{				//头结点及其数组类型定义
	 VertexType data;       	//顶点信息
	 ArcNode *firstarc;      	 //指向第一条弧
	} VNode,AdjList[MAX_VERTEX_NUM];
	
typedef  struct {  //邻接表的类型定义
    AdjList vertices;     	 //头结点数组
    int vexnum,arcnum;   	  //顶点数、弧数
    GraphKind  kind;        //图的类型
   } ALGraph;


typedef struct {//森林的定义
    struct {
        char name[30];
        ALGraph G;
    }elem[11];
    int length;
    int listsize;
}GRAPHS;
GRAPHS Graphs;

int num = 10;

status check1(VertexType V[]){
//判断结点集里是否有重复结点 
    int i = 0;
    if(V[i].key == -1) return 1;
    while(V[i].key != -1){
        for(int j = 0; j < i; j++)
            if(V[j].key == V[i].key)
                return 1;
        i++;
    }
    if(i > 20) return 1;
    return 0;
}

void deleteVR(KeyType VR[][2], int i, int &num){
    for(int k = i; k <= num; k++){
        VR[k][0] = VR[k + 1][0];
        VR[k][1] = VR[k + 1][1];
    }
    num--;
    return;
}

status check2(VertexType V[], KeyType VR[][2]){
//判断是否有重复，错乱，多余的边 
    int flag = 0;
    for(int i = 0; VR[i][0] != -1; i++){
        for(int j = 0; V[j].key != -1; j++){
            if(VR[i][0] == V[j].key){
                flag = 1;
                break;
            }            
        }
        if(!flag) return 1;
        flag = 0;
    }
    for(int i = 0; VR[i][1] != -1; i++){
        for(int j = 0; V[j].key != -1; j++){
            if(VR[i][1] == V[j].key){
                flag = 1;
                break;
            }            
        }
        if(!flag) return 1;
        flag = 0;
    }
    int i = 0, num = 0;
    while(VR[num++][0] != -1);
    while(VR[i][0] != -1){
        if(VR[i][0] == VR[i][1]){
            deleteVR(VR, i, num);
            continue;
        }    
        for(int j = 0; j < i; j++){
            if(VR[j][0] == VR[i][0] && VR[j][1] == VR[i][1]){
               deleteVR(VR, i, num);
               i--;
               break;
            }
            if(VR[j][0] == VR[i][1] && VR[j][1] == VR[i][0]){
               deleteVR(VR, i, num);
               i--;
               break;
            }
        }
        i++;
    }
    return 0;
}

status CreateCraph(ALGraph &G,VertexType V[],KeyType VR[][2])
/*根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR
如果有相同的关键字，返回ERROR。*/
{
    if(check1(V)) return ERROR;
    if(check2(V, VR)) return ERROR;
    G.kind = UDG;
    G.vexnum = 0, G.arcnum = 0;
    int m;
    while(V[G.vexnum].key != -1){
        G.vertices[G.vexnum].data.key = V[G.vexnum].key;
        strcpy(G.vertices[G.vexnum].data.others, V[G.vexnum].others);
        G.vexnum++;
    }
    for(int i = 0; i < G.vexnum; i++){
        G.vertices[i].firstarc = NULL;
    }
    while(VR[G.arcnum][0] != -1){
        for(int i = 0; i < G.vexnum; i++){
            if(G.vertices[i].data.key == VR[G.arcnum][0]){
                for(m = 0; m < G.vexnum; m++){
                    if(G.vertices[m].data.key == VR[G.arcnum][1])
                        break;
                }
                ArcNode *t = (ArcNode *)malloc(sizeof(ArcNode));
                t->adjvex = m;
                t->nextarc = G.vertices[i].firstarc;
                G.vertices[i].firstarc = t;      
            }
            if(G.vertices[i].data.key == VR[G.arcnum][1]){
                for(m = 0; m < G.vexnum; m++){
                    if(G.vertices[m].data.key == VR[G.arcnum][0])
                        break;
                }
                ArcNode *t = (ArcNode *)malloc(sizeof(ArcNode));
                t->adjvex = m;
                t->nextarc = G.vertices[i].firstarc;
                G.vertices[i].firstarc = t;      
            }   
        }
        G.arcnum++;
    }
    return OK;
}


status DestroyGraph(ALGraph &G)
/*销毁无向图G,删除G的全部顶点和边*/
{
    int i;
    ArcNode *p = NULL, *q = NULL;
    for(i = 0; i < G.vexnum; i++){
        p = G.vertices[i].firstarc;
        if(!p) continue;
        while(1){
            if(!p) break;
            q = p->nextarc;
            free(p);
            p = q;
        }
    }
    G.vexnum = G.arcnum = 0;
    return OK;
}


int LocateVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回位序，否则返回-1；
{
    int i;
    for(i = 0; i < G.vexnum; i++){
        if(G.vertices[i].data.key == u)
        return i;
    }
    return -1;

}



status PutVex(ALGraph &G,KeyType u,VertexType value)
//根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
//如果查找失败或关键字不唯一，返回ERROR
{
    int i;
    for(i = 0; i < G.vexnum; i++)
        if(G.vertices[i].data.key == value.key)
            if(G.vertices[i].data.key != u)
                return ERROR;
    for(i = 0; i < G.vexnum; i++){
        if(G.vertices[i].data.key == u){
        G.vertices[i].data.key = value.key;
        strcpy(G.vertices[i].data.others, value.others);
        return OK;  
        }    
    }
    return ERROR;

}


int FirstAdjVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1；
{
    int i;
    for(i = 0; i < G.vexnum; i++){
        if(G.vertices[i].data.key == u)
            if(G.vertices[i].firstarc)
                return G.vertices[i].firstarc->adjvex;
    }
    return -1;
   
}

int NextAdjVex(ALGraph G,KeyType v,KeyType w)
//根据u在图G中查找顶点，查找成功返回顶点v的邻接顶点相对于w的下一邻接顶点的位序，查找失败返回-1；
{

    int i;
    ArcNode *p = NULL;
    for(i = 0; i < G.vexnum; i++){
        if(G.vertices[i].data.key == v){
            if(G.vertices[i].firstarc){
                p = G.vertices[i].firstarc;
                while(p){
                    if(G.vertices[p->adjvex].data.key == w)
                        if(p->nextarc)
                            return p->nextarc->adjvex;
                    p = p->nextarc;
                }
            }
        }
    }
    return -1;

}

status InsertVex(ALGraph &G,VertexType v)
//在图G中插入顶点v，成功返回OK,否则返回ERROR
{
    if(G.vexnum == MAX_VERTEX_NUM) return ERROR;
    if(LocateVex(G, v.key) != -1) return ERROR;
    G.vertices[G.vexnum++].data = v;
    return OK;
    
}


status DeleteVex(ALGraph &G,KeyType v)
//在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
    int k;
    if((k = LocateVex(G, v)) == -1) return ERROR;
    if(G.vexnum == 1) return ERROR;
    ArcNode *p = NULL, *q = NULL;
    p = G.vertices[k].firstarc;
    int count = 0;
    int temp[100], t[100] = {0};
    t[k] = 1;
    while(1){
        if(!p) break;
        q = p->nextarc;
        temp[count++] = p->adjvex;
        t[p->adjvex] = 1;
        free(p);
        p = q;
    }
    for(int i = 0; i < count; i++){
        p = G.vertices[temp[i]].firstarc;
        if(G.vertices[p->adjvex].data.key == v) {
            G.vertices[temp[i]].firstarc = G.vertices[temp[i]].firstarc->nextarc;
            free(p);
            p = G.vertices[temp[i]].firstarc;
            while(p){
                if(p->adjvex > k)
                    p->adjvex--;
                p = p->nextarc;
            }
        }
        else{
            while(G.vertices[p->adjvex].data.key != v){
                q = p;
                if(q->adjvex > k)
                    q->adjvex--;
                p = p->nextarc;
            }
            q->nextarc = p->nextarc;
            free(p);
            p = q->nextarc;
            while(p){
                if(p->adjvex > k)
                    p->adjvex--;
                p = p->nextarc;
            }
        }
    }
    for(int i = 0; i < G.vexnum; i++){
        if(t[i]) continue;
        p = G.vertices[i].firstarc;
        while(p){
            if(p->adjvex > k)
                p->adjvex--;
            p = p->nextarc;
        }
    }
    for(int j = k; j < G.vexnum - 1; j++){
        G.vertices[j] = G.vertices[j + 1];
    }
    G.vexnum--;
    G.arcnum = G.arcnum - count;
    return OK;
 
}


status InsertArc(ALGraph &G,KeyType v,KeyType w)
//在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
    int i, j;
    ArcNode *p = NULL;
    if((i = LocateVex(G, v)) == -1) return ERROR;
    if((j = LocateVex(G, w)) == -1) return ERROR;
    p = G.vertices[i].firstarc;
    while(p){
        if(p->adjvex == j)
            return ERROR;
        p = p->nextarc; 
    }
    ArcNode *t = (ArcNode *)malloc(sizeof(ArcNode));
    t->adjvex = j;
    t->nextarc = G.vertices[i].firstarc;
    G.vertices[i].firstarc = t;
    ArcNode *tt = (ArcNode *)malloc(sizeof(ArcNode));
    tt->adjvex = i;
    tt->nextarc = G.vertices[j].firstarc;
    G.vertices[j].firstarc = tt;
    G.arcnum++;
    return OK;

}

status DeleteArc(ALGraph &G,KeyType v,KeyType w)
//在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
{
    int i, j, flag = 0;
    ArcNode *p = NULL, *q = NULL;
    if((i = LocateVex(G, v)) == -1) return ERROR;
    if((j = LocateVex(G, w)) == -1) return ERROR;
    p = G.vertices[i].firstarc;
    while(p){
        if(p->adjvex == j){
            flag = 1;
            break;
        }
        p = p->nextarc; 
    }
    if(!flag) return ERROR;
    q = p = G.vertices[i].firstarc;
    if(p->adjvex == j){
        G.vertices[i].firstarc = p->nextarc;
        free(p); 
    }
    else{
        while(p){
            if(p->adjvex == j){
                q->nextarc = p->nextarc;
                free(p);
                break;
            }
            q = p;
            p = p->nextarc;
        }
    }
    q = p = G.vertices[j].firstarc;
    if(p->adjvex == i){
        G.vertices[j].firstarc = p->nextarc;
        free(p); 
    }
    else{
        while(p){
            if(p->adjvex == i){
                q->nextarc = p->nextarc;
                free(p);
                break;
            }
            q = p;
            p = p->nextarc;
        }
    }
    G.arcnum--;
    return OK;
    
}

void DFS(ALGraph G, int *t, int v, void (*visit)(VertexType)){
    ArcNode *p = NULL;
    visit(G.vertices[v].data);
    t[v] = 1;
    p = G.vertices[v].firstarc;
    while(p != NULL){
        if(!t[p->adjvex]){
            DFS(G, t, p->adjvex, visit);
        }
        p = p->nextarc;
    }
}

status DFSTraverse(ALGraph &G,void (*visit)(VertexType))
//对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    if(G.vexnum == 0) return ERROR;
    int t[100] = {0};
    for(int i = 0; i < G.vexnum; i++){
        if(!t[i])
            DFS(G, t, i, visit);
    }
    return OK;
    
}

void visit(VertexType v)
{
    printf(" %d %s",v.key,v.others);
}

status BFSTraverse(ALGraph &G,void (*visit)(VertexType))
//对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    int stack[100], top = 0, low = 0;
    int t[100] = {0};
    ArcNode *p = NULL;
    for(int i = 0; i < G.vexnum; i++){
        if(!t[i]){
            stack[top++] = i;
            p = G.vertices[i].firstarc;
            visit(G.vertices[i].data);
            t[i] = 1;
            while(p || low != top){
                if(p){
                    if(!t[p->adjvex]){
                        visit(G.vertices[p->adjvex].data);
                        t[p->adjvex] = 1;
                        stack[top++] = p->adjvex;
                    }
                    p = p->nextarc;
                    continue;
                }
                if(low != top){
                    p = G.vertices[stack[low++]].firstarc;
                }
            }
        }
    }
   
}

status SaveGraph(ALGraph G, char FileName[])
//将图的数据写入到文件FileName中
{
    if(G.vexnum == 0) return INFEASIBLE;
    int nu = -1;
    FILE *fp;
    if ((fp = fopen(FileName,"wb")) == NULL)
    {
        printf("File open error!\n ");
        return ERROR;
    }
    fwrite(&G.vexnum, sizeof(int), 1, fp);
    fwrite(&G.arcnum, sizeof(int), 1, fp);
    
    for(int i = 0; i < G.vexnum; i++)
    {
        fwrite(&G.vertices[i].data, sizeof(VertexType), 1, fp);
        ArcNode* s = G.vertices[i].firstarc;
        while(s)
        {
            fwrite(&s->adjvex, sizeof(int), 1, fp);
            s = s->nextarc;
        }
        fwrite(&nu, sizeof(int), 1, fp);
    }
    fclose(fp);
    return OK;
  
}

status LoadGraph(ALGraph &G, char FileName[]) //14
//读入文件FileName的图数据，创建图的邻接表
{
    if(G.vexnum != 0) return INFEASIBLE;
    FILE *fp;
    if ((fp = fopen(FileName,"rb")) == NULL)
    {
        printf("File open error!\n ");
        return ERROR;
    }
    fread(&G.vexnum, sizeof(int), 1, fp);
    fread(&G.arcnum, sizeof(int), 1, fp);
    G.kind = UDG;

    for(int i = 0; i < G.vexnum ; i++)
    {
        fread(&G.vertices[i].data, sizeof(VertexType), 1, fp);
        G.vertices[i].firstarc = NULL;
        ArcNode* last = G.vertices[i].firstarc;
        int arc, flag = 0;
        while(fread(&arc, sizeof(int), 1, fp))
        {
            if(arc == -1) break;
            if(flag == 0)
            {
                ArcNode* s = (ArcNode*)malloc(sizeof(ArcNode));
                flag = 1;
                s->adjvex = arc;
                s->nextarc = NULL;
                G.vertices[i].firstarc = s;
                last = s;
                continue;
            }
            ArcNode* s = (ArcNode*)malloc(sizeof(ArcNode));
            s->adjvex = arc;
            s->nextarc = NULL;
            last->nextarc = s;
            
            last = s;
        }
    }
    fclose(fp);
    return OK;
    
}

status AddList(GRAPHS& Graphs,char ListName[])
// 需要在Graphs中增加一个名称为ListName的空图 
{
	for(int i = 0; i < Graphs.length; i++){
		if(!strcmp(Graphs.elem[i].name,ListName)) return INFEASIBLE;
	}
    strcpy(Graphs.elem[Graphs.length].name,ListName);
    Graphs.elem[Graphs.length].G.vexnum = 0;
    Graphs.length++;
    return OK;
}

status DestoryList(GRAPHS& Graphs,char ListName[])
// Graphs中删除一个名称为ListName的图 
{
    for(int i = 0; i < Graphs.length; i++)
        if(!strcmp(ListName, Graphs.elem[i].name)){
            if(Graphs.elem[i].G.vexnum)
                DestroyGraph(Graphs.elem[i].G);
            for (int j = i; j < Graphs.length - 1; j++)
                Graphs.elem[j] = Graphs.elem[j + 1];
            Graphs.length--;
            return OK;
        }
    return ERROR;

}

int LocateList(GRAPHS& Graphs,char ListName[])
// 在Graphs中查找一个名称为ListName的图，成功返回逻辑序号，否则返回0
{
    if(!Graphs.elem) return INFEASIBLE;//疑问未解决
    for(int i = 0; i < Graphs.length; i++)
        if(!strcmp(ListName, Graphs.elem[i].name))
            return i + 1;
    return 0;

}

status TraverseList(GRAPHS& Graphs){
// 如果多图表不为空，依次显示多图表的名称，每个名称间空一格，返回OK；如果多图表为空，返回INFEASIBLE。
    if(Graphs.length == 0) return INFEASIBLE;
    printf("\n-----------all names -----------------------\n");
    for(int i = 0; i < Graphs.length; i++){
        printf("%s",Graphs.elem[i].name);
        if(i != Graphs.length - 1) printf(" ");
    }
    printf("\n------------------ end ------------------------\n");
    return OK;
}

status SelectList(GRAPHS& Graphs, int i){
// 进行图的选择
    if(Graphs.length == 0) return INFEASIBLE;
    if(i < 1 || i > Graphs.length) return ERROR;
    num = i - 1;
    return OK;
}
//5 a 6 b 7 c 8 d 9 e 10 f 11 g -1 nil
//5 7 7 8 5 8 7 9 8 9 6 8 10 11 -1 -1

status VerticesSetLessThanK(ALGraph G, int v, int k){
//查找与给定结点距离为k的结点 
	int stack[100], top = 0, low = 0, count = 0 , num = 1;
    int t[100] = {0}, b[100];
    for(int i = 0; i < G.vexnum; i++)
    	b[i] = G.vexnum;
    VNode p;
    ArcNode *q = NULL;
    int i = LocateVex(G, v);
    p = G.vertices[i];
    stack[top++] = i;
    t[i] = 1;
    if(k <= 1){
    	printf("请不要查找距离为1以下的结点！\n");
	} 
    while(1){
    	if(low == num){
    		count++;
    		num = top;
		}
		if(count == k - 1) break;
		visit(p.data);
		q = p.firstarc;
		while(q){
			if(!t[q->adjvex]){
				stack[top++] = q->adjvex;
				t[q->adjvex] = 1;	
			}
			q = q->nextarc;
		}
		if(low + 1 == top){
			break;
		}
		p = G.vertices[stack[++low]];	
	}
	return OK;
}

void FindTarget(ALGraph G, int* t, int &target, int current, int i, int j){
	if(i == j){
		target = (target > current) ? current : target;
		return;
	}
	current++;
	t[i] = 1;
	ArcNode *p = G.vertices[i].firstarc;
	while(p){
		if(!t[p->adjvex]){
			t[p->adjvex] = 1;
			FindTarget(G, t, target, current, p->adjvex, j);
			t[p->adjvex] = 0;
		}
		p = p->nextarc; 
	}
}

status ShortestPathLength(ALGraph G, int v, int w){
//查找最短路径 
	int t[100] = {0}, i, j, target = G.vexnum;
	if((i = LocateVex(G, v)) == -1) return ERROR;
    if((j = LocateVex(G, w)) == -1) return ERROR;
	FindTarget(G, t, target, 0, i, j);
	return target;
}

status ConnectedComponentsNums(ALGraph G){
//计算连通分支数目 
	int stack[100], top = 0, low = 0;
    int t[100] = {0};
    ArcNode *p = NULL;
    int count = 0;
    for(int i = 0; i < G.vexnum; i++){
        if(!t[i]){
            stack[top++] = i;
            p = G.vertices[i].firstarc;
            t[i] = 1;
            while(p || low != top){
                if(p){
                    if(!t[p->adjvex]){
                        t[p->adjvex] = 1;
                        stack[top++] = p->adjvex;
                    }
                    p = p->nextarc;
                    continue;
                }
                if(low != top){
                    p = G.vertices[stack[low++]].firstarc;
                }
            }
            count++;
        }
    }
    return count;
}

int main(){
    int op=1;
    int i, e, ans, j; 
    VertexType V[100];
	KeyType VR[100][2]; 
    VertexType value;
    char FileName[100], Name[100];
    while(op){
        system("cls");
        printf("\n\n");
        printf("                                    Menu for Graphy  \n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("    	  1. CreateCraph   创建图                 12. BFSTraverse     广度优先搜索遍历\n");
        printf("    	  2. DestroyGraph  销毁图                 13. SaveGraph       图文件保存\n");
        printf("    	  3. LocateVex     查找顶点               14. LoadGraph       图文件录入  \n");
        printf("    	  4. PutVex        顶点赋值               15. AddList         多图表添加  \n");
        printf("    	  5. FirstAdjVex   获得第一邻接点         16. DestroyList     多图表销毁    \n");
        printf("    	  6. NextAdjVex    获得下一邻接点         17. LocateList      多图表位置查找    \n");
        printf("          7. InsertVex     插入顶点               18. TraverseList    多图表遍历\n");
        printf("          8. DeleteVex     删除顶点               19. SelectList      图操作选择\n");
        printf("          9. InsertArc     插入弧                 20. VerticesSetLessThanK     距离小于k的顶点集合\n");
        printf("          10. DeleteArc   删除弧                  21. ShortestPathLength       顶点间最短路径和长度\n");
        printf("          11. DFSTraverse   深度优先搜索遍历      22. ConnectedComponentsNums  图的连通分量\n");
        printf("    	  0. Exit        退出\n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("          说明：每次操作过后请点击空格确认才能进行下一步操作！\n");
        printf("\n          当前操作的二叉树为：");
        if(num < 1|| num > Graphs.length){
            if(num > Graphs.length){
                Graphs.elem[num].G.vexnum = 0;
                num = 0;
            }
            printf("默认图");
            if(Graphs.elem[num].G.vexnum == 0)
                printf("(未创建)");
            printf("\n\n\n");
        }
        else{
        	printf("%s",Graphs.elem[num - 1].name);
        	if(Graphs.elem[num].G.vexnum == 0)
                printf("(未创建)");
            printf("\n\n\n");
		}   
        if(op > 22 || op < 0)
            printf("上一步命令出错！请根据菜单正确输入！\n\n\n");
        printf("请选择你的操作[0~22]:");
        scanf("%d",&op);
        switch(op){
            case 1:
                //printf("\n----CreateCraph功能待实现！\n");
                if(Graphs.elem[num].G.vexnum){
                    printf("该图已存在！\n");
                    getchar();getchar();
                    break;
                }
                i = 0;
	            printf("请输入顶点序列(-1 nil作为结束标志)：");
	            do{
	                scanf("%d%s", &V[i].key, V[i].others);
	            }while (V[i++].key != -1);
	            i = 0;
	            printf("请输入关系对序列，以-1 -1结束：");
	            do{
	                scanf("%d%d", &VR[i][0], &VR[i][1]);
	            }while (VR[i++][0] != -1);
	            if (CreateCraph(Graphs.elem[num].G, V, VR) == OK)
	                printf("图创建成功！\n");
	            else
	                printf("图创建失败！\n");
                getchar();getchar();
                break;
            case 2:
                //printf("\n----DestroyGraph功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                ans = DestroyGraph(Graphs.elem[num].G);
                if(ans == OK) printf("图销毁成功！\n");
                else printf("图销毁失败！\n");
                getchar();getchar();
                break;    
            case 3:
                //printf("\n----LocateVex功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
				printf("请输入想要查找的顶点关键字：");
	            scanf("%d", &e);
	            ans = LocateVex(Graphs.elem[num].G, e);
	            if (ans != -1) printf("图中关键字为%d的顶点的位序为%d\n", e, ans);
	            else
	                printf("图中不存在该顶点！\n");
                getchar();getchar();
                break;
           	case 4:
                //printf("\n----PutVex功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
				printf("请输入想要修改的顶点的关键字：");
                scanf("%d", &e);
	            printf("将其顶点值修改为：");
	            scanf("%d %s", &value.key, value.others);
	            ans = PutVex(Graphs.elem[num].G, e, value);
	            if (ans == ERROR)
	                printf("赋值操作失败！\n");
	            else if (ans == OK)
	                printf("已将关键字为%d的顶点值修改为%d,%s\n", e, value.key, value.others);
                getchar();getchar();
                break;
            case 5:
                //printf("\n----BiTreeDepth功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
				printf("请输入想要查找其第一邻接点的顶点：");
	            scanf("%d", &e);
	            ans = FirstAdjVex(Graphs.elem[num].G, e);
				if (ans != -1)
	                printf("顶点%d的第一邻接点的位序为%d\n关键字为：%d关键信息为：%s\n", e, ans, Graphs.elem[num].G.vertices[ans].data.key, Graphs.elem[num].G.vertices[ans].data.others);
	            else
	                printf("顶点%d没有第一邻接点！\n", e);
                getchar();getchar();
                break;
            case 6:
                //printf("\n----NextAdjVex功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入两个顶点的关键字：");
	            scanf("%d %d", &e, &j);
	            ans = NextAdjVex(Graphs.elem[num].G, e, j);
	            if (ans != -1)
	                printf("顶点%d相对于顶点%d的下一个邻接顶点位序为%d\n关键字为：%d关键信息为：%s\n", e, j, ans, Graphs.elem[num].G.vertices[ans].data.key, Graphs.elem[num].G.vertices[ans].data.others);
	            else printf("无下一邻接顶点！\n");
                getchar();getchar();
                break;
            case 7:
                //printf("\n----InsertVex功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入想要插入的关键字和关键信息：");
	            scanf("%d %s", &value.key, value.others);
	            ans = InsertVex(Graphs.elem[num].G, value);
	            if (ans == OK)
					printf("顶点 %d %s 已成功插入图中\n", value.key, value.others);
		        else if (ans == ERROR)
		            printf("插入失败！\n");
                getchar();getchar();
                break;
            case 8:
                //printf("\n----DeleteVex功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
	            printf("请输入想要删除的顶点的关键字：");
	            scanf("%d", &e);
	            ans = DeleteVex(Graphs.elem[num].G, e);
	            if (ans == OK)
	                printf("关键字为%d的顶点已从图中删除\n", e);
	            else if (ans == ERROR)
	                printf("删除失败！\n");
                getchar();getchar();
                break;
            case 9:
                //printf("\n----InsertArc功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入想要插入的弧：");
	            scanf("%d %d", &e, &j);
				ans = InsertArc(Graphs.elem[num].G, e, j);
	            if (ans == OK)
	                printf("插入成功！\n");
	            else if (ans == ERROR)
	                printf("插入失败！\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----DeleteArc功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入要删除弧的两个端点："); 
                scanf("%d %d", &e, &j);
	            ans = DeleteArc(Graphs.elem[num].G, e, j);
	            if (ans == OK)
	                printf("删除成功！\n");
	            else if (ans == ERROR)
	                printf("删除失败！\n");
                getchar();getchar();
                break;
            case 11:
                //printf("\n----DFSTraverse功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("深度优先搜索遍历：\n");
	            DFSTraverse(Graphs.elem[num].G, visit);
	            printf("\n");
                getchar();getchar();
                break;
            case 12:
                //printf("\n----BFSTraverse功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("广度优先搜索遍历：\n");
	            BFSTraverse(Graphs.elem[num].G, visit);
	            printf("\n");
                getchar();getchar();
                break;
            case 13:
                //printf("\n----SaveList功能待实现！\n");
                printf("请输入要保存的文件名称：");
                scanf("%s",FileName);
                ans = SaveGraph(Graphs.elem[num].G, FileName);
                if(ans == INFEASIBLE) printf("文件读入失败！\n");
                else if(ans == ERROR);
                else printf("文件读入成功！\n");
                getchar();getchar();
                break;
            case 14:
                //printf("\n----LoadList功能待实现！\n");
                printf("请输入要录入的文件名称：");
                scanf("%s", FileName);
                if(LoadGraph(Graphs.elem[num].G, FileName) == INFEASIBLE) printf("文件录入失败！\n");
                else printf("文件录入成功！\n");
                getchar();getchar();
                break;
            case 15:
                //printf("\n----AddList功能待实现！\n");
                if(Graphs.length == MAXlength) {
                    printf("多图表管理已满，请清除某些图后再操作！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入新增图的名称：");
                scanf("%s",Name);
                ans = AddList(Graphs, Name);
                if(ans == INFEASIBLE) printf("该名称的图已经存在!\n"); 
                else printf("%s已成功添加！\n",Name);
                getchar();getchar();
                break;
            case 16:
                //printf("\n---DestoryList功能待实现！\n");
                if(Graphs.length == 0) {
                    printf("多图表管理已空，请添加某些图后再操作！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入销毁图的名称：");
                scanf("%s",Name);
                ans = DestoryList(Graphs, Name);
                if(ans == OK)printf("%s已成功销毁！\n",Name);
                else printf("图不存在！\n");
                getchar();getchar();
                break;
            case 17:
                //printf("\n---LocateList功能待实现！\n");
                printf("请输入查找图的名称：");
                scanf("%s",Name);
                if(LocateList(Graphs, Name)) printf("该图的逻辑索引为：%d\n", LocateList(Graphs, Name));
                else printf("图查找失败！\n");
                getchar();getchar();
                break;
            case 18:
                //printf("\n----TraverseList功能待实现！\n");
                if(TraverseList(Graphs) == INFEASIBLE) printf("多图表为空！\n");
                getchar();getchar();
                break;
            case 19:
                //printf("\n----SelectList功能待实现！\n");
                printf("请选择要处理的图的逻辑索引：");
                scanf("%d", &ans);
                if(SelectList(Graphs, ans) == OK) {
                    printf("已选取成功！\n");
                    num = ans;
                }
                else printf("选取失败！\n");
                getchar();getchar();
                break;
            case 20:
                //printf("\n----VerticesSetLessThanK功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入顶点和距离："); 
                scanf("%d %d", &e, &j);
                VerticesSetLessThanK(Graphs.elem[num].G ,e,j);
                getchar();getchar();
                break;
            case 21:
                //printf("\n----ShortestPathLength功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                printf("请输入顶点v和顶点w："); 
                scanf("%d %d", &e, &j);
                if(e == j){
                	printf("请不要输入两个相同的结点！\n"); 
				}
				ans = ShortestPathLength(Graphs.elem[num].G,e,j);
				if(ans == Graphs.elem[num].G.vexnum){
					printf("两者间不存在路径！\n");
                    getchar();getchar();
                    break;
				}
				if(ans == ERROR){
					printf("两顶点不都存在！\n");
                    getchar();getchar();
                    break;
				}
                printf("两节点之间的最短路径为：%d\n", ans);
                getchar();getchar();
                break;
            case 22:
                //printf("\n----ConnectedComponentsNums功能待实现！\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("图为空！\n");
                    getchar();getchar();
                    break;
                }
                ans = ConnectedComponentsNums(Graphs.elem[num].G);
                printf("连通分量包含%d个！\n", ans);
                getchar();getchar();
                break;  
            case 0:
                break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    return 0;
}//end of main()





