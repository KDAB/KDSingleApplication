/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtWidgets/QApplication>

#include <kdsingleapplication.h>

#include "primaryinstancewidget.h"
#include "secondaryinstancewidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    KDSingleApplication kdsa;

    QWidget *widget;

    if (kdsa.isPrimaryInstance()) {
        PrimaryInstanceWidget *piw = new PrimaryInstanceWidget;
        QObject::connect(&kdsa, &KDSingleApplication::messageReceived,
                         piw, &PrimaryInstanceWidget::addMessage);

        widget = piw;
    } else {
        SecondaryInstanceWidget *siw = new SecondaryInstanceWidget(&kdsa);
        widget = siw;
    }

    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();

    return app.exec();
}
