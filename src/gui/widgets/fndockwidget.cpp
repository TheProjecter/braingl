/*
 * fndockwidget.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: schurade
 */

#include "fndockwidget.h"

FNDockWidget::FNDockWidget( QString name, QWidget* widget, QWidget* parent ) :
    QDockWidget( name, parent )
{
    setObjectName( name );

    m_titleWidget = new QWidget();

    setWidget( widget );
    setAllowedAreas( Qt::AllDockWidgetAreas );
    setFeatures( QDockWidget::AllDockWidgetFeatures );
}

FNDockWidget::~FNDockWidget()
{
}

void FNDockWidget::toggleTitleWidget()
{
    QWidget* tmp = titleBarWidget();
    setTitleBarWidget( m_titleWidget );
    m_titleWidget = tmp;
}