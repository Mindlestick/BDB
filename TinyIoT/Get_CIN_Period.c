#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <time.h>
#include "onem2m.h"

int main() {
    char* past = "20220807T222120";
    char* now = Get_LocalTime();

    Node* cin = Get_CIN_Period(past, now);
    while (cin) {
        fprintf(stderr, "%s\n", cin->rn);
        cin = cin->siblingRight;
    }

    return 0;
}

char* Get_LocalTime() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char year[5], mon[3], day[3], hour[3], minute[3], sec[3];

    sprintf(year, "%d", tm.tm_year + 1900);
    sprintf(mon, "%02d", tm.tm_mon + 1);
    sprintf(day, "%02d", tm.tm_mday);
    sprintf(hour, "%02d", tm.tm_hour);
    sprintf(minute, "%02d", tm.tm_min);
    sprintf(sec, "%02d", tm.tm_sec);

    char* now = (char*)calloc(16, sizeof(char));

    strcat(now, year);
    strcat(now, mon);
    strcat(now, day);
    strcat(now, "T");
    strcat(now, hour);
    strcat(now, minute);
    strcat(now, sec);

    return now;
}

Node* Get_CIN_Period(char* start_time, char* end_time) {
    //start_time과 end_time은 [20220807T215215] 형태
    fprintf(stderr, "<%s ~ %s>\n",start_time, end_time);

    // start_ri와 end_ri는 [4-20220808T113154] 형태
    char* start_ri = (char*)calloc(18, sizeof(char));
    char* end_ri = (char*)calloc(18, sizeof(char));

    strcat(start_ri, "4-");
    strcat(start_ri, start_time);

    strcat(end_ri, "4-");
    strcat(end_ri, end_time);

    //fprintf(stderr, "<%s ~ %s>\n", start_ri, end_ri);

    // DB 관련 함수
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
    int* arr = NULL;

    // 오브젝트가 몇개인지 찾기 위한 커서
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
    if (cnt == 0) {
        fprintf(stderr, "Data not exist\n");
        return NULL;
        exit(1);
    }
    //fprintf(stderr, "<%d>\n", cnt);

    //오브젝트 개수만큼 동적할당
    arr = (int*)malloc(sizeof(int) * cnt);
    for (int i = 0; i < cnt; i++) arr[i] = 0;

    // 해당하는 오브젝트가 몇개인지 찾기 위한 커서
    DBC* dbcp1;
    if ((ret = dbp->cursor(dbp, NULL, &dbcp1, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        exit(1);
    }
    // 해당하는 오브젝트 배열에 1로 표시 0 1 1 1 0 <- 두번째 세번째 네번째 오브젝트가 해당
    while ((ret = dbcp1->get(dbcp1, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "ri", key.size) == 0) {
            //9 : date, 16: time
            if (strncmp(start_ri, data.data, 16) <= 0 && strncmp(end_ri, data.data, 16) >= 0)
                arr[idx] = 1;
            idx++;
        }
    }
    //해당하는 오브젝트가 없음
    int sum = 0;
    for (int i = 0; i < cnt; i++) {
        sum += arr[i];
    }
    if (sum == 0) {
        fprintf(stderr, "Find data not exist\n");
        return NULL;
        exit(1);
    }
    //for (int i = 0; i < cnt; i++) printf("%d ", arr[i]);


    Node* head = (Node*)malloc(sizeof(Node));
    Node* node_ri;
    Node* node_pi;
    Node* node_rn;
    Node* node_ty;

    node_ri = node_pi = node_rn = node_ty = head;


    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "pi", key.size) == 0) {
            if (arr[idx % cnt]) {
                //printf("[%d]", idx % cnt);
                node_pi->pi = malloc(data.size);
                strcpy(node_pi->pi, data.data);
                node_pi->siblingRight = (Node*)malloc(sizeof(Node));
                node_pi->siblingRight->siblingLeft = node_pi;
                node_pi = node_pi->siblingRight;
            }
            idx++;
        }
        if (strncmp(key.data, "ri", key.size) == 0) {
            if (arr[idx % cnt]) {
                node_ri->ri = malloc(data.size);
                strcpy(node_ri->ri, data.data);
                node_ri = node_ri->siblingRight;
            }
            idx++;
        }
        if (strncmp(key.data, "rn", key.size) == 0) {
            if (arr[idx % cnt]) {
                node_rn->rn = malloc(data.size);
                strcpy(node_rn->rn, data.data);
                node_rn = node_rn->siblingRight;
            }
            idx++;
        }
        if (strncmp(key.data, "ty", key.size) == 0) {
            if (arr[idx % cnt]) {
                node_ty->ty = *(int*)data.data;
                node_ty = node_ty->siblingRight;
            }
            idx++;
        }
        
    }
    // for (int i = 0; i < cnt; i++) printf("%d ", arr[i]);

    node_pi->siblingLeft->siblingRight = NULL;
    free(node_pi);
    node_ri = node_pi = node_rn = node_ty = NULL;
    free(arr);

    if (ret != DB_NOTFOUND) {
        dbp->err(dbp, ret, "DBcursor->get");
        fprintf(stderr, "Cursor ERROR\n");
        exit(0);
    }

    return head;
}