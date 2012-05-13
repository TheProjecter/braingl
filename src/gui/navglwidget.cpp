/*
 * navglwidget.cpp
 *
 *  Created on: 12.05.2012
 *      Author: Ralph
 */
#include <QtGui/QtGui>
#include <QtOpenGL/QGLWidget>

#include "navglwidget.h"

NavGLWidget::NavGLWidget( QString name, QWidget* parent ) :
    QDockWidget( name, parent )
{
    setObjectName( QString("nav gl") + name );

    m_glWidget = new QGLWidget( this );
    m_glWidget->setToolTip( QString( "nav gl" ) );

    this->setAllowedAreas( Qt::AllDockWidgetAreas );
    this->setFeatures( QDockWidget::AllDockWidgetFeatures );

    QWidget* panel = new QWidget( this );

    m_layout = new QVBoxLayout();

    QHBoxLayout* sliderLayout = new QHBoxLayout();
    m_slider = new QSlider();
    m_slider->setOrientation( Qt::Horizontal );
    m_slider->setMinimum( 0 );
    m_slider->setMaximum( 200 );

    connect( m_slider, SIGNAL( valueChanged( int) ), this, SLOT( sliderChanged( int ) ) );

    sliderLayout->addWidget( m_slider );

    m_layout->addWidget( m_glWidget );
    m_layout->addLayout( sliderLayout );

    panel->setLayout( m_layout );
    setWidget( panel );

    m_glWidget->setMinimumSize( 50,50 );

}

NavGLWidget::~NavGLWidget()
{
}

QSize NavGLWidget::minimumSizeHint() const
{
    return QSize( 50, 50 );
}

QSize NavGLWidget::sizeHint() const
{
    return QSize( 400, 400 );
}


void NavGLWidget::sliderChanged( int value )
{
    //qDebug() << value;
}