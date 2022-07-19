#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include "onem2m.h"

int main() {
    AE **ae = Get_All_AE();
    if (ae == NULL || *ae == NULL) {
        fprintf(stderr, "Return NULL\n");
        return -1;
    }
    else {
        for (int i = 0; i < 3; i++) {
            fprintf(stderr, "ri : %s\nrn : %s\npi : %s\net : %s\naei : %s\napi : %s\nct : %s\nlt : %s\n",
                ae[i]->ri, ae[i]->rn, ae[i]->pi, ae[i]->et, ae[i]->aei,
                ae[i]->api, ae[i]->ct, ae[i]->lt);
            if (ae[i]->rr == 1) {
                fprintf(stderr, "rr : true\n");
            }
            fprintf(stderr, "ty : %d\n", ae[i]->ty);
        }

    }
    return 0;
}

AE** Get_All_AE(){
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
    AE** new_ae = (AE**)malloc(sizeof(AE*)*cnt);
    for (int i = 0; i < cnt; i++) {
        new_ae[i] = (AE*)malloc(sizeof(AE));
    }


    int cnt_ri = 0;
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
        if (strncmp(key.data, "ri", key.size) == 0) {
            new_ae[cnt_ri]->ri = malloc(data.size);
            strcpy(new_ae[cnt_ri]->ri, data.data);
            cnt_ri++;
        }
        if (strncmp(key.data, "rn", key.size) == 0) {
            new_ae[cnt_rn]->rn = malloc(data.size);
            strcpy(new_ae[cnt_rn]->rn, data.data);
            cnt_rn++;
        }
        if (strncmp(key.data, "pi", key.size) == 0) {
            new_ae[cnt_pi]->pi = malloc(data.size);
            strcpy(new_ae[cnt_pi]->pi, data.data);
            cnt_pi++;
        }
        if (strncmp(key.data, "api", key.size) == 0) {
            new_ae[cnt_api]->api = malloc(data.size);
            strcpy(new_ae[cnt_api]->api, data.data);
            cnt_api++;
        }
        if (strncmp(key.data, "aei", key.size) == 0) {
            new_ae[cnt_aei]->aei = malloc(data.size);
            strcpy(new_ae[cnt_aei]->aei, data.data);
            cnt_aei++;
        }
        if (strncmp(key.data, "et", key.size) == 0) {
            new_ae[cnt_et]->et = malloc(data.size);
            strcpy(new_ae[cnt_et]->et, data.data);
            cnt_et++;
        }
        if (strncmp(key.data, "lt", key.size) == 0) {
            new_ae[cnt_lt]->lt = malloc(data.size);
            strcpy(new_ae[cnt_lt]->lt, data.data);
            cnt_lt++;
        }
        if (strncmp(key.data, "ct", key.size) == 0) {
            new_ae[cnt_ct]->ct = malloc(data.size);
            strcpy(new_ae[cnt_ct]->ct, data.data);
            cnt_ct++;
        }
        if (strncmp(key.data, "ty", key.size) == 0) {
            new_ae[cnt_ty]->ty = *(int*)data.data;
            cnt_ty++;
        }
        if (strncmp(key.data, "rr", key.size) == 0) {
            new_ae[cnt_rr]->rr = *(bool*)data.data;
            cnt_rr++;
        }
    }

    if (ret != DB_NOTFOUND) {
        dbp->err(dbp, ret, "DBcursor->get");
        fprintf(stderr, "Cursor ERROR\n");
        exit(0);
    }

    return new_ae;
}