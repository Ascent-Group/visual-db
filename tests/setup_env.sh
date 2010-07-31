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
    DBUSER=music_user
    DBNAME=music_db
    LANG=plpgsql

    # create user if it doesn't exist already
    psql template1 postgres -c "\du" | grep $DBUSER > /dev/null
    if [ $? != 0 ]; then
        createuser -U postgres --createdb --no-superuser --no-createrole $DBUSER

        if [ $? != 0 ]; then
            echo "Unable to create db user!";
            exit 1;
        fi
    fi

    # create db if it doesn't exist already
    psql template1 postgres -c "\l" | grep $DBNAME > /dev/null
    if [ $? != 0 ]; then
        createdb --host=localhost --username=$DBUSER --template=template1 $DBNAME

        if [ $? != 0 ]; then
            echo "Unable to create db!";
            exit 1;
        fi
    fi

    # create lang if it doesn't exist already
    createlang --host=localhost --username=$DBUSER --dbname=$DBNAME -l | grep $LANG > /dev/null
    if [ $? != 0 ]; then
        createlang --host=localhost --username=$DBUSER --dbname=$DBNAME $LANG

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
        echo "Applying $i";
        psql -f $i $DBNAME $DBUSER > /dev/null

        # no way check the last cmd for failure, somehow psql returns 0 on error

    done
}

#######################
# MYSQL specific part #
#######################
mysqlSetup()
{

}

########################
# SQLITE specific part #
########################
sqliteSetp()
{

}

########################
# ORACLE specific part #
########################
oracleSetup()
{

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
