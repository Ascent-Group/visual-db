#!/bin/sh

TMP_DIR=visual-db

cd `dirname $0`

echo "FETCHING SNAPSHOT FROM SVN..."
svn co https://visiual-db.svn.sourceforge.net/svnroot/visiual-db/trunk $TMP_DIR

if [ $? != 0 ]; then
    echo "[ERROR] Unable to svn co!"
    exit -1;
fi

RM_DIRS=".svn .obj .moc tests scripts html latex bin"
#RM_FILES="*.o *.a *~ *.so Makefile *Doxyfile* *.swp cscope* tags *core svn-commit*"

for d in $RM_DIRS; do
    find $TMP_DIR/ -name $d | xargs rm -rf
done

#for f in $RM_FILES; do
#    find $TMP_DIR/ -name '$f' | xargs rm
#done

tar cfvz ${TMP_DIR}_snapshot_`date "+%Y%m%d"`.tar.gz $TMP_DIR

rm -fr $TMP_DIR

