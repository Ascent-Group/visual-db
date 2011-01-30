/*-
 * Copyright (c) 2009, Ascent Group.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of the Ascent Group nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <common/DbProcedure.h>
#include <common/DbRole.h>
#include <common/DbSchema.h>
#include <common/DbTrigger.h>
#include <common/DbView.h>
#include <factory/Procedure.h>
#include <factory/Table.h>
#include <factory/Trigger.h>
#include <factory/View.h>
#include <psql/Tools.h>

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * Constructor
 *
 * On creation automatically adds newly created schema object to
 * a Database's vector of schemas
 */
DbSchema::DbSchema(QString iName)
    : DbObject(iName),
      mOwner(0),
      mDescription(""),
      mTables(),
      mViews(),
      mProcedures(),
      mTriggers()
{
}

/*!
 * Destructor
 */
DbSchema::~DbSchema()
{
    resetData();
}

/*!
 * \brief Add table to a schema
 *
 * \param[in] iTable
 *
 * \return true - If the table object has been added
 * \return false - If the table object has alread existed in the vector
 */
bool
DbSchema::addTable(const DbTablePtr &iTable)
{
    if (mTables.contains(iTable)) {
        return false;
    }

//    iTable->setSchema(Database::instance()->findSchema(mName));

    mTables.push_back(iTable);
    return true;
}

/*!
 * \brief Add view to a schema
 *
 * \param[in] iView
 *
 * \return true - If the view object has been added
 * \return false - If the view object has already existed in the vector
 */
bool
DbSchema::addView(const DbViewPtr &iView)
{
    if (mViews.contains(iView)) {
        return false;
    }

//    iView->setSchema(Database::instance()->findSchema(mName));

    mViews.push_back(iView);
    return true;
}

/*!
 * \brief Add proc to a schema
 *
 * \param[in] iProc
 *
 * \return true - If the proc object has been added
 * \return false - If the proc object has already existed in the vector
 */
bool
DbSchema::addProcedure(const DbProcedurePtr &iProc)
{
    if (mProcedures.contains(iProc)) {
        return false;
    }

//    iProc->setSchema(Database::instance()->findSchema(mName));

    mProcedures.push_back(iProc);
    return true;
}

/*!
 * \brief Add trig to a schema
 *
 * \param[in] iTrig
 *
 * \return true - If the trigger object has been added
 * \return false - If the trigger object has laready existed in the vector
 */
bool
DbSchema::addTrigger(const DbTriggerPtr &iTrig)
{
    if (mTriggers.contains(iTrig)) {
        return false;
    }

//    iTrig->setSchema(Database::instance()->findSchema(mName));

    mTriggers.push_back(iTrig);
    return true;
}

/*!
 * Get a list of tables' names
 *
 * \param[out] oList - List of tables' names
 */
void
DbSchema::tablesList(QStringList &oList) const
{
    oList.clear();

    // \todo use foreach with const refs
    QVector<DbTablePtr>::const_iterator iter;

    for (iter = mTables.constBegin(); iter != mTables.constEnd(); ++iter) {
        oList.append(iter->name());
    }
}

/*!
 * Calculate the number of tables in the given schema
 *
 * \return Number of tables currently stored in the schema
 */
quint64
DbSchema::tablesCount() const
{
    return mTables.count();
}

/*!
 * Get a list of views' names
 *
 * \param[out] oList - List of views' names
 */
void
DbSchema::viewsList(QStringList &oList) const
{
    oList.clear();

    QVector<DbViewPtr>::const_iterator iter;

    for (iter = mViews.constBegin(); iter != mViews.constEnd(); ++iter) {
        oList.append(iter->name());
    }
}

/*!
 * Calculate the number of views in the given schema
 *
 * \return Number of views currently stored in the schema
 */
quint64
DbSchema::viewsCount() const
{
    return mViews.count();
}

/*!
 * Get a list of procs' names
 *
 * \param[out] oList - List of procs' names
 */
void
DbSchema::proceduresList(QStringList &oList) const
{
    oList.clear();

    QVector<DbProcedurePtr>::const_iterator iter;

    for (iter = mProcedures.constBegin(); iter != mProcedures.constEnd(); ++iter) {
        oList.append(iter->name());
    }
}

/*!
 * Get a list of triggers' names
 *
 * \param[out] oList - List of triggers' names
 */
void
DbSchema::triggersList(QStringList &oList) const
{
    oList.clear();

    QVector<DbTriggerPtr>::const_iterator iter;

    for (iter = mTriggers.constBegin(); iter != mTriggers.constEnd(); ++iter) {
        oList.append(iter->name());
    }
}

/*!
 * Calculate the number of procs in the given schema
 *
 * \return Number of procs currently stored in the schema
 */
quint64
DbSchema::proceduresCount() const
{
    return mProcedures.count();
}

/*!
 * Calculate the number of triggers in the given schema
 *
 * \return Number of triggers currently stored in the schema
 */
quint16
DbSchema::triggersCount() const
{
    return mTriggers.count();
}

/*!
 * Find table by its name
 *
 * \param[in] iTableName - Name of the table we are looking for
 *
 * \return Handle to the found table or NULL is not found.
 */
