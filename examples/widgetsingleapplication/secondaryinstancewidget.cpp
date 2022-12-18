/*
    MIT License

    Copyright (C) 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
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
