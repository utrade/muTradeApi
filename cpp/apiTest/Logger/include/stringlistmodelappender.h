/*
  Copyright (c) 2010 Boris Moiseev (cyberbobs at gmail dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
*/
#ifndef STRINGLISTMODELAPPENDER_H
#define STRINGLISTMODELAPPENDER_H

// Logger
#include "CuteLogger_global.h"
#include <AbstractStringAppender.h>

// Qt
#include <QStringListModel>
//#include <QTextStream>
//! This appender configures a QStringListModel and writes the log records to that model.
class CUTELOGGERSHARED_EXPORT StringListModelAppender : public AbstractStringAppender
{
public:
    //! Constructs the new stringListModel appender assigned to the model reference prvided.
    StringListModelAppender(QStringListModel *model);

    ~StringListModelAppender();

    /**
     * \sa setStringListModel()
     */
    void setStringListModel(QStringListModel *model);
    //    QString fileName() const;

    //! Sets the StringListModel .

protected:
    //! Write the log record to the file.
    /**
     * \sa fileName()
     * \sa AbstractStringAppender::format()
     */
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, const QString& message);

private:
    QStringListModel *m_logModel;
    QAbstractItemModel *itemModel;
    //    QTextStream m_logStream;
    mutable QMutex m_logModelMutex;
    QStringList *stringlist;

};

#endif // STRINGLISTMODELAPPENDER_H
