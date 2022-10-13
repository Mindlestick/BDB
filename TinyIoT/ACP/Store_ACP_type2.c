#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <ctype.h>

#include "onem2m.h"

int main() {
    ACP acp;

    //input sample
    acp.rn = "acp1";
    acp.ri = "5-20191210093452845";
    acp.pi = "pipipi";
    acp.ty = 5;
    acp.ct = "20191210T093452";
    acp.lt = "20191210T093452";
    acp.et = "20211210T093452";
    acp.pv_acor = "CAE1";
    acp.pv_acop = "2";
    acp.pvs_acor = "SM";
    acp.pvs_acop = "63";


    // [success -> 1] 
    if(Store_ACP(&acp)) fprintf(stderr, "store success!\n");

    // print
    char* DATABASE = "ACP_type2.db";
    display(DATABASE);

    return 0;
}

int Store_ACP(ACP *acp_object) {
    char* DATABASE = "ACP_type2.db";

    DB* dbp;    // db handle
    DBC* dbcp;
    DBT key, data;  // storving key and real data
    int ret;        // template value

    DBT key_ri;
    DBT data_ct, data_lt, data_et, data_rn, data_pi, data_ty,
        data_pv_acor,data_pv_acop,data_pvs_acor,data_pvs_acop;  // storving key and real data


    // if input == NULL
    if (acp_object->ri == NULL) {
        fprintf(stderr, "ri is NULL\n");
        return 0;
    }
    if (acp_object->rn == NULL) acp_object->rn = "";
    if (acp_object->pi == NULL) acp_object->pi = "NULL";
    if (acp_object->ty == '\0') acp_object->ty = -1;
    if (acp_object->ct == NULL) acp_object->ct = "";
    if (acp_object->lt == NULL) acp_object->lt = "";
    if (acp_object->et == NULL) acp_object->et = "";

   if (acp_object->pv_acor == NULL) acp_object->pv_acor = "";       
   if (acp_object->pv_acop == NULL) acp_object->pv_acop = ""; 
   if (acp_object->pvs_acop == NULL) acp_object->pvs_acor = ""; 
   if (acp_object->pvs_acop == NULL) acp_object->pvs_acop = ""; 
  
    ret = db_create(&dbp, NULL, 0);
    if (ret) {
        fprintf(stderr, "db_create : %s\n", db_strerror(ret));
        fprintf(stderr, "File ERROR\n");
        return 0;
    }

    /*Set duplicate*/
    ret = dbp->set_flags(dbp, DB_DUP);
    if (ret != 0) {
        dbp->err(dbp, ret, "Attempt to set DUPSORT flag failed.");
        fprintf(stderr, "Flag Set ERROR\n");
        dbp->close(dbp, 0);
        return(ret);
    }

    /*DB Open*/
    ret = dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0664);
    if (ret) {
        dbp->err(dbp, ret, "%s", DATABASE);
        fprintf(stderr, "DB Open ERROR\n");
        exit(1);
    }

    /*
  * The DB handle for a Btree database supporting duplicate data
  * items is the argument; acquire a cursor for the database.
  */
    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        fprintf(stderr, "Cursor ERROR");
        exit(1);
    }

    /* keyand data must initialize */
    memset(&key_ri, 0, sizeof(DBT));

    memset(&data_rn, 0, sizeof(DBT));
    memset(&data_pi, 0, sizeof(DBT));
    memset(&data_ty, 0, sizeof(DBT));
    memset(&data_ct, 0, sizeof(DBT));
    memset(&data_lt, 0, sizeof(DBT));
    memset(&data_et, 0, sizeof(DBT));
    memset(&data_pv_acor, 0, sizeof(DBT));
    memset(&data_pv_acop, 0, sizeof(DBT));
    memset(&data_pvs_acor, 0, sizeof(DBT));
    memset(&data_pvs_acop, 0, sizeof(DBT));

    /* initialize the data to be the first of two duplicate records. */
    key_ri.data = acp_object->ri;
    key_ri.size = strlen(acp_object->ri) + 1;

    data_rn.data = acp_object->rn;
    data_rn.size = strlen(acp_object->rn) + 1;

    data_pi.data = acp_object->pi;
    data_pi.size = strlen(acp_object->pi) + 1;

    data_ct.data = acp_object->ct;
    data_ct.size = strlen(acp_object->ct) + 1;

    data_lt.data = acp_object->lt;
    data_lt.size = strlen(acp_object->lt) + 1;    

    data_et.data = acp_object->et;
    data_et.size = strlen(acp_object->et) + 1;

    data_ty.data = &acp_object->ty;
    data_ty.size = sizeof(acp_object->ty);

    data_pv_acor.data = acp_object->pv_acor;
    data_pv_acor.size = strlen(acp_object->pv_acor) + 1;

    data_pv_acop.data = acp_object->pvs_acop;
    data_pv_acop.size = strlen(acp_object->pv_acop) + 1;

    data_pvs_acor.data = acp_object->pvs_acor;
    data_pvs_acor.size = strlen(acp_object->pvs_acor) + 1;

    data_pvs_acop.data = acp_object->pvs_acop;
    data_pvs_acop.size = strlen(acp_object->pvs_acop) + 1;


    /* input DB */
    if ((ret = dbcp->put(dbcp, &key_ri, &data_rn, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_pi, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_ty, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_ct, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_lt, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_et, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");

    if ((ret = dbcp->put(dbcp, &key_ri, &data_pv_acor, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_pv_acop, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_pvs_acor, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");
    if ((ret = dbcp->put(dbcp, &key_ri, &data_pvs_acop, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "db->cursor");

    /* DB close */
    dbcp->close(dbcp);
    dbp->close(dbp, 0); 

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
            strncmp(key.data, "cs", key.size) == 0
            ){
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