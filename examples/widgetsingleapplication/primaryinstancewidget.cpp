/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "primaryinstancewidget.h"

#include <QLabel>
#include <QListWidget>

#include <QVBoxLayout>

PrimaryInstanceWidget::PrimaryInstanceWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Primary instance"));
    QLabel *label = new QLabel(tr("<b>Primary instance.</b> Messages received from secondaries:"));
    m_messagesListWidget = new QListWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(m_messagesListWidget);
}

void PrimaryInstanceWidget::addMessage(const QByteArray &message)
{
    m_messagesListWidget->addItem(QString::fromUtf8(message));
    m_messagesListWidget->scrollToBottom();
}
