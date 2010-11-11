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
         * \param[in] ipName - Name of an object
         * \param[in] ipSchemaName - Name of schema that contains the given object or an
         *                           empty string if schema is not applicable.
         */
        DbObjectPtr(const QString &ipName, const QString &ipSchemaName = "")
            : mName(ipName),
              mSchemaName(ipSchemaName),
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
         */
        DbObjectPtr(const DbObjectPtr<T> &ipInst)
            : mName(ipInst.mName),
              mSchemaName(ipInst.mSchemaName),
              mPointee(ipInst.mPointee)
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
         *
         */
        const T* get() const
        {
            return mPointee;
        }

        /*!
         *
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
         *
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
         *
         */
        bool operator==(const DbObjectPtr<T> &ipRhs) const
        {
            return (mName == ipRhs.mName &&
                mSchemaName == ipRhs.mSchemaName &&
                mPointee == ipRhs.mPointee);
        }

        /*!
         *
         * \note Used in operator=
         */
        bool operator!=(const DbObjectPtr<T> &ipRhs) const
        {
            return (!operator==(ipRhs));
        }

        /*!
         *
         */
        DbObjectPtr<T>& operator=(const DbObjectPtr<T> &ipRhs)
        {
//            qDebug() << "DbObjectPtr::operator= (" << this << " = " << &ipRhs << ")";
            // we should compared dereferenced this, so that our operator ==  is used.
            if (*this != ipRhs) {
                // unregister old pointee
                decrementRefCount();
                // do copying
                mName = ipRhs.mName;
                mSchemaName = ipRhs.mSchemaName;
                mPointee = ipRhs.mPointee;
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

        static QMap<T*, quint32> mRefs;
    private:

        /*!
         *
         */
        inline T* initialize()
        {
            return 0;
        }

        /*!
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

