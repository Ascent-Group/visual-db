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

#ifndef DBOBJECTS_COMMON_DBOBJECTPTR_H
#define DBOBJECTS_COMMON_DBOBJECTPTR_H

//#include <common/DbIndex.h>
//#include <common/DbLanguage.h>
//#include <common/DbProcedure.h>
//#include <common/DbRole.h>
//#include <common/DbSchema.h>
//#include <common/DbTable.h>
//#include <common/DbTrigger.h>
//#include <common/DbView.h>

#include <QMap>
#include <QString>

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * \class DbObjectPtr
 * \brief Defines a template proxy for database objects
 * \headerfile common/DbObjectPtr.h
 */
template<typename T>
class DbObjectPtr
{
    typedef QMap<T*, quint32> References;

    public:
        /*!
         * Constructor
         *
         * \param[in] iName - Name of an object
         * \param[in] iSchemaName - Name of schema that contains the given object or an
         *                           empty string if schema is not applicable.
         */
        DbObjectPtr(const QString &iName, const QString &iSchemaName = "")
            : mName(iName),
              mSchemaName(iSchemaName),
              mPointee(0)
        {
            initialize();
            // register reference
            incrementRefCount();
        }

        /*!
         * Default constructor
         */
        DbObjectPtr()
            : mName(),
              mSchemaName(),
              mPointee(0)
        {}

        /*!
         * Copying constructor
         * \param[in] iInst - Another instance of proxy
         */
        DbObjectPtr(const DbObjectPtr<T> &iInst)
            : mName(iInst.mName),
              mSchemaName(iInst.mSchemaName),
              mPointee(iInst.mPointee)
        {
            incrementRefCount();
        }

        /*!
         * Destructor
         */
        ~DbObjectPtr()
        {
            decrementRefCount();
        }

        /*!
         * \return Name of a real object
         */
        QString name() const
        {
            return mName;
        }

        /*!
         * \return Schema's name of a real object
         */
        QString schemaName() const
        {
            return mSchemaName;
        }

        /*!
         * Get the real pointer to the real object.
         *
         * \return Pointer to the object
         */
        const T* get() const
        {
            // if name is empty then we have an empty proxy and mPointee is not a valid
            // object.
            // \note Empty proxies will be used when creating new database objects.
//            if (mName.isEmpty()) {
//                return 0;
//            }

            return mPointee;
        }

        /*!
         * \return true If the proxy points to a valid and NOT anonimous object.
         * \return false Otherwise.
         */
        bool valid() const
        {
            if (mPointee && !mName.isEmpty()) {
                return true;
            }

            return false;
        }

        /*!
         * \brief Overloaded -> operator.
         *
         * \note If you call ->setName() you have to manually set proxy's name too.
         *
         * \return Pointer to the object
         */
        T* operator->()
        {
            if (!mPointee->isLoaded()) {
                mPointee->loadData();
            }

            return mPointee;
//            return initialize<T>();
        }

        /*!
         * \brief Const overloaded -> operator
         *
         * \return Pointer to the object
         */
        T* operator->() const
        {
            if (!mPointee->isLoaded()) {
                mPointee->loadData();
            }

            return mPointee;
        }

        /*!
         *
         */
//        T& operator*()
//        {
//            return *mPointee;
//            return *initialize<T>();
//        }

        /*!
         * \param[in] iRhs Instance of proxy we are comparing our proxy to.
         *
         * \return true If proxies are identical
         * \return false Otherwise
         */
        bool operator==(const DbObjectPtr<T> &iRhs) const
        {
            return (mName == iRhs.mName &&
                mSchemaName == iRhs.mSchemaName &&
                mPointee == iRhs.mPointee);
        }

        /*!
         * \note Used in operator=
         *
         * \param[in] iRhs - Another instance of proxy we are comparing to.
         *
         */
        bool operator!=(const DbObjectPtr<T> &iRhs) const
        {
            return (!operator==(iRhs));
        }

        /*!
         * \brief Assignment operator
         *
         * \param[in] iRhs - Instance of a proxy we are assigning to the given proxy
         *
         * \return Reference to the given proxy
         * \todo Implement assignment operator using copy constructor
         */
        DbObjectPtr<T>& operator=(const DbObjectPtr<T> &iRhs)
        {
//            qDebug() << "DbObjectPtr::operator= (" << this << " = " << &iRhs << ")";
            // we should compared dereferenced this, so that our operator ==  is used.
            if (*this != iRhs) {
                // unregister old pointee
                decrementRefCount();
                // do copying
                mName = iRhs.mName;
                mSchemaName = iRhs.mSchemaName;
                mPointee = iRhs.mPointee;
                // register new pointee
                incrementRefCount();
            }

//            qDebug() << "DbObjectPtr::operator= >>> " << mName << " mRefs[" << mPointee << "] = " << mRefs[mPointee];
            return *this;
        }

        /*!
         * \note Debug helper
         */
        void printRefs()
        {
            qDebug() << "##############################################\n";
            typename References::iterator iter;
            for (iter = mRefs.begin(); iter != mRefs.end(); ++iter) {
                qDebug() << iter.key() << " | " << iter.value() << "\n";
            }
        }

    protected:
        /*! Name of a real object */
        QString mName;
        /*! Name of a schema that contains a real object */
        QString mSchemaName;
        /*! Handle to a real object */
        /*mutable*/ T *mPointee;
        /*! Pointee-to-refcount registry */
        static QMap<T*, quint32> mRefs;

    private:

        /*!
         * Real object initialization function
         * \note By default does nothing, because it doesn't know which factory to call.
         *       The default implementation implies that this proxy is applicable only to
         *       db objects.
         */
        inline T* initialize()
        {
            return 0;
        }

        /*!
         * \brief Decrements the reference count for a real object
         * \return The reference count after decrementing
         */
        int decrementRefCount()
        {
//            qDebug() << "decrementRefCount> " << this << " mPointee = " << mPointee;
            typename References::iterator iter = mRefs.find(mPointee);

            if (mRefs.end() == iter) {
                if (!mName.isEmpty()) {
//                    qDebug() << "decrementRefCount> " << this << " This should never had happened for [" << mName << "] which points to " << mPointee;
                }
                return -1;
            }

            // decrement ref count
            iter.value()--;
//            qDebug() << "decrementRefCount> " << this << " mRefs[" << mPointee << "] = " << mRefs.value(mPointee);

            // if refcount == 0
            if (0 == iter.value()) {
                // unregister
                mRefs.erase(iter);
                // delete pointee
//                qDebug() << "decrementRefCount> " << this << " will delete " << mPointee;
                delete mPointee;

                mPointee = 0;
                return 0;
            }

            return mRefs.value(mPointee);
        }

        /*!
         * \brief Increments the reference count for a real object
         * \return The reference count after incrementing
         */
        int incrementRefCount()
        {
//            qDebug() << "incrementRefCount> " << this;
            typename QMap<T*, quint32>::iterator iter = mRefs.find(mPointee);

            if (mRefs.end() == iter) {
                mRefs.insert(mPointee, 1);
            } else {
                iter.value()++;
            }

//            qDebug() << "incrementRefCount> " << this << " mRefs[" << mPointee << "]= " << mRefs.value(mPointee);
            return mRefs.value(mPointee);
        }
};

} // namespace Common
} // namespace DbObjects

#include <common/DbObjectPtr.cpp>

#endif // DBOBJECTS_COMMON_DBOBJECTPTR_H

