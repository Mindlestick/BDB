#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include "onem2m.h"

ACP* Get_ACP(char* ri);
int main() {
    ACP *acp1 = Get_ACP("1-20181210093452845"); 

    printf("[%s]\n",acp1->rn);
    return 0;
}

ACP* Get_ACP(char* ri) {
    char* database = "ACP_type2.db";

    //struct to return
    ACP* new_acp = (ACP*)malloc(sizeof(ACP));

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
    int flag = 0;
    //int struct_size = 10; // struct size-1

    DBC* dbcp0;
    if ((ret = dbp->cursor(dbp, NULL, &dbcp0, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }
    while ((ret = dbcp0->get(dbcp0, &key, &data, DB_NEXT)) == 0) {
        cnt++;
        if (strncmp(key.data, ri, key.size) == 0) {
            flag=1;
            break;
        }
    }
    if (cnt == 0 || flag==0) {
        fprintf(stderr, "Data not exist\n");
        return NULL;
    }

    int idx = 0;
    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, ri, key.size) == 0) {
            // ri = key
            new_acp->ri = malloc(key.size);
            strcpy(new_acp->ri, key.data);
            switch (idx) {
                case 0:
                    new_acp->rn = malloc(data.size);
                    strcpy(new_acp->rn, data.data);

                    idx++;
                    break;
                case 1:
                    new_acp->pi = malloc(data.size);
                    strcpy(new_acp->pi, data.data);

                    idx++;
                    break;
                case 2:
                    new_acp->ct = malloc(data.size);
                    strcpy(new_acp->ct, data.data);

                    idx++;
                    break;
                case 3:
                    new_acp->lt = malloc(data.size);
                    strcpy(new_acp->lt, data.data);

                    idx++;
                    break;
                case 4:
                    new_acp->et = malloc(data.size);
                    strcpy(new_acp->et, data.data);

                    idx++;
                    break;
                case 5:
                    new_acp->ty = *(int*)data.data;

                    idx++;
                    break;
                case 6:
                    new_acp->pv_acor = malloc(data.size);
                    strcpy(new_acp->pv_acor, data.data);

                    idx++;
                    break;
                case 7:
                    new_acp->pv_acop = malloc(data.size);
                    strcpy(new_acp->pv_acop, data.data);

                    idx++;
                    break;
                case 8:
                    new_acp->pvs_acor = malloc(data.size);
                    strcpy(new_acp->pvs_acor, data.data);

                    idx++;
                    break;
                case 9:
                    new_acp->pvs_acop = malloc(data.size);
                    strcpy(new_acp->pvs_acop, data.data);

                    idx++;
                    break;
                default:
                    idx=-1;
            }
        }

    }
    if (ret != DB_NOTFOUND) {
        dbp->err(dbp, ret, "DBcursor->get");
        fprintf(stderr, "Cursor ERROR\n");
        exit(0);
    }

        /* Cursors must be closed */
    if (dbcp0 != NULL)
        dbcp0->close(dbcp0);
    if (dbcp != NULL)
        dbcp->close(dbcp);
    if (dbp != NULL)
        dbp->close(dbp, 0);

    return new_acp;
}