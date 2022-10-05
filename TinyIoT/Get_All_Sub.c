#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <math.h>
#include <time.h>

#include "onem2m.h"

//-lm
int NetToBit(char* net);
int main() {
    Node* sub = Get_All_Sub();

    while (sub) {
        fprintf(stderr, "%s %s %s %d %s %s\n", sub->ri,sub->rn,sub->nu,sub->net,sub->pi,sub->sur);
        sub = sub->siblingRight;
    }
    
    return 0;
}

int NetToBit(char* net) {
    int netLen = strlen(net);
    int ret = 0;

    for (int i = 0; i < netLen; i++) {
        int exp = atoi(net + i);
        if (exp > 0) ret = (ret | (int)pow(2, exp - 1));
    }

    return ret;
}

Node* Get_All_Sub(){
    char* database = "SUB.db";

    DB* dbp;
    DBC* dbcp;
    DBT key, data;
    int ret;

    /* Open the database. */
    if ((ret = db_create(&dbp, NULL, 0)) != 0) {
        fprintf(stderr,
            "%s: db_create: %s\n", database, db_strerror(ret));
        return 0;
    }

    ret = dbp->open(dbp, NULL, database, NULL, DB_BTREE, DB_CREATE, 0664);
    if (ret) {
        dbp->err(dbp, ret, "%s", database);
        exit(1);
    }

    /* Acquire a cursor for the database. */
    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }

    /* Initialize the key/data return pair. */
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    int cnt = 0;
    int idx = 0;
    int cnt_sub = 0;

    DBC* dbcp0;
    if ((ret = dbp->cursor(dbp, NULL, &dbcp0, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }
    while ((ret = dbcp0->get(dbcp0, &key, &data, DB_NEXT)) == 0) {
        cnt++;
    }
    if (cnt == 0) {
        fprintf(stderr, "Data not exist\n");
        return NULL;
    }

    int struct_size = 10;
    cnt = cnt / struct_size;
    char* tmp;


    Node* head = (Node*)calloc(cnt, sizeof(Node));
    Node* node;
    node = head;
    //node_ri = node_pi = node_rn = node_nu = node_sub_bit = head;
    
    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        switch (idx) {
            case 0:
                node->ri = malloc(data.size);
                strcpy(node->ri, data.data);

                node->siblingRight = (Node*)malloc(sizeof(Node));
                node->siblingRight->siblingLeft = node;

                idx++;
                break;
            case 1:
                node->rn = malloc(data.size);
                strcpy(node->rn, data.data);

                idx++;
                break;
            case 2:
                node->nu = malloc(data.size);
                strcpy(node->nu, data.data);

                idx++;
                break;
            case 3:
                //tmp = malloc(data.size);
                //strcpy(tmp, data.data);
                node->net = NetToBit(data.data);

                //node->net = malloc(data.size);
                //strcpy(node->net, data.data);

                idx++;
                break;
            case 4:
                node->sur = malloc(data.size);
                strcpy(node->sur, data.data);

                idx++;
                break;
            case 5:
                node->pi = malloc(key.size);
                strcpy(node->pi, key.data);

                node = node->siblingRight;
                idx++;
                break;
            default:
                idx++;
                if (idx == struct_size) idx = 0;
        }
    }

    node->siblingLeft->siblingRight = NULL;
    free(node);
    node = NULL;

    /* DB close */
    dbcp->close(dbcp0);
    dbcp->close(dbcp);
    dbp->close(dbp, 0);

    return head;
}