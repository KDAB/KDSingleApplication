/*
    MIT License

    Copyright (C) 2019-2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

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

void PrimaryInstanceWidget::addMessage(const QString &message)
{
    m_messagesListWidget->addItem(message);
    m_messagesListWidget->scrollToBottom();
}
