#include <stdio.h>
#include <stdbool.h>
#include <db.h>

typedef enum {
	o_CREATE = 1,
	o_RETRIEVE,
	o_UPDATE,
	o_DELETE
}Operation;

typedef enum {
	t_AE = 2,
	t_CNT,
	t_CIN,
	t_CSE
}ObjectType;

// OneM2M Resource struct
typedef struct {
	char* ct;
	char* lt;
	char* rn;
	char* ri;
	char* pi;
	char* csi;
	int ty;
} CSE;

typedef struct {
	char* et;
	char* ct;
	char* lt;
	char* rn;
	char* ri;
	char* pi;
	char* api;
	char* aei;
	int ty;
	bool rr;
} AE;

typedef struct {
	char* et;
	char* ct;
	char* lt;
	char* rn;
	char* ri;
	char* pi;
	int ty;
	int st;
	int cni;
	int cbs;
} CNT;

typedef struct {
	char* et;
	char* ct;
	char* lt;
	char* rn;
	char* ri;
	char* pi;
	char* csi;
	char* con;
	int ty;
	int st;
	int cs;
} CIN;

typedef struct Node {
	struct Node* parent;
	struct Node* child;
	struct Node* siblingLeft;
	struct Node* siblingRight;

	char* rn;
	char* ri;
	char* pi;
	ObjectType ty;
}Node;

typedef struct {
	Node* root;
}RT;

//Request parse function
Operation Parse_Operation();
ObjectType Parse_ObjectType();
char* Parse_Request_JSON();

//OneM2M CRUD function
void Create_Object(char* json_payload, Node* pnode);
void Retrieve_Object(Node* pnode);
void Delete_Object();

void Create_AE(char* json_payload, Node* pnode);
CNT* Create_CNT(char* json_payload);
CIN* Create_CIN(char* json_payload);

void Retrieve_CSE();
void Retrieve_AE();
CNT* Retrieve_CNT();
CIN* Retrieve_CIN();

CSE* Update_CSE(char* json_payload);
AE* Update_AE(char* json_payload);
CNT* Update_CNT(char* json_payload);

CSE* JSON_to_CSE(char* json_payload);
AE* JSON_to_AE(char* json_payload);
CNT* JSON_to_CNT(char* json_payload);
CIN* JSON_to_CIN(char* json_payload);

char* CSE_to_json(CSE* cse_object);
char* AE_to_json(AE* ae_object);
char* CNT_to_json(CNT* cnt_object);
char* CIN_to_json(CIN* cin_object);

//DB function
int display(char* database);

int Store_CSE(CSE* cse_object);
int Store_AE(AE* ae_object);
int Store_CNT(CNT* cnt_object);
int Store_CIN(CIN* cin_object);

CSE* Get_CSE();
AE* Get_AE(char* ri);
CNT* Get_CNT(char* ri);
CIN* Get_CIN(char* ri);

CSE* Delete_CSE(char* ri);
AE* Delete_AE(char* ri);
CNT* Delete_CNT(char* ri);
CIN* Delete_CIN(char* ri);

Node* Get_All_CSE();
Node* Get_All_AE();
Node* Get_All_CNT();
Node* Get_All_CIN();

//Resource Tree function
void Set_AE(AE* ae, char* pi);

CSE* Get_sample_CSE();
AE* Get_sample_AE();
CNT* Get_sample_CNT();
CIN* Get_sample_CIN();

Node* Create_Node(char* ri, char* rn, char* pi, ObjectType ty);
Node* Validate_URI(RT* rt);
int Add_child(Node* parent, Node* child);
void Delete_Node(Node* node, int flag);

void tree_data(Node* node);
void Restruct_ResourceTree();
Node* Restruct_childs(Node* node, Node* list);
