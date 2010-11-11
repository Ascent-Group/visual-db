TEMPLATE = subdirs

SUBDIRS +=  $$PWD/src/dbobjects \
            $$PWD/src
#            $$PWD/tests

CONFIG += ordered warn_on

QMAKE_EXTRA_TARGETS += build_tests run_tests docs

### Target for building tests
#build_tests.CONFIG = <nothing_here_yet>
build_tests.commands = @echo "Building tests" \
                       && $(QMAKE) $$PWD/tests/tests.pro \
                       && make -f $$OUT_PWD/tests/Makefile

### Target for running tests
run_tests.depends = build_tests
run_tests.commands = @echo "Running tests" \
                     && env VDB_DB_DRV=QPSQL ./tests/tests

# $$QMAKE_LIBDIR_QT = /path/to/qt/installation/lib
QTDIR = $$QMAKE_LIBDIR_QT/..

ASSISTANT = $$QTDIR/bin/assistant

QHP_NAMESPACE = "org.ascent.vdb"
QHP_VIRTUAL_FOLDER = "visual-db_0.1"
QCH_FILE = "visual-db.qch"

### Target for generating docs and registering them with Qt Assistant
docs.commands = env SRC_DIR=$$PWD QTDIR=$$QTDIR QHP_NAMESPACE=$$QHP_NAMESPACE QHP_VIRTUAL_FOLDER=$$QHP_VIRTUAL_FOLDER QCH_FILE=$$QCH_FILE /usr/local/bin/doxygen $$PWD/Doxyfile \
                && $$ASSISTANT -unregister $$OUT_PWD/html/*.qch \
                && $$ASSISTANT -register $$OUT_PWD/html/*.qch

### Target for compilation and running test for compiled sources
#auto.commands = $(MAKE) | grep cpp | grep -v moc_ | awk "'{print \$$NF;}'" | xargs basename | sed -e 's/.cpp/Test/' | xargs env VDB_DB_DRV=QPSQL $$OUT_PWD/tests/tests
