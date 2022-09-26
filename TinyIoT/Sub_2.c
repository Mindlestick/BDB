#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <time.h>
#include "onem2m.h"
double start, end;

int Subscription_2(Sub* sub_object);
int main() {
    Sub sub1;
    Sub sub2;
    Sub sub3;

    sub1.rn = "sub1";
    sub1.ri = "23-2022040684653299304";
    sub1.pi = "3-20220406084023203796";
    sub1.nu = "http://223.131.176.101:3000/ct=json";
    sub1.net = "1";
    sub1.ct = "20220406T084653";
    sub1.et = "20220406T084653";
    sub1.lt = "20220406T084653";
    sub1.ty = 23;
    sub1.nct = 1;

    sub2.rn = "sub2";
    sub2.ri = "23-2021040684653299304";
    sub2.pi = "3-20220406084023203796";
    sub2.nu = "http://223.131.176.101:3000/ct=json";
    sub2.net = "2";
    sub2.ct = "20210406T084653";
    sub2.et = "20210406T084653";
    sub2.lt = "20210406T084653";
    sub2.ty = 23;
    sub2.nct = 1;

    sub3.rn = "sub3";
    sub3.ri = "23-2023040684653299304";
    sub3.pi = "3-20220406084023203796";
    sub3.nu = "http://223.131.176.101:3000/ct=json";
    sub3.net = "15";
    sub3.ct = "20210406T084653";
    sub3.et = "20210406T084653";
    sub3.lt = "20210406T084653";
    sub3.ty = 23;
    sub3.nct = 1;


    start = (double)clock() / CLOCKS_PER_SEC;
    Subscription_2(&sub1);
    Subscription_2(&sub2);
    Subscription_2(&sub3);
    end = (((double)clock()) / CLOCKS_PER_SEC);
    printf("Sub2_time :%lf\n", (end - start));

    display("SUB_2.db");

    return 0;
}

