/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#ifndef PRIMARYINSTANCEWIDGET_H
#define PRIMARYINSTANCEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QListWidget;
QT_END_NAMESPACE

class PrimaryInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrimaryInstanceWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    void addMessage(const QByteArray &message);

private:
    QListWidget *m_messagesListWidget = nullptr;
};

#endif // PRIMARYINSTANCEWIDGET_H
