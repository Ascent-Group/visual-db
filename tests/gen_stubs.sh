#!/bin/sh

gen_h() {
    IN_FILE=$1

    OUT_FILE=`basename $IN_FILE | sed 's/\.h/Test\.h/'`

    CLASS_NAME=`basename $OUT_FILE | sed 's/\.h//'`
    
    echo "Class: $CLASS_NAME File: $OUT_FILE";

    #license
    cat ../LICENSE > $OUT_FILE
    echo "" >> $OUT_FILE

    #ifndef/define
    echo "#ifndef `echo $CLASS_NAME | tr a-z A-Z`_H" >> $OUT_FILE
    echo "#define `echo $CLASS_NAME | tr a-z A-Z`_H" >> $OUT_FILE
    echo "" >> $OUT_FILE

    #include <QtTest/QtTest>
    echo "#include <QtTest/QtTest>" >> $OUT_FILE
    echo "" >> $OUT_FILE

    #declare class : public QObject
    echo "class $CLASS_NAME : public QObject" >> $OUT_FILE
    echo "{" >> $OUT_FILE
    
    #Q_OBJECT
    echo "    Q_OBJECT" >> $OUT_FILE
    echo "" >> $OUT_FILE
    
    #private slots
    echo "    private slots:" >> $OUT_FILE
    echo "        void initTestCase();" >> $OUT_FILE
    echo "        void cleanupTestCase();" >> $OUT_FILE
    echo "" >> $OUT_FILE

    FUNCS=`cat $IN_FILE | egrep "\(.*\).*;" | sed 's,//.*;.*,,g' | sed 's/(.*).*;/Test();/g' | sed -e 's/const//g' -e 's/virtual//g' -e 's/=.*0//g' -e 's/<.*>//g' | sed 's/[\*~]//g' | sed 's/static//g' | sed 's/&//g' | awk '{print $2;}' | grep -v "$CLASS_NAME(" | sort -u`

    for i in $FUNCS; do
    if [ "x`echo $i | tr -d ' ' | tr -d '\n'`" != "x" ]; then
        i=`echo $i | sed -e 's/operator\s*==/equals/g'`
        i=`echo $i | sed -e 's/operator\s*!=/notEquals/g'`
        i=`echo $i | sed -e 's/operator\s*=/assignment/g'`

        echo "        void $i" >> $OUT_FILE
    fi
    done
    
    #
    echo "};" >> $OUT_FILE
    echo "" >> $OUT_FILE
    #endif // 
    echo "#endif // `echo $CLASS_NAME | tr a-z A-Z`_H" >> $OUT_FILE
    echo "" >> $OUT_FILE

}

gen_cpp() {
    IN_FILE=`basename $1 | sed 's/\.h/Test.h/g'`

    OUT_FILE=`echo $IN_FILE | sed 's/\.h/.cpp/'`

    CLASS_NAME=`basename $OUT_FILE | sed 's/\.cpp//'`

    echo "Class: $CLASS_NAME File: $OUT_FILE";

    #license
    cat ../LICENSE > $OUT_FILE
    echo "" >> $OUT_FILE

    #include
    echo "#include <$CLASS_NAME.h>" >> $OUT_FILE
    echo "" >> $OUT_FILE


#    FUNCS=`cat $IN_FILE | egrep "\(.*\).*;" | sed 's,//.*;.*,,g' | sed 's/(.*).*;/Test();/g' | sed -e 's/const//g' -e 's/virtual//g' -e 's/=.*0//g' -e 's/<.*>//g' | sed 's/[\*~]//g' | sed 's/static//g' | awk '{print $2;}'`
    FUNCS=`cat $IN_FILE | egrep "\(.*\).*;" | sed 's,//.*;.*,,g' | sed -e 's/const//g' -e 's/virtual//g' -e 's/=.*0//g' -e 's/<.*>//g' | sed 's/[\*~]//g' | sed 's/static//g' | sed 's/&//g' | awk '{print $2;}'`

    for i in $FUNCS; do
    if [ "x`echo $i | tr -d ' ' | tr -d '\n'`" != "x" ]; then
#        i=`echo $i | sed -e "s/void\s*/void\n$CLASS_NAME::/g"`
        i=`echo $i | sed -e "s/;//g"`

        echo "void" >> $OUT_FILE
        echo "$CLASS_NAME::$i" >> $OUT_FILE
        echo "{" >> $OUT_FILE
        echo "    QVERIFY(0);" >> $OUT_FILE
        echo "}" >> $OUT_FILE
        echo "" >> $OUT_FILE
    fi
    done

    exit 1;
}

########
# MAIN #
########
# ./gen_stubs [file.h]

cd `dirname $0`

if [ "x$1" != "x" ]; then
    gen_h $1
    gen_cpp $1
    exit 0;
fi

#for i in `ls ../src/gui/*.h`; do
#    gen_h $i
#done
#
#for i in `ls ../src/gui/behaviour/*.h`; do
#    gen_h $i
#done
#
#for i in `ls ../src/connect*.h`; do
#    gen_h $i
#done
#
#for i in `ls ../src/dbobjects/common/*.h`; do
#    gen_h $i
#done
#
#for i in `ls ../src/dbobjects/psql/*.h`; do
#    gen_h $i
#done
#
#for i in `ls ../src/dbobjects/mysql/*.h`; do
#    gen_h $i
#done

exit 0;