int Subscription_2(Sub* sub_object) {
    char* DATABASE = "SUB_2.db";

    DB* dbp;    // db handle
    DBC* dbcp;
    FILE* error_file_pointer;
    DBT key, data;  // storving key and real data
    int ret;        // template value

    DBT key_rn, key_ri, key_pi, key_nu, key_net, key_ct, key_et, key_lt, key_ty, key_nct;
    DBT data_rn, data_ri, data_pi, data_nu, data_net, data_ct, data_et, data_lt, data_ty, data_nct;  // storving key and real data

    char* program_name = "my_prog";

    // if input == NULL
    if (sub_object->rn == NULL) sub_object->rn = "";
    if (sub_object->ri == NULL) sub_object->ri = "";
    if (sub_object->pi == NULL) sub_object->pi = "";
    if (sub_object->nu == NULL) sub_object->nu = "";
    if (sub_object->net == NULL) sub_object->net = "1";
    if (sub_object->ct == NULL) sub_object->ct = "";
    if (sub_object->et == NULL) sub_object->et = "";
    if (sub_object->lt == NULL) sub_object->lt = "";
    if (sub_object->ty == '\0') sub_object->ty = 23;
    if (sub_object->nct == '\0') sub_object->nct = 0;


    ret = db_create(&dbp, NULL, 0);
    if (ret) {
        fprintf(stderr, "db_create : %s\n", db_strerror(ret));
        printf("File ERROR\n");
        exit(1);
    }

    dbp->set_errfile(dbp, error_file_pointer);
    dbp->set_errpfx(dbp, program_name);

    /*Set duplicate*/
    ret = dbp->set_flags(dbp, DB_DUP);
    if (ret != 0) {
        dbp->err(dbp, ret, "Attempt to set DUPSORT flag failed.");
        printf("Flag Set ERROR\n");
        dbp->close(dbp, 0);
        return(ret);
    }

    /*DB Open*/
    ret = dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0664);
    if (ret) {
        dbp->err(dbp, ret, "%s", DATABASE);
        printf("DB Open ERROR\n");
        exit(1);
    }

    /*
  * The DB handle for a Btree database supporting duplicate data
  * items is the argument; acquire a cursor for the database.
  */
    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        printf("Cursor ERROR");
        exit(1);
    }

    /* keyand data must initialize */
    memset(&key_rn, 0, sizeof(DBT));
    memset(&key_ri, 0, sizeof(DBT));
    memset(&key_pi, 0, sizeof(DBT));
    memset(&key_nu, 0, sizeof(DBT));
    memset(&key_net, 0, sizeof(DBT));
    memset(&key_ct, 0, sizeof(DBT));
    memset(&key_et, 0, sizeof(DBT));
    memset(&key_lt, 0, sizeof(DBT));
    memset(&key_ty, 0, sizeof(DBT));
    memset(&key_nct, 0, sizeof(DBT));

    memset(&data_rn, 0, sizeof(DBT));
    memset(&data_ri, 0, sizeof(DBT));
    memset(&data_pi, 0, sizeof(DBT));
    memset(&data_nu, 0, sizeof(DBT));
    memset(&data_net, 0, sizeof(DBT));
    memset(&data_ct, 0, sizeof(DBT));
    memset(&data_et, 0, sizeof(DBT));
    memset(&data_lt, 0, sizeof(DBT));
    memset(&data_ty, 0, sizeof(DBT));
    memset(&data_nct, 0, sizeof(DBT));

    /* initialize the data to be the first of two duplicate records. */
    data_rn.data = sub_object->rn;
    data_rn.size = strlen(sub_object->rn) + 1;
    key_rn.data = "rn";
    key_rn.size = strlen("rn") + 1;

    data_ri.data = sub_object->ri;
    data_ri.size = strlen(sub_object->ri) + 1;
    key_ri.data = "ri";
    key_ri.size = strlen("ri") + 1;

    data_pi.data = sub_object->pi;
    data_pi.size = strlen(sub_object->pi) + 1;
    key_pi.data = "pi";
    key_pi.size = strlen("pi") + 1;

    data_nu.data = sub_object->nu;
    data_nu.size = strlen(sub_object->nu) + 1;
    key_nu.data = "nu";
    key_nu.size = strlen("nu") + 1;

    data_net.data = sub_object->net;
    data_net.size = strlen(sub_object->net) + 1;
    key_net.data = "net";
    key_net.size = strlen("net") + 1;

    data_ct.data = sub_object->ct;
    data_ct.size = strlen(sub_object->ct) + 1;
    key_ct.data = "ct";
    key_ct.size = strlen("ct") + 1;

    data_et.data = sub_object->et;
    data_et.size = strlen(sub_object->et) + 1;
    key_et.data = "et";
    key_et.size = strlen("et") + 1;

    data_lt.data = sub_object->lt;
    data_lt.size = strlen(sub_object->lt) + 1;
    key_lt.data = "lt";
    key_lt.size = strlen("lt") + 1;

    data_ty.data = &sub_object->ty;
    data_ty.size = sizeof(sub_object->ty) + 1;
    key_ty.data = "ty";
    key_ty.size = strlen("ty") + 1;

    data_nct.data = &sub_object->nct;
    data_nct.size = sizeof(sub_object->nct) + 1;
    key_nct.data = "nct";
    key_nct.size = strlen("nct") + 1;

    /* input DB */
    if ((ret = dbcp->put(dbcp, &key_rn, &data_rn, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_ri, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_pi, &data_pi, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_nu, &data_nu, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_net, &data_net, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ct, &data_ct, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_et, &data_et, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_lt, &data_lt, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ty, &data_ty, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_nct, &data_nct, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");

    dbcp->close(dbcp);
    dbp->close(dbp, 0); //DB close

    return 1;
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
        if (strncmp(key.data, "ty", key.size) == 0 ||
            strncmp(key.data, "st", key.size) == 0 ||
            strncmp(key.data, "cni", key.size) == 0 ||
            strncmp(key.data, "cbs", key.size) == 0 ||
            strncmp(key.data, "cs", key.size) == 0 ||
            strncmp(key.data, "nct", key.size) == 0
            ) {
            printf("%.*s : %d\n", (int)key.size, (char*)key.data, *(int*)data.data);
        }
        else if (strncmp(key.data, "rr", key.size) == 0) {
            printf("%.*s : ", (int)key.size, (char*)key.data);
            if (*(bool*)data.data == true)
                printf("true\n");
            else
                printf("false\n");
        }
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