#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include "onem2m.h"

int main() {

    CNT cnt_before;
    cnt_before.rn = "status2_update";
    cnt_before.ty = 3;
    cnt_before.pi = "TAE1";
    cnt_before.ri = "3-20210513093154147745";
    cnt_before.ct = "202105T093154";
    cnt_before.lt = "202105T093154";
    cnt_before.et = "202105T093154";
    cnt_before.cni = -1;
    cnt_before.cbs = -1;
    cnt_before.st = -1;

    CNT* cnt_after = DB_Update_CNT(&cnt_before);
    display("CNT.db");


    return 0;
}

void Free_Node(Node* node) {
    free(node->ri);
    free(node->rn);
    free(node->pi);
    free(node);
}
Node* Create_Node(char* ri, char* rn, char* pi, ObjectType ty) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->rn = (char*)malloc(sizeof(rn));
    node->ri = (char*)malloc(sizeof(ri));
    node->pi = (char*)malloc(sizeof(pi));
    strcpy(node->rn, rn);
    strcpy(node->ri, ri);
    strcpy(node->pi, pi);
    node->parent = NULL;
    node->child = NULL;
    node->siblingLeft = NULL;
    node->siblingRight = NULL;
    node->ty = ty;
    if (strcmp(rn, "") && strcmp(rn, "TinyIoT")) {
        fprintf(stderr, "\nCreate Tree Node\n[rn] %s\n[ri] %s\n", node->rn, node->ri);
    }
    return node;
}


/*
    CNT 업데이트 구독 되어있는지 확인하는 함수
    구독 되어있으면 -> 해당 sub 반환
    구독 되어있지 않으면 -> NULL 반환
*/
int Notification(char* ri, char event_num,) {

    char* database = "CNT.db";

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

    // 수정할 오브젝트가 몇번째인지 찾기 위한 커서
    DBC* dbcp0;
    if ((ret = dbp->cursor(dbp, NULL, &dbcp0, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }
    while ((ret = dbcp0->get(dbcp0, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "ri", key.size) == 0) {
            idx++;
            if (strncmp(data.data, cnt_object->ri, data.size) == 0) {
                cnt++; // update할 CNT의 ri가 존재하면 cnt > 0
                break;
            }
        }
    }

    // 인자로 들어온 ri가 존재하지 않으면 NULL 반환
    if (cnt == 0) {
        fprintf(stderr, "Data not exist\n");
        return NULL;
        exit(1);
    }

    int cnt_rn = 0;
    int cnt_pi = 0;
    int cnt_ty = 0;
    int cnt_et = 0;
    int cnt_lt = 0;
    int cnt_ct = 0;
    int cnt_cni = 0;
    int cnt_cbs = 0;
    int cnt_st = 0;

    
    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "rn", key.size) == 0) {
            cnt_rn++;
            if (cnt_rn == idx) {
                data.size = strlen(cnt_object->rn) + 1;
                strcpy(data.data, cnt_object->rn);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }

        if (strncmp(key.data, "pi", key.size) == 0) {
            cnt_pi++;
            if (cnt_pi == idx) {
                data.size = strlen(cnt_object->pi) + 1;
                strcpy(data.data, cnt_object->pi);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "et", key.size) == 0) {
            cnt_et++;
            if (cnt_et == idx) {
                data.size = strlen(cnt_object->et) + 1;
                strcpy(data.data, cnt_object->et);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "lt", key.size) == 0) {
            cnt_lt++;
            if (cnt_lt == idx) {
                data.size = strlen(cnt_object->lt) + 1;
                strcpy(data.data, cnt_object->lt);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "ct", key.size) == 0) {
            cnt_ct++;
            if (cnt_ct == idx) {
                data.size = strlen(cnt_object->ct) + 1;
                strcpy(data.data, cnt_object->ct);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "ty", key.size) == 0) {
            cnt_ty++;
            if (cnt_ty == idx) {
                *(int*)data.data = cnt_object->ty;
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "cni", key.size) == 0) {
            cnt_cni++;
            if (cnt_cni == idx) {
                *(int*)data.data = cnt_object->cni;
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "cbs", key.size) == 0) {
            cnt_cbs++;
            if (cnt_cbs == idx) {
                *(int*)data.data = cnt_object->cbs;
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "st", key.size) == 0) {
            cnt_st++;
            if (cnt_st == idx) {
                *(int*)data.data = cnt_object->st;
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
    }

    if (ret != DB_NOTFOUND) {
        dbp->err(dbp, ret, "DBcursor->get");
        printf("Cursor ERROR\n");
        exit(0);
    }

    dbcp->close(dbcp);
    dbcp->close(dbcp0);
    dbp->close(dbp, 0); //DB close

    return cnt_object;
}

int display(char* database)
{
    printf("[Display] %s \n", database); //DB name print

    DB* dbp;
    DBC* dbcp;
    DBT key, data;
    int close_db, close_dbc, ret;

    close_db = close_dbc = 0;

    /* Open the database. */
    if ((ret = db_create(&dbp, NULL, 0)) != 0) {
        fprintf(stderr,
            "%s: db_create: %s\n", database, db_strerror(ret));
        return (1);
    }
    close_db = 1;

    /* Turn on additional error output. */
    dbp->set_errfile(dbp, stderr);
    dbp->set_errpfx(dbp, database);

    /* Open the database. */
    if ((ret = dbp->open(dbp, NULL, database, NULL,
        DB_UNKNOWN, DB_RDONLY, 0)) != 0) {
        dbp->err(dbp, ret, "%s: DB->open", database);
        goto err;
    }

    /* Acquire a cursor for the database. */
    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        goto err;
    }
    close_dbc = 1;

    /* Initialize the key/data return pair. */
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    /* Walk through the database and print out the key/data pairs. */
    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        //int
        if (strncmp(key.data, "ty", key.size) == 0 ||
            strncmp(key.data, "st", key.size) == 0 ||
            strncmp(key.data, "cni", key.size) == 0 ||
            strncmp(key.data, "cbs", key.size) == 0 ||
            strncmp(key.data, "cs", key.size) == 0
            ) {
            printf("%.*s : %d\n", (int)key.size, (char*)key.data, *(int*)data.data);
        }
        //bool
        else if (strncmp(key.data, "rr", key.size) == 0) {
            printf("%.*s : ", (int)key.size, (char*)key.data);
            if (*(bool*)data.data == true)
                printf("true\n");
            else
                printf("false\n");
        }

        //string
        else {
            printf("%.*s : %.*s\n",
                (int)key.size, (char*)key.data,
                (int)data.size, (char*)data.data);
        }
    }
    if (ret != DB_NOTFOUND) {
        dbp->err(dbp, ret, "DBcursor->get");
        printf("Cursor ERROR\n");
        exit(0);
    }


err:    if (close_dbc && (ret = dbcp->close(dbcp)) != 0)
dbp->err(dbp, ret, "DBcursor->close");
if (close_db && (ret = dbp->close(dbp, 0)) != 0)
fprintf(stderr,
    "%s: DB->close: %s\n", database, db_strerror(ret));
return (0);
}