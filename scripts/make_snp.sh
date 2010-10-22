#!/bin/sh

TMP_DIR=visual-db

cd `dirname $0`

echo "FETCHING SNAPSHOT FROM SVN..."
svn export https://visiual-db.svn.sourceforge.net/svnroot/visiual-db/trunk $TMP_DIR

if [ $? != 0 ]; then
    echo "[ERROR] Unable to svn co!"
    exit -1;
fi

RM_DIRS="tests scripts html latex bin plugins"

for d in $RM_DIRS; do
    find $TMP_DIR/ -name $d -type d | xargs rm -rf
done

tar cfvz ${TMP_DIR}_snapshot_`date "+%Y%m%d"`.tar.gz $TMP_DIR

rm -fr $TMP_DIR

