#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include "onem2m.h"

int display(char* database);

int main() {
    CNT* cnt = Delete_CNT("3-20220513093154147745");
    display("CNT.db");

    fprintf(stderr, "[Delete Object]\n");
    fprintf(stderr, "ri : %s\nrn : %s\npi : %s\net : %s\n",
        cnt->ri, cnt->rn, cnt->pi, cnt->et);

    return 0;
}

CNT* Delete_CNT(char* ri) {
    printf("[Delete CNT] ri = %s\n", ri);

    //store CNT
    CNT* new_cnt = (CNT*)malloc(sizeof(CNT));

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

    /* Open the database. */
    ret = dbp->open(dbp, NULL, database, NULL, DB_BTREE, DB_CREATE, 0664);
    if (ret) {
        dbp->err(dbp, ret, "%s", database);
        exit(1);
    }


    /* Initialize the key/data return pair. */
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    int idx = 0;
    int flag = 0;
    // 몇번째 CNT인지 찾기 위한 커서
    DBC* dbcp0;
    if ((ret = dbp->cursor(dbp, NULL, &dbcp0, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }

    while ((ret = dbcp0->get(dbcp0, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "ri", key.size) == 0) {
            idx++;
            if (strncmp(data.data, ri, data.size) == 0) {
                flag = 1;
                new_cnt->ri = malloc(data.size);
                strcpy(new_cnt->ri, data.data);
                dbcp0->del(dbcp0, 0);
                break;
            }
        }
    }
    if (flag == 0) {
        printf("Not Found\n");
        return NULL;
        exit(1);
    }

    // 해당 index에 순서의 값 찾아 지움
    int cnt_rn = 0;
    int cnt_pi = 0;
    int cnt_ty = 0;
    int cnt_et = 0;
    int cnt_lt = 0;
    int cnt_ct = 0;
    int cnt_st = 0;
    int cnt_cni = 0;
    int cnt_cbs = 0;

    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }
    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "rn", key.size) == 0) {
            cnt_rn++;
            if (cnt_rn == idx) {
                new_cnt->rn = malloc(data.size);
                strcpy(new_cnt->rn, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "pi", key.size) == 0) {
            cnt_pi++;
            if (cnt_pi == idx) {
                new_cnt->pi = malloc(data.size);
                strcpy(new_cnt->pi, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "et", key.size) == 0) {
            cnt_et++;
            if (cnt_et == idx) {
                new_cnt->et = malloc(data.size);
                strcpy(new_cnt->et, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "lt", key.size) == 0) {
            cnt_lt++;
            if (cnt_lt == idx) {
                new_cnt->lt = malloc(data.size);
                strcpy(new_cnt->lt, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "ct", key.size) == 0) {
            cnt_ct++;
            if (cnt_ct == idx) {
                new_cnt->ct = malloc(data.size);
                strcpy(new_cnt->ct, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "ty", key.size) == 0) {
            cnt_ty++;
            if (cnt_ty == idx) {
                new_cnt->ty = *(int*)data.data;
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "st", key.size) == 0) {
            cnt_st++;
            if (cnt_st == idx) {
                new_cnt->st = *(int*)data.data;
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "cbs", key.size) == 0) {
            cnt_cbs++;
            if (cnt_cbs == idx) {
                new_cnt->cbs = *(int*)data.data;
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "cni", key.size) == 0) {
            cnt_cni++;
            if (cnt_cni == idx) {
                new_cnt->cni = *(int*)data.data;
                dbcp->del(dbcp, 0);
            }
        }

    }


    /* Cursors must be closed */
    if (dbcp0 != NULL)
        dbcp0->close(dbcp0);
    if (dbcp != NULL)
        dbcp->close(dbcp);
    if (dbp != NULL)
        dbp->close(dbp, 0);
    

    return new_cnt;
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