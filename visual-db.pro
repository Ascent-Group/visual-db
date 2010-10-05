TEMPLATE = subdirs

SUBDIRS +=  $$PWD/src/dbobjects \
            $$PWD/src

CONFIG += ordered warn_on

QMAKE_EXTRA_TARGETS += build_tests run_tests

#build_tests.CONFIG = <nothing_here_yet>
build_tests.commands = @echo "Building tests" \
                       && $(QMAKE) $$PWD/tests/tests.pro \
                       && make

run_tests.commands = @echo "Running tests" \
                     && env VDB_DB_DRV=QPSQL ./tests/tests
run_tests.depends = build_tests

