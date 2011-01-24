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

# force release build of snapshot
sed -e 's/CONFIG.*= debug/CONFIG \+= release/g' $TMP_DIR/src/src.pro > $TMP_DIR/src/src.pro.new
mv $TMP_DIR/src/src.pro.new $TMP_DIR/src/src.pro

# force release build of dbobjects
mv $TMP_DIR/src/dbobjects/common/common.pro $TMP_DIR/src/dbobjects/common/common.pro.new
echo "CONFIG += release" > $TMP_DIR/src/dbobjects/common/common.pro
cat $TMP_DIR/src/dbobjects/common/common.pro.new >> $TMP_DIR/src/dbobjects/common/common.pro
rm $TMP_DIR/src/dbobjects/common/common.pro.new

mv $TMP_DIR/src/dbobjects/factory/factory.pro $TMP_DIR/src/dbobjects/factory/factory.pro.new
echo "CONFIG += release" > $TMP_DIR/src/dbobjects/factory/factory.pro
cat $TMP_DIR/src/dbobjects/factory/factory.pro.new >> $TMP_DIR/src/dbobjects/factory/factory.pro
rm $TMP_DIR/src/dbobjects/factory/factory.pro.new

mv $TMP_DIR/src/dbobjects/psql/psql.pro $TMP_DIR/src/dbobjects/psql/psql.pro.new
echo "CONFIG += release" > $TMP_DIR/src/dbobjects/psql/psql.pro
cat $TMP_DIR/src/dbobjects/psql/psql.pro.new >> $TMP_DIR/src/dbobjects/psql/psql.pro
rm $TMP_DIR/src/dbobjects/psql/psql.pro.new

mv $TMP_DIR/src/dbobjects/mysql/mysql.pro $TMP_DIR/src/dbobjects/mysql/mysql.pro.new
echo "CONFIG += release" > $TMP_DIR/src/dbobjects/mysql/mysql.pro
cat $TMP_DIR/src/dbobjects/mysql/mysql.pro.new >> $TMP_DIR/src/dbobjects/mysql/mysql.pro
rm $TMP_DIR/src/dbobjects/mysql/mysql.pro.new

# clean docs
rm -rf $TMP_DIR/doc/Manual.odt
rm -rf $TMP_DIR/doc/pages
rm -rf $TMP_DIR/doc/images
rm -rf $TMP_DIR/doc/vdb.sdx

tar cfvz ${TMP_DIR}_snapshot_`date "+%Y%m%d"`.tar.gz $TMP_DIR

rm -fr $TMP_DIR

