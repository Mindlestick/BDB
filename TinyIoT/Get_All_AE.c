#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include "onem2m.h"


int main() {

    Node* ae = Get_All_AE();
    while (ae) {
        fprintf(stderr, "%s\n", ae->rn);
        ae = ae->siblingRight;
    }

    return 0;
}

Node* Get_All_AE() {
    fprintf(stderr, "[Get All AE]\n");

    char* database = "AE.db";

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
    // 몇번째 AE인지 찾기 위한 커서
    DBC* dbcp0;
    if ((ret = dbp->cursor(dbp, NULL, &dbcp0, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }
    while ((ret = dbcp0->get(dbcp0, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "ri", key.size) == 0) {
            cnt++; // 오브젝트 개수
        }
    }
    //fprintf(stderr, "<%d>\n",cnt);

    if (cnt == 0) {
        fprintf(stderr, "Data not exist\n");
        return NULL;
        exit(1);
    }

    // cnt 개수만큼 동적할당
    Node* head = (Node*)malloc(sizeof(Node));
    Node* node_ri;
    Node* node_pi;
    Node* node_rn;
    Node* node_ty;

    node_ri = node_pi = node_rn = node_ty = head;

    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "pi", key.size) == 0) {
            node_pi->pi = malloc(data.size);
            strcpy(node_pi->pi, data.data);
            node_pi->siblingRight = (Node*)malloc(sizeof(Node));
            node_pi->siblingRight->siblingLeft = node_pi;
            node_pi = node_pi->siblingRight;
        }
        if (strncmp(key.data, "ri", key.size) == 0) {
            node_ri->ri = malloc(data.size);
            strcpy(node_ri->ri, data.data);
            node_ri = node_ri->siblingRight;

        }
        if (strncmp(key.data, "rn", key.size) == 0) {
            node_rn->rn = malloc(data.size);
            strcpy(node_rn->rn, data.data);
            node_rn = node_rn->siblingRight;
        }
        if (strncmp(key.data, "ty", key.size) == 0) {
            node_ty->ty = *(int*)data.data;
            node_ty = node_ty->siblingRight;
        }
    }

    node_pi->siblingLeft->siblingRight = NULL;
    free(node_pi);
    node_ri = node_pi = node_rn = node_ty = NULL;

    if (ret != DB_NOTFOUND) {
        dbp->err(dbp, ret, "DBcursor->get");
        fprintf(stderr, "Cursor ERROR\n");
        exit(0);
    }

    return head;
}