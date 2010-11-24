#!/bin/sh

cd `dirname $0`

if [ "x$1" = "x" ]; then
    echo "Usage: `basename $0` DB_TYPE";
    echo "       DB_TYPE: [psql, mysql, oracle, sqlite]";
    exit 1;
fi

######################
# PSQL specific part #
######################
psqlSetup()
{
    HOST=localhost
    DBUSER=music_user
    DBNAME=music_db
    DBPASS=qwe
    LANG=plpgsql
    PSQL_DIR=/opt/rubystack-2.1-0/postgresql
    PSQL_SUPERUSER=postgres

    # drop db and user if they exist
    $PSQL_DIR/bin/dropdb --host=$HOST -U $PSQL_SUPERUSER $DBNAME
    $PSQL_DIR/bin/dropuser --host=$HOST -U $PSQL_SUPERUSER $DBUSER

    # create user if it doesn't exist already
    $PSQL_DIR/bin/psql template1 --host=$HOST -U $PSQL_SUPERUSER -c "\du" | grep $DBUSER > /dev/null
    if [ $? != 0 ]; then
        $PSQL_DIR/bin/createuser --host=$HOST -P -W -U $PSQL_SUPERUSER --createdb --no-superuser --no-createrole $DBUSER

        if [ $? != 0 ]; then
            echo "Unable to create db user!";
            exit 1;
        fi
    fi

    # create db if it doesn't exist already
    $PSQL_DIR/bin/psql --host=$HOST template1 $PSQL_SUPERUSER -c "\l" | grep $DBNAME > /dev/null
    if [ $? != 0 ]; then
        $PSQL_DIR/bin/createdb --host=$HOST --owner=$DBUSER --username=$PSQL_SUPERUSER --template=template1 $DBNAME

        if [ $? != 0 ]; then
            echo "Unable to create db!";
            exit 1;
        fi
    fi

    # create lang if it doesn't exist already
    $PSQL_DIR/bin/createlang --host=$HOST --username=$PSQL_SUPERUSER --dbname=$DBNAME -l | grep $LANG > /dev/null
    if [ $? != 0 ]; then
        $PSQL_DIR/bin/createlang --host=$HOST --username=$PSQL_SUPERUSER --dbname=$DBNAME $LANG

        if [ $? != 0 ]; then
            echo "Unable to create lang!";
            exit 1;
        fi
    fi

    SQLS="sql/psql/drop.sql \
          sql/psql/schemas.sql \
          sql/psql/tables.sql \
          sql/psql/views.sql \
          sql/psql/triggers.sql \
          sql/psql/indices.sql \
          sql/psql/procedures.sql \
          sql/psql/data.sql"

    for i in $SQLS; do
        echo "Applying $i on behalf of $DBUSER";
        $PSQL_DIR/bin/psql --host=$HOST -f $i --username=$DBUSER $DBNAME > /dev/null

        # no way check the last cmd for failure, somehow psql returns 0 on error

    done
}

#######################
# MYSQL specific part #
#######################
mysqlSetup()
{
    exit -1;
}

########################
# SQLITE specific part #
########################
sqliteSetp()
{
    exit -1;
}

########################
# ORACLE specific part #
########################
oracleSetup()
{
    exit -1;
}

########
# MAIN #
########

case $1 in
    psql)
        psqlSetup;
        ;;
    mysql)
        mysqlSetup;
        ;;
    oracle)
        oracleSetup;
        ;;
    sqlite)
        sqliteSetup;
        ;;
    *)
        echo "[ERROR] Wrong db type!"
        exit 1;
    ;;
esac

exit 0;
