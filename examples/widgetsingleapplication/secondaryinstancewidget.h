/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#ifndef SECONDARYINSTANCEWIDGET_H
#define SECONDARYINSTANCEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class KDSingleApplication;

class SecondaryInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SecondaryInstanceWidget(KDSingleApplication *kdsa,
                                     QWidget *parent = nullptr);

private:
    void sendMessage();
    KDSingleApplication *m_kdsa = nullptr;
    QLineEdit *m_messageEdit = nullptr;
};

#endif // SECONDARYINSTANCEWIDGET_H
