#include <AppearancePageTest.h>
#include <ArrowItemTest.h>
#include <ColorsPageTest.h>
#include <ControlWidgetTest.h>
#include <DbParametersTest.h>
#include <DescriptionWidgetTest.h>
#include <GraphicsItemTest.h>
#include <GraphicsSceneTest.h>
#include <GraphicsViewTest.h>
#include <LegendTest.h>
#include <MainWindowTest.h>
#include <OptionsDialogTest.h>
#include <PreferencesPageTest.h>
#include <ProxyParametersTest.h>
#include <SceneWidgetTest.h>
#include <SelectColorWidgetTest.h>
#include <SqlConnectionDialogTest.h>
#include <SqlWidgetTest.h>
#include <TableItemGroupTest.h>
#include <TableItemTest.h>
#include <TabWidgetTest.h>
#include <TreeWidgetTest.h>

/* dbobjects */
#include <DatabaseTest.h>
#include <DbIndexTest.h>
#include <DbLanguageTest.h>
#include <DbObjectTest.h>
#include <DbProcedureTest.h>
#include <DbRoleTest.h>
#include <DbSchemaTest.h>
#include <DbTableTest.h>
#include <DbTriggerTest.h>
#include <DbViewTest.h>
#include <MysqlTableTest.h>
#include <PsqlIndexTest.h>
#include <PsqlLanguageTest.h>
#include <PsqlProcedureTest.h>
#include <PsqlRoleTest.h>
#include <PsqlTableTest.h>
#include <PsqlTriggerTest.h>
#include <PsqlViewTest.h>

int main(int argc, char **argv)
{
    AppearancePageTest appearancePageTest;
    QTest::qExec(&appearancePageTest);

    ArrowItemTest arrowItemTest;
    QTest::qExec(&arrowItemTest);

    ColorsPageTest colorsPageTest;
    QTest::qExec(&colorsPageTest);

    ControlWidgetTest controlWidgetTest;
    QTest::qExec(&controlWidgetTest);
    
    DbParametersTest dbParametersTest ;
    QTest::qExec(&dbParametersTest);
    
    DescriptionWidgetTest descriptionWidgetTest;
    QTest::qExec(&descriptionWidgetTest);
    
    GraphicsItemTest graphicsItemTest;
    QTest::qExec(&graphicsItemTest);
    
    GraphicsSceneTest graphicsSceneTest;
    QTest::qExec(&graphicsSceneTest);
    
    GraphicsViewTest graphicsViewTest;
    QTest::qExec(&graphicsViewTest);
    
    LegendTest legendTest;
    QTest::qExec(&legendTest);

    MainWindowTest mainWindowTest;
    QTest::qExec(&mainWindowTest);
    
    OptionsDialogTest optionsDialogTest;
    QTest::qExec(&optionsDialogTest);
    
    PreferencesPageTest preferencesPageTest;
    QTest::qExec(&preferencesPageTest);
    
    ProxyParametersTest proxyParametersTest;
    QTest::qExec(&proxyParametersTest);
    
    SceneWidgetTest sceneWidgetTest;
    QTest::qExec(&sceneWidgetTest);
    
    SelectColorWidgetTest selectColorWidgetTest;
    QTest::qExec(&selectColorWidgetTest);
    
    SqlConnectionDialogTest sqlConnectionDialogTest;
    QTest::qExec(&sqlConnectionDialogTest);
    
    SqlWidgetTest sqlWidgetTest;
    QTest::qExec(&sqlWidgetTest);
    
    TableItemGroupTest tableItemGroupTest;
    QTest::qExec(&tableItemGroupTest);
    
    TableItemTest tableItemTest;
    QTest::qExec(&tableItemTest);
    
    TabWidgetTest tabWidgetTest;
    QTest::qExec(&tabWidgetTest);
    
    TreeWidgetTest treeWidgetTest;
    QTest::qExec(&treeWidgetTest);

    /* dbobjects */
    DatabaseTest databaseTest;
    QTest::qExec(&databaseTest);

    DbIndexTest dbIndexTest;
    QTest::qExec(&dbIndexTest);
    
    DbLanguageTest dbLanguageTest;
    QTest::qExec(&dbLanguageTest);
    
    DbObjectTest dbObjectTest;
    QTest::qExec(&dbObjectTest);
    
    DbProcedureTest dbProcedureTest;
    QTest::qExec(&dbProcedureTest);
    
    DbRoleTest dbRoleTest;
    QTest::qExec(&dbRoleTest);
    
    DbSchemaTest dbSchemaTest;
    QTest::qExec(&dbSchemaTest);
    
    DbTableTest dbTableTest;
    QTest::qExec(&dbTableTest);
    
    DbTriggerTest dbTriggerTest;
    QTest::qExec(&dbTriggerTest);
    
    DbViewTest dbViewTest;
    QTest::qExec(&dbViewTest);
    
    MysqlTableTest mysqlTableTest;
    QTest::qExec(&mysqlTableTest);
    
    PsqlIndexTest psqlIndexTest;
    QTest::qExec(&psqlIndexTest);
    
    PsqlLanguageTest psqlLanguageTest;
    QTest::qExec(&psqlLanguageTest);
    
    PsqlProcedureTest psqlProcedureTest;
    QTest::qExec(&psqlProcedureTest);
    
    PsqlRoleTest psqlRoleTest;
    QTest::qExec(&psqlRoleTest);
    
    PsqlTableTest psqlTableTest;
    QTest::qExec(&psqlTableTest);
    
    PsqlTriggerTest psqlTriggerTest;
    QTest::qExec(&psqlTriggerTest);
    
    PsqlViewTest psqlViewTest;
    QTest::qExec(&psqlViewTest);

}
