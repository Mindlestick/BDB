#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <time.h>
#include "onem2m.h"

Node* Get_CIN_Range(char* now);
char* Get_LocalTime();

int main() {
    char* now = Get_LocalTime();

    Node* cin = Get_CIN_Range(now);
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

Node* Get_CIN_Range(char* now) {
    //fprintf(stderr, "[Get_CIN_Range]\n");
    char year[5], mon[3], day[3], hour[3], minute[3], sec[3];
    
    strncpy(year, now, 4);
    year[4] = '\0';
    strncpy(mon, now + 4, 2);
    mon[2] = '\0';
    strncat(day, now + 6, 2);
    day[2] = '\0';
    //printf("<%s/%s/%s>\n", year, mon, day);

    strncpy(hour, now + 9, 2);
    hour[2] = '\0';
    strncpy(minute, now + 11, 2);
    minute[2] = '\0';
    strncpy(sec, now + 13, 2);
    sec[2] = '\0';
    //printf("<%s/%s/%s>\n", hour, minute, sec);

    strcpy(day, "07");
    char* search = (char*)calloc(18, sizeof(char));
    //strcat(search_past, "4-");
    strcat(search, year);
    strcat(search, mon);
    strcat(search, day);
    strcat(search, "T");
    strcat(search, hour);
    strcat(search, minute);
    strcat(search, sec);

    fprintf(stderr, "<%s ~ %s>\n",now, search);


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
    // 몇번째 CIN인지 찾기 위한 커서
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

    Node* head = (Node*)malloc(sizeof(Node));
    Node* node_ri;
    Node* node_pi;
    Node* node_rn;
    Node* node_ty;
    Node* node_lt;

    node_ri = node_pi = node_rn = node_ty = node_lt = head;
    int* arr = NULL;
    int idx = 0;
    arr = (int*)malloc(sizeof(int) * cnt);

    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0) {
        if (strncmp(key.data, "lt", key.size) == 0) {
            //printf("(%d)\n", strncmp(search, data.data, 16)); //10 : day, 16: time
            if(strncmp(search, data.data, 16) <= 0 && strncmp(now, data.data, 16) >=0 )
                arr[idx] = 1;
            idx++;
        }
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
    //for (int i = 0; i < 5; i++) printf("%d ", arr[i]);

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