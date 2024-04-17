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
} VertexType; //�������Ͷ���


typedef struct ArcNode {         //�������Ͷ���
	 int adjvex;              //����λ�ñ�� 
	 struct ArcNode  *nextarc;	   //��һ������ָ��
} ArcNode;

typedef struct VNode{				//ͷ��㼰���������Ͷ���
	 VertexType data;       	//������Ϣ
	 ArcNode *firstarc;      	 //ָ���һ����
	} VNode,AdjList[MAX_VERTEX_NUM];
	
typedef  struct {  //�ڽӱ�����Ͷ���
    AdjList vertices;     	 //ͷ�������
    int vexnum,arcnum;   	  //������������
    GraphKind  kind;        //ͼ������
   } ALGraph;


typedef struct {//ɭ�ֵĶ���
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
//�жϽ�㼯���Ƿ����ظ���� 
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
//�ж��Ƿ����ظ������ң�����ı� 
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
/*����V��VR����ͼT������OK�����V��VR����ȷ������ERROR
�������ͬ�Ĺؼ��֣�����ERROR��*/
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
/*��������ͼG,ɾ��G��ȫ������ͱ�*/
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
//����u��ͼG�в��Ҷ��㣬���ҳɹ�����λ�򣬷��򷵻�-1��
{
    int i;
    for(i = 0; i < G.vexnum; i++){
        if(G.vertices[i].data.key == u)
        return i;
    }
    return -1;

}



status PutVex(ALGraph &G,KeyType u,VertexType value)
//����u��ͼG�в��Ҷ��㣬���ҳɹ����ö���ֵ�޸ĳ�value������OK��
//�������ʧ�ܻ�ؼ��ֲ�Ψһ������ERROR
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
//����u��ͼG�в��Ҷ��㣬���ҳɹ����ض���u�ĵ�һ�ڽӶ���λ�򣬷��򷵻�-1��
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
//����u��ͼG�в��Ҷ��㣬���ҳɹ����ض���v���ڽӶ��������w����һ�ڽӶ����λ�򣬲���ʧ�ܷ���-1��
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
//��ͼG�в��붥��v���ɹ�����OK,���򷵻�ERROR
{
    if(G.vexnum == MAX_VERTEX_NUM) return ERROR;
    if(LocateVex(G, v.key) != -1) return ERROR;
    G.vertices[G.vexnum++].data = v;
    return OK;
    
}


status DeleteVex(ALGraph &G,KeyType v)
//��ͼG��ɾ���ؼ���v��Ӧ�Ķ����Լ���صĻ����ɹ�����OK,���򷵻�ERROR
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
//��ͼG�����ӻ�<v,w>���ɹ�����OK,���򷵻�ERROR
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
//��ͼG��ɾ����<v,w>���ɹ�����OK,���򷵻�ERROR
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
//��ͼG������������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
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
//��ͼG���й�������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
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
//��ͼ������д�뵽�ļ�FileName��
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
//�����ļ�FileName��ͼ���ݣ�����ͼ���ڽӱ�
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
// ��Ҫ��Graphs������һ������ΪListName�Ŀ�ͼ 
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
// Graphs��ɾ��һ������ΪListName��ͼ 
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
// ��Graphs�в���һ������ΪListName��ͼ���ɹ������߼���ţ����򷵻�0
{
    if(!Graphs.elem) return INFEASIBLE;//����δ���
    for(int i = 0; i < Graphs.length; i++)
        if(!strcmp(ListName, Graphs.elem[i].name))
            return i + 1;
    return 0;

}

status TraverseList(GRAPHS& Graphs){
// �����ͼ��Ϊ�գ�������ʾ��ͼ������ƣ�ÿ�����Ƽ��һ�񣬷���OK�������ͼ��Ϊ�գ�����INFEASIBLE��
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
// ����ͼ��ѡ��
    if(Graphs.length == 0) return INFEASIBLE;
    if(i < 1 || i > Graphs.length) return ERROR;
    num = i - 1;
    return OK;
}
//5 a 6 b 7 c 8 d 9 e 10 f 11 g -1 nil
//5 7 7 8 5 8 7 9 8 9 6 8 10 11 -1 -1

status VerticesSetLessThanK(ALGraph G, int v, int k){
//���������������Ϊk�Ľ�� 
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
    	printf("�벻Ҫ���Ҿ���Ϊ1���µĽ�㣡\n");
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
//�������·�� 
	int t[100] = {0}, i, j, target = G.vexnum;
	if((i = LocateVex(G, v)) == -1) return ERROR;
    if((j = LocateVex(G, w)) == -1) return ERROR;
	FindTarget(G, t, target, 0, i, j);
	return target;
}

status ConnectedComponentsNums(ALGraph G){
//������ͨ��֧��Ŀ 
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
        printf("    	  1. CreateCraph   ����ͼ                 12. BFSTraverse     ���������������\n");
        printf("    	  2. DestroyGraph  ����ͼ                 13. SaveGraph       ͼ�ļ�����\n");
        printf("    	  3. LocateVex     ���Ҷ���               14. LoadGraph       ͼ�ļ�¼��  \n");
        printf("    	  4. PutVex        ���㸳ֵ               15. AddList         ��ͼ�����  \n");
        printf("    	  5. FirstAdjVex   ��õ�һ�ڽӵ�         16. DestroyList     ��ͼ������    \n");
        printf("    	  6. NextAdjVex    �����һ�ڽӵ�         17. LocateList      ��ͼ��λ�ò���    \n");
        printf("          7. InsertVex     ���붥��               18. TraverseList    ��ͼ�����\n");
        printf("          8. DeleteVex     ɾ������               19. SelectList      ͼ����ѡ��\n");
        printf("          9. InsertArc     ���뻡                 20. VerticesSetLessThanK     ����С��k�Ķ��㼯��\n");
        printf("          10. DeleteArc   ɾ����                  21. ShortestPathLength       ��������·���ͳ���\n");
        printf("          11. DFSTraverse   ���������������      22. ConnectedComponentsNums  ͼ����ͨ����\n");
        printf("    	  0. Exit        �˳�\n");
        printf("          ----------------------------------------------------------------------------\n");
        printf("          ˵����ÿ�β������������ո�ȷ�ϲ��ܽ�����һ��������\n");
        printf("\n          ��ǰ�����Ķ�����Ϊ��");
        if(num < 1|| num > Graphs.length){
            if(num > Graphs.length){
                Graphs.elem[num].G.vexnum = 0;
                num = 0;
            }
            printf("Ĭ��ͼ");
            if(Graphs.elem[num].G.vexnum == 0)
                printf("(δ����)");
            printf("\n\n\n");
        }
        else{
        	printf("%s",Graphs.elem[num - 1].name);
        	if(Graphs.elem[num].G.vexnum == 0)
                printf("(δ����)");
            printf("\n\n\n");
		}   
        if(op > 22 || op < 0)
            printf("��һ�������������ݲ˵���ȷ���룡\n\n\n");
        printf("��ѡ����Ĳ���[0~22]:");
        scanf("%d",&op);
        switch(op){
            case 1:
                //printf("\n----CreateCraph���ܴ�ʵ�֣�\n");
                if(Graphs.elem[num].G.vexnum){
                    printf("��ͼ�Ѵ��ڣ�\n");
                    getchar();getchar();
                    break;
                }
                i = 0;
	            printf("�����붥������(-1 nil��Ϊ������־)��");
	            do{
	                scanf("%d%s", &V[i].key, V[i].others);
	            }while (V[i++].key != -1);
	            i = 0;
	            printf("�������ϵ�����У���-1 -1������");
	            do{
	                scanf("%d%d", &VR[i][0], &VR[i][1]);
	            }while (VR[i++][0] != -1);
	            if (CreateCraph(Graphs.elem[num].G, V, VR) == OK)
	                printf("ͼ�����ɹ���\n");
	            else
	                printf("ͼ����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 2:
                //printf("\n----DestroyGraph���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                ans = DestroyGraph(Graphs.elem[num].G);
                if(ans == OK) printf("ͼ���ٳɹ���\n");
                else printf("ͼ����ʧ�ܣ�\n");
                getchar();getchar();
                break;    
            case 3:
                //printf("\n----LocateVex���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
				printf("��������Ҫ���ҵĶ���ؼ��֣�");
	            scanf("%d", &e);
	            ans = LocateVex(Graphs.elem[num].G, e);
	            if (ans != -1) printf("ͼ�йؼ���Ϊ%d�Ķ����λ��Ϊ%d\n", e, ans);
	            else
	                printf("ͼ�в����ڸö��㣡\n");
                getchar();getchar();
                break;
           	case 4:
                //printf("\n----PutVex���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
				printf("��������Ҫ�޸ĵĶ���Ĺؼ��֣�");
                scanf("%d", &e);
	            printf("���䶥��ֵ�޸�Ϊ��");
	            scanf("%d %s", &value.key, value.others);
	            ans = PutVex(Graphs.elem[num].G, e, value);
	            if (ans == ERROR)
	                printf("��ֵ����ʧ�ܣ�\n");
	            else if (ans == OK)
	                printf("�ѽ��ؼ���Ϊ%d�Ķ���ֵ�޸�Ϊ%d,%s\n", e, value.key, value.others);
                getchar();getchar();
                break;
            case 5:
                //printf("\n----BiTreeDepth���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
				printf("��������Ҫ�������һ�ڽӵ�Ķ��㣺");
	            scanf("%d", &e);
	            ans = FirstAdjVex(Graphs.elem[num].G, e);
				if (ans != -1)
	                printf("����%d�ĵ�һ�ڽӵ��λ��Ϊ%d\n�ؼ���Ϊ��%d�ؼ���ϢΪ��%s\n", e, ans, Graphs.elem[num].G.vertices[ans].data.key, Graphs.elem[num].G.vertices[ans].data.others);
	            else
	                printf("����%dû�е�һ�ڽӵ㣡\n", e);
                getchar();getchar();
                break;
            case 6:
                //printf("\n----NextAdjVex���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������������Ĺؼ��֣�");
	            scanf("%d %d", &e, &j);
	            ans = NextAdjVex(Graphs.elem[num].G, e, j);
	            if (ans != -1)
	                printf("����%d����ڶ���%d����һ���ڽӶ���λ��Ϊ%d\n�ؼ���Ϊ��%d�ؼ���ϢΪ��%s\n", e, j, ans, Graphs.elem[num].G.vertices[ans].data.key, Graphs.elem[num].G.vertices[ans].data.others);
	            else printf("����һ�ڽӶ��㣡\n");
                getchar();getchar();
                break;
            case 7:
                //printf("\n----InsertVex���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������Ҫ����Ĺؼ��ֺ͹ؼ���Ϣ��");
	            scanf("%d %s", &value.key, value.others);
	            ans = InsertVex(Graphs.elem[num].G, value);
	            if (ans == OK)
					printf("���� %d %s �ѳɹ�����ͼ��\n", value.key, value.others);
		        else if (ans == ERROR)
		            printf("����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 8:
                //printf("\n----DeleteVex���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
	            printf("��������Ҫɾ���Ķ���Ĺؼ��֣�");
	            scanf("%d", &e);
	            ans = DeleteVex(Graphs.elem[num].G, e);
	            if (ans == OK)
	                printf("�ؼ���Ϊ%d�Ķ����Ѵ�ͼ��ɾ��\n", e);
	            else if (ans == ERROR)
	                printf("ɾ��ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 9:
                //printf("\n----InsertArc���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("��������Ҫ����Ļ���");
	            scanf("%d %d", &e, &j);
				ans = InsertArc(Graphs.elem[num].G, e, j);
	            if (ans == OK)
	                printf("����ɹ���\n");
	            else if (ans == ERROR)
	                printf("����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 10:
                //printf("\n----DeleteArc���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("������Ҫɾ�����������˵㣺"); 
                scanf("%d %d", &e, &j);
	            ans = DeleteArc(Graphs.elem[num].G, e, j);
	            if (ans == OK)
	                printf("ɾ���ɹ���\n");
	            else if (ans == ERROR)
	                printf("ɾ��ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 11:
                //printf("\n----DFSTraverse���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("�����������������\n");
	            DFSTraverse(Graphs.elem[num].G, visit);
	            printf("\n");
                getchar();getchar();
                break;
            case 12:
                //printf("\n----BFSTraverse���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("�����������������\n");
	            BFSTraverse(Graphs.elem[num].G, visit);
	            printf("\n");
                getchar();getchar();
                break;
            case 13:
                //printf("\n----SaveList���ܴ�ʵ�֣�\n");
                printf("������Ҫ������ļ����ƣ�");
                scanf("%s",FileName);
                ans = SaveGraph(Graphs.elem[num].G, FileName);
                if(ans == INFEASIBLE) printf("�ļ�����ʧ�ܣ�\n");
                else if(ans == ERROR);
                else printf("�ļ�����ɹ���\n");
                getchar();getchar();
                break;
            case 14:
                //printf("\n----LoadList���ܴ�ʵ�֣�\n");
                printf("������Ҫ¼����ļ����ƣ�");
                scanf("%s", FileName);
                if(LoadGraph(Graphs.elem[num].G, FileName) == INFEASIBLE) printf("�ļ�¼��ʧ�ܣ�\n");
                else printf("�ļ�¼��ɹ���\n");
                getchar();getchar();
                break;
            case 15:
                //printf("\n----AddList���ܴ�ʵ�֣�\n");
                if(Graphs.length == MAXlength) {
                    printf("��ͼ����������������ĳЩͼ���ٲ�����\n");
                    getchar();getchar();
                    break;
                }
                printf("����������ͼ�����ƣ�");
                scanf("%s",Name);
                ans = AddList(Graphs, Name);
                if(ans == INFEASIBLE) printf("�����Ƶ�ͼ�Ѿ�����!\n"); 
                else printf("%s�ѳɹ���ӣ�\n",Name);
                getchar();getchar();
                break;
            case 16:
                //printf("\n---DestoryList���ܴ�ʵ�֣�\n");
                if(Graphs.length == 0) {
                    printf("��ͼ������ѿգ������ĳЩͼ���ٲ�����\n");
                    getchar();getchar();
                    break;
                }
                printf("����������ͼ�����ƣ�");
                scanf("%s",Name);
                ans = DestoryList(Graphs, Name);
                if(ans == OK)printf("%s�ѳɹ����٣�\n",Name);
                else printf("ͼ�����ڣ�\n");
                getchar();getchar();
                break;
            case 17:
                //printf("\n---LocateList���ܴ�ʵ�֣�\n");
                printf("���������ͼ�����ƣ�");
                scanf("%s",Name);
                if(LocateList(Graphs, Name)) printf("��ͼ���߼�����Ϊ��%d\n", LocateList(Graphs, Name));
                else printf("ͼ����ʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 18:
                //printf("\n----TraverseList���ܴ�ʵ�֣�\n");
                if(TraverseList(Graphs) == INFEASIBLE) printf("��ͼ��Ϊ�գ�\n");
                getchar();getchar();
                break;
            case 19:
                //printf("\n----SelectList���ܴ�ʵ�֣�\n");
                printf("��ѡ��Ҫ�����ͼ���߼�������");
                scanf("%d", &ans);
                if(SelectList(Graphs, ans) == OK) {
                    printf("��ѡȡ�ɹ���\n");
                    num = ans;
                }
                else printf("ѡȡʧ�ܣ�\n");
                getchar();getchar();
                break;
            case 20:
                //printf("\n----VerticesSetLessThanK���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("�����붥��;��룺"); 
                scanf("%d %d", &e, &j);
                VerticesSetLessThanK(Graphs.elem[num].G ,e,j);
                getchar();getchar();
                break;
            case 21:
                //printf("\n----ShortestPathLength���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                printf("�����붥��v�Ͷ���w��"); 
                scanf("%d %d", &e, &j);
                if(e == j){
                	printf("�벻Ҫ����������ͬ�Ľ�㣡\n"); 
				}
				ans = ShortestPathLength(Graphs.elem[num].G,e,j);
				if(ans == Graphs.elem[num].G.vexnum){
					printf("���߼䲻����·����\n");
                    getchar();getchar();
                    break;
				}
				if(ans == ERROR){
					printf("�����㲻�����ڣ�\n");
                    getchar();getchar();
                    break;
				}
                printf("���ڵ�֮������·��Ϊ��%d\n", ans);
                getchar();getchar();
                break;
            case 22:
                //printf("\n----ConnectedComponentsNums���ܴ�ʵ�֣�\n");
                if(!Graphs.elem[num].G.vexnum) {
                    printf("ͼΪ�գ�\n");
                    getchar();getchar();
                    break;
                }
                ans = ConnectedComponentsNums(Graphs.elem[num].G);
                printf("��ͨ��������%d����\n", ans);
                getchar();getchar();
                break;  
            case 0:
                break;
        }//end of switch
    }//end of while
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
    return 0;
}//end of main()





