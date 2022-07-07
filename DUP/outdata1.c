#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>

#define DATABASE "access1.db"

int main() {

    DBC* dbcp;
    DB* dbp;
    DBT key, data;
    int close_db, close_dbc, ret;

    close_db = close_dbc = 0;
    char* progname = DATABASE;

    /* Open the database. */
    if ((ret = db_create(&dbp, NULL, 0)) != 0) {
        fprintf(stderr,
            "%s: db_create: %s\n", progname, db_strerror(ret));
        return (1);
    }
    close_db = 1;

    dbp->set_errfile(dbp, stderr);
    dbp->set_errpfx(dbp, progname);

    if ((ret = dbp->open(dbp, NULL, DATABASE, NULL,
        DB_UNKNOWN, DB_RDONLY, 0)) != 0) {
        dbp->err(dbp, ret, "%s: DB->open", DATABASE);
        goto err;
    }

    if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
        dbp->err(dbp, ret, "DB->cursor");
        goto err;
    }

    while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0)
        printf("%.*s : %.*s\n",
            (int)key.size, (char*)key.data,
            (int)data.size, (char*)data.data);
    if (ret != DB_NOTFOUND) {
        dbp->err(dbp, ret, "DBcursor->get");
        goto err;
    }

err:    if ((ret = dbcp->close(dbcp)) != 0)
dbp->err(dbp, ret, "DBcursor->close");

return (0);
}