DbTablePtr
DbSchema::findTable(const QString &iTableName) const
{
    foreach (const DbTablePtr &table, mTables) {
        if (iTableName == table.name()) {
            return table;
        }
    }
    return DbTablePtr();
}

/*!
 * Find view by its name
 *
 * \param[in] iViewName - Name of the view we are looking for
 *
 * \return Handle to the found view or NULL is not found.
 */
DbViewPtr
DbSchema::findView(const QString &iViewName) const
{
    foreach (const DbViewPtr &view, mViews) {
        if (iViewName == view.name()) {
            return view;
        }
    }
    return DbViewPtr();
}

/*!
 * Find procedure by its name
 *
 * \param[in] iProcedureName - Name of the procedure we are looking for
 *
 * \return Handle to the found procedure or NULL is not found.
 */
DbProcedurePtr
DbSchema::findProcedure(const QString &iProcedureName) const
{
    foreach (const DbProcedurePtr &procedure, mProcedures) {
        if (iProcedureName == procedure.name()) {
            return procedure;
        }
    }
    return DbProcedurePtr();
}

/*!
 * Find trigger by its name
 *
 * \param[in] iTriggerName - Name of the trigger we are looking for
 *
 * \return Handle to the found trigger or NULL is not found.
 */
DbTriggerPtr
DbSchema::findTrigger(const QString &iTriggerName) const
{
    foreach (const DbTriggerPtr &trigger, mTriggers) {
        if (iTriggerName == trigger.name()) {
            return trigger;
        }
    }
    return DbTriggerPtr();
}

/*!
 * Read tables' names list from DB and populate
 * tables vector with *Table objects
 */
void
DbSchema::readTables()
{
    // clear tables list
    mTables.clear();

    QStringList tablesNamesList;

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
            qDebug() << "DbSchema::readTables> Sql driver was not set";
            break;

        case Database::PostgreSQL:
            DbObjects::Psql::Tools::tablesList(mName, tablesNamesList);
            break;

        case Database::MySQL:
        case Database::Oracle:
        case Database::SQLite:
            /* temporarily no support */
            return;
            break;

    }

    // for every retrieved row
    foreach (const QString &name, tablesNamesList) {

        DbTablePtr table(name, mName);

        // add table
        addTable(table);
    }
}

/*!
 * Reads views within the schema
 */
void
DbSchema::readViews()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear views list
    mViews.clear();

    QStringList viewsNamesList;

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "Database::readViews> SqlDriver was not set";
        case Database::PostgreSQL:
                        Psql::Tools::viewsList(mName, viewsNamesList);
                        break;
        case Database::MySQL:
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, viewsNamesList) {

        // declare new view object
        DbViewPtr view(name, mName);

        // add view
        addView(view);
    }
}

/*!
 * Read procs within the given schema
 */
void
DbSchema::readProcedures()
{
    // clear procs list
    mProcedures.clear();

    QStringList proceduresNamesList;

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

        // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "Database::readProcedures> SqlDriver was not set";
        case Database::PostgreSQL:
                        Psql::Tools::proceduresList(mName, proceduresNamesList);
                        break;
        case Database::MySQL:
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, proceduresNamesList) {

        // declare new proc object
        DbProcedurePtr proc(name, mName);

        // add proc
        addProcedure(proc);
    }
}

/*!
 * Reads triggers
 */
void
DbSchema::readTriggers()
{
    // clear triggers list
    mTriggers.clear();

    QStringList triggersNamesList;

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "DbSchema::readTriggers> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        Psql::Tools::triggersList(mName, triggersNamesList);
                        break;
        case Database::MySQL:
//                        Mysql::Tools::triggersList(mName, triggersNamesList);
                        break;
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, triggersNamesList) {
        // declare new trigger object
        DbTriggerPtr trigger(name, mName);

        // add trigger
        addTrigger(trigger);
    }
}

/*!
 * \todo Implement
 *
 * \return true - Even if nothing has been read
 */
bool
DbSchema::loadChildren()
{
    qDebug() << "Common::DbSchema::loadChildren> ";
    readTables();
    readViews();
    readProcedures();
    readTriggers();

    return true/*DbObject::loadData()*/;
}

/*!
 * Cleanup
 */
void
DbSchema::resetData()
{
#ifdef DEBUG_QUERY
    qDebug() << "DbSchema::resetData> cleaning for " << mName;
#endif

    // clear vectors
    mTables.clear();
    mViews.clear();
    mProcedures.clear();
    mTriggers.clear();

    DbObject::resetData();
}

/*!
 * \return DbObject::SchemaObject
 */
DbObject::Type
DbSchema::type() const
{
    return DbObject::SchemaObject;
}

/*!
 * \return Handle to owner object
 */
DbRolePtr
DbSchema::owner() const
{
    return mOwner;
}

/*!
 * \brief Set owner of schema
 *
 * \param[in] iRole - handle for role object
 */
void
DbSchema::setOwner(const DbRolePtr &iRole)
{
    mOwner = iRole;
}

/*!
 * \return Db schema description
 */
QString
DbSchema::description() const
{
    return mDescription;
}

/*!
 * Set description for the schema
 *
 * \param[in] iDescription - Description text
 */
void
DbSchema::setDescription(const QString &iDescription)
{
    mDescription = iDescription;
}

} // namespace Common

} // namespace DbObjects

