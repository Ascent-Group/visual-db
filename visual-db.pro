TEMPLATE = subdirs

SUBDIRS +=  $$PWD/src/dbobjects \
            $$PWD/src

CONFIG += ordered warn_on

QMAKE_EXTRA_TARGETS += build_tests run_tests docs

#build_tests.CONFIG = <nothing_here_yet>
build_tests.commands = @echo "Building tests" \
                       && $(QMAKE) $$PWD/tests/tests.pro \
                       && make -f $$OUT_PWD/tests/Makefile

run_tests.depends = build_tests
run_tests.commands = @echo "Running tests" \
                     && env VDB_DB_DRV=QPSQL ./tests/tests

# $$QMAKE_LIBDIR_QT = /path/to/qt/installation/lib
QTDIR = $$QMAKE_LIBDIR_QT/..

ASSISTANT = $$QTDIR/bin/assistant

QHP_NAMESPACE = "org.ascent.vdb"
QHP_VIRTUAL_FOLDER = "visual-db_0.1"
QCH_FILE = "visual-db.qch"

docs.commands = env SRC_DIR=$$PWD QTDIR=$$QTDIR QHP_NAMESPACE=$$QHP_NAMESPACE QHP_VIRTUAL_FOLDER=$$QHP_VIRTUAL_FOLDER QCH_FILE=$$QCH_FILE /usr/local/bin/doxygen $$PWD/Doxyfile \
                && $$ASSISTANT -unregister $$OUT_PWD/html/*.qch \
                && $$ASSISTANT -register $$OUT_PWD/html/*.qch

