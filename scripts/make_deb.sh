#!/bin/sh

SCRIPT_FILE=`basename $0`

if [ ! -f $SCRIPT_FILE ]; then
    echo "Has to be run from visual-db/scripts";
fi

if [ -d pkg ]; then
    rm -rf pkg;
fi

mkdir -p pkg
mkdir -p pkg/usr
mkdir -p pkg/lib

# copy bin
cp -r ../bin pkg/usr/

# copy psql plugin
#find /usr -name libqsqlpsql.so -type f | xargs cp pkg/lib/
PSQL_LIB=/usr/lib/qt4/plugins/sqldrivers/libqsqlpsql.so
cp $PSQL_LIB pkg/lib/

# create debian-binary
echo "2.0" > pkg/debian-binary

# create control file
CONTROL_FILE=pkg/DEBIAN/control
mkdir pkg/DEBIAN
touch $CONTROL_FILE

echo "Package: visual-db" >> $CONTROL_FILE
echo "Version: 1.0" >> $CONTROL_FILE
echo "Section: database" >> $CONTROL_FILE
echo "Priority: optional" >> $CONTROL_FILE
echo "Architecture: ia32" >> $CONTROL_FILE
echo "Essential:" >> $CONTROL_FILE
echo "Depends: qt4" >> $CONTROL_FILE
echo "Pre-Depends:" >> $CONTROL_FILE
echo "Recommends:" >> $CONTROL_FILE
echo "Suggests:" >> $CONTROL_FILE
echo "Installed-Size:" >> $CONTROL_FILE
echo "Maintainer: lyuts <dioxinu@gmail.com>" >> $CONTROL_FILE
echo "Conflicts:" >> $CONTROL_FILE
echo "Replaces:" >> $CONTROL_FILE
echo "Provides:" >> $CONTROL_FILE
echo "Description: Database structure visualization tool" >> $CONTROL_FILE

dpkg-deb --build pkg .

rm -rf pkg
