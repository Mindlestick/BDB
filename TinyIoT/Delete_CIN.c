#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include "onem2m.h"

int main() {
    CIN* cin = Delete_CIN("4-20220513093154147745");
    display("CIN.db");

    fprintf(stderr, "[Delete Object]\n");
    fprintf(stderr, "ri : %s\nrn : %s\npi : %s\net : %s\n",
        cin->ri, cin->rn, cin->pi, cin->et);

    return 0;
}

CIN* Delete_CIN(char* ri) {
    printf("[Delete CIN] ri = %s\n", ri);

    //store CIN
    CIN* new_cin = (CIN*)malloc(sizeof(CIN));

    char* database = "CIN.db";

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
    // 몇번째 CIN인지 찾기 위한 커서
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
                new_cin->ri = malloc(data.size);
                strcpy(new_cin->ri, data.data);
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
    int cin_rn = 0;
    int cin_pi = 0;
    int cin_ty = 0;
    int cin_et = 0;
    int cin_lt = 0;
    int cin_ct = 0;
    int cin_st = 0;
    int cin_cs = 0;
    int cin_con = 0;
    int cin_csi = 0;

    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }
    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "rn", key.size) == 0) {
            cin_rn++;
            if (cin_rn == idx) {
                new_cin->rn = malloc(data.size);
                strcpy(new_cin->rn, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "pi", key.size) == 0) {
            cin_pi++;
            if (cin_pi == idx) {
                new_cin->pi = malloc(data.size);
                strcpy(new_cin->pi, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "et", key.size) == 0) {
            cin_et++;
            if (cin_et == idx) {
                new_cin->et = malloc(data.size);
                strcpy(new_cin->et, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "lt", key.size) == 0) {
            cin_lt++;
            if (cin_lt == idx) {
                new_cin->lt = malloc(data.size);
                strcpy(new_cin->lt, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "ct", key.size) == 0) {
            cin_ct++;
            if (cin_ct == idx) {
                new_cin->ct = malloc(data.size);
                strcpy(new_cin->ct, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "con", key.size) == 0) {
            cin_con++;
            if (cin_con == idx) {
                new_cin->con = malloc(data.size);
                strcpy(new_cin->con, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "csi", key.size) == 0) {
            cin_csi++;
            if (cin_csi == idx) {
                new_cin->csi = malloc(data.size);
                strcpy(new_cin->csi, data.data);
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "ty", key.size) == 0) {
            cin_ty++;
            if (cin_ty == idx) {
                new_cin->ty = *(int*)data.data;
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "st", key.size) == 0) {
            cin_st++;
            if (cin_st == idx) {
                new_cin->st = *(int*)data.data;
                dbcp->del(dbcp, 0);
            }
        }
        if (strncmp(key.data, "cs", key.size) == 0) {
            cin_cs++;
            if (cin_cs == idx) {
                new_cin->cs = *(int*)data.data;
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
    

    return new_cin;
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