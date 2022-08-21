#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include "onem2m.h"

int main() {

    AE ae_before;
    ae_before.rn = "Sensor1_update";
    ae_before.ty = 3;
    ae_before.pi = "5-20191210093452845";
    ae_before.ri = "TAE2";
    ae_before.ct = "20220513T083900";
    ae_before.lt = "20220513T083900";
    ae_before.et = "20240513T083900";
    ae_before.api = "tinyProject1_update";
    ae_before.rr = false;
    ae_before.aei = "TAE1";

    AE *ae_after = DB_Update_AE(&ae_before);
    display("AE.db");

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

AE* DB_Update_AE(AE* ae) {

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
            if (strncmp(data.data, ae->ri, data.size) == 0) {
                cnt++; // update할 AE의 ri가 존재하면 cnt > 0
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
    int cnt_api = 0;
    int cnt_aei = 0;
    int cnt_rr = 0;
    
    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "rn", key.size) == 0) {
            cnt_rn++;
            if (cnt_rn == idx) {
                data.size = strlen(ae->rn) + 1;
                strcpy(data.data, ae->rn);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "pi", key.size) == 0) {
            cnt_pi++;
            if (cnt_pi == idx) {
                data.size = strlen(ae->pi) + 1;
                strcpy(data.data, ae->pi);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "api", key.size) == 0) {
            cnt_api++;
            if (cnt_api == idx) {
                data.size = strlen(ae->api) + 1;
                strcpy(data.data, ae->api);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "aei", key.size) == 0) {
            cnt_aei++;
            if (cnt_aei == idx) {
                data.size = strlen(ae->aei) + 1;
                strcpy(data.data, ae->aei);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "et", key.size) == 0) {
            cnt_et++;
            if (cnt_et == idx) {
                data.size = strlen(ae->et) + 1;
                strcpy(data.data, ae->et);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "lt", key.size) == 0) {
            cnt_lt++;
            if (cnt_lt == idx) {
                data.size = strlen(ae->lt) + 1;
                strcpy(data.data, ae->lt);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "ct", key.size) == 0) {
            cnt_ct++;
            if (cnt_ct == idx) {
                data.size = strlen(ae->ct) + 1;
                strcpy(data.data, ae->ct);
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "ty", key.size) == 0) {
            cnt_ty++;
            if (cnt_ty == idx) {
                *(int*)data.data = ae->ty;
                dbcp->put(dbcp, &key, &data, DB_CURRENT);
            }
        }
        if (strncmp(key.data, "rr", key.size) == 0) {
            cnt_rr++;
            if (cnt_rr == idx) {
                *(bool*)data.data = ae->rr;
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

    return ae;
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