#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>

#define DATABASE "access1.db"

int main() {

    DB *dbp;    // db handle
    DBC* dbcp;
    FILE* error_file_pointer;
    DBT key, data;  // storving key and real data
    int ret;        // template value

    char* keys, * datas, *datas1;
    char* program_name = "my_prog";

    keys = "key1";
    datas = "data2";
    datas1 = "data3";

    ret = db_create(&dbp, NULL, 0);
    if (ret) {
        fprintf(stderr, "db_create : %s\n", db_strerror(ret));
        exit(1);
    }

    dbp->set_errfile(dbp, error_file_pointer);
    dbp->set_errpfx(dbp, program_name);

    ret = dbp->set_flags(dbp, DB_DUP);
    if (ret != 0) {
        dbp->err(dbp, ret, "Attempt to set DUPSORT flag failed.");
        dbp->close(dbp, 0);
        return(ret);
    }

    ret = dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0664);
    if (ret) {
        dbp->err(dbp, ret, "%s", DATABASE);
        exit(1);
    }

    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        printf("Cursor ERROR");
        exit(1);
    }

    // key and data must initialize
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    key.data = keys;
    key.size = strlen(keys)+1;
    data.data = datas;
    data.size = strlen(datas)+1;

    if ((ret = dbcp->put(dbcp, &key, &data, DB_KEYFIRST)) != 0)
        dbp->err(dbp, ret, "DB->cursor");

    data.data = datas1;
    data.size = strlen(datas1) + 1;

    if ((ret = dbcp->put(dbcp, &key, &data, DB_KEYLAST)) != 0)
        dbp->err(dbp, ret, "DB->cursor");

    /*
    ret = dbp->put(dbp, NULL, &key, &data, 0);
    if (ret) {
        dbp->err(dbp, ret, "DB->put");
        exit(1);
    }
    printf("db : %s : store key \n", (char*)key.data);
    */
    dbp->close(dbp, 0);

    return 0;
}

