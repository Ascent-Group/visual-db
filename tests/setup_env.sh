#!/bin/sh

cd `dirname $0`

DBUSER=music_user
DBNAME=music_db
LANG=plpgsql

# create user if it doesn't exist already
psql template1 -c "\du" | grep $DBUSER > /dev/null
if [ $? != 0 ]; then
    createuser --createdb --no-superuser --no-createrole $DBUSER

    if [ $? != 0 ]; then
	echo "Unable to create db user!";
        exit 1;
    fi
fi

# create db if it doesn't exist already
psql template1 -c "\l" | grep $DBNAME > /dev/null
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

SQLS="sql/drop.sql \
     sql/schemas.sql \
     sql/tables.sql \
     sql/views.sql \
     sql/triggers.sql \
     sql/indices.sql \
     sql/procedures.sql \
     sql/data.sql"

for i in $SQLS; do
    echo "Applying $i";
    psql -f $i $DBNAME $DBUSER > /dev/null

    # no way check the last cmd for failure, somehow psql returns 0 on error

done

exit 0;
