/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "secondaryinstancewidget.h"

#include <kdsingleapplication.h>

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>

#include <QtWidgets/QMessageBox>

SecondaryInstanceWidget::SecondaryInstanceWidget(KDSingleApplication *kdsa, QWidget *parent)
    : QWidget(parent)
    , m_kdsa(kdsa)
{
    setWindowTitle(tr("Secondary instance"));
    QLabel *label = new QLabel(tr("<b>Secondary instance.</b> Send message to primary:"));
    m_messageEdit = new QLineEdit;
    m_messageEdit->setPlaceholderText(tr("Type something here..."));

    QPushButton *sendButton = new QPushButton(tr("&Send"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 0, 1, 2);
    layout->addWidget(m_messageEdit, 1, 0);
    layout->addWidget(sendButton, 1, 1);

    setLayout(layout);

    connect(m_messageEdit, &QLineEdit::returnPressed, this, &SecondaryInstanceWidget::sendMessage);
    connect(m_messageEdit, &QLineEdit::textChanged,
            this, [sendButton](const QString &text) { sendButton->setEnabled(!text.isEmpty()); });
    connect(sendButton, &QPushButton::clicked, this, &SecondaryInstanceWidget::sendMessage);
}

void SecondaryInstanceWidget::sendMessage()
{
    const QString message = m_messageEdit->text();
    if (!message.isEmpty()) {
        if (m_kdsa->sendMessageWithTimeout(message.toUtf8(), 1000)) {
            m_messageEdit->clear();
        } else {
            QMessageBox::warning(this,
                                 tr("Error sending message"),
                                 tr("The message '%1' could not be sent to the primary.").arg(message));
        }
    }
}
