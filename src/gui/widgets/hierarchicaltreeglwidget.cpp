/*
 * HierarchicalTreeGLWidget.cpp
 *
 * Created on: May 03, 2012
 * @author Ralph Schurade
 */

#include "hierarchicaltreeglwidget.h"

#include "../gl/treewidgetrenderer.h"

#include "../../data/models.h"
#include "../../data/datasets/dataset.h"

#include <QtGui>
#include <QDebug>

HierarchicalTreeGLWidget::HierarchicalTreeGLWidget( QString name, QWidget *parent, const QGLWidget *shareWidget ) :
	QGLWidget( parent, shareWidget )
{
    m_renderer = new TreeWidgetRenderer( "tree renderer" );


    connect( Models::g(), SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), this, SLOT( update() ) );
    connect( Models::d(), SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), this, SLOT( update() ) );
}

HierarchicalTreeGLWidget::~HierarchicalTreeGLWidget()
{
    destroy ( m_renderer );
}

QSize HierarchicalTreeGLWidget::minimumSizeHint() const
{
    return QSize( 50, 50 );
}

QSize HierarchicalTreeGLWidget::sizeHint() const
{
    return QSize( 2000, 2000 );
}

void HierarchicalTreeGLWidget::initializeGL()
{
	m_renderer->initGL();
}

void HierarchicalTreeGLWidget::paintGL()
{
    if ( m_visible )
    {
        m_renderer->draw();
    }
}

void HierarchicalTreeGLWidget::resizeGL( int width, int height )
{
    m_renderer->resizeGL( width, height );
}

void HierarchicalTreeGLWidget::mousePressEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
        QList<Dataset*>dl = Models::getDatasets( Fn::DatasetType::TREE );
        for ( int i = 0; i < dl.size(); ++i )
        {
            dl[i]->mousePick( 0, QVector3D( event->x(), event->y(), 0 ), event->modifiers(), "tree" );
        }
    }
    if ( event->buttons() & Qt::MiddleButton )
    {
        m_renderer->middleMouseDown( event->x(), event->y() );
    }
    updateGL();
}

void HierarchicalTreeGLWidget::mouseMoveEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
        m_renderer->leftMouseDrag( event->x(), event->y() );
    }
    if ( event->buttons() & Qt::MiddleButton )
    {
        m_renderer->middleMouseDrag( event->x(), event->y() );
    }
    updateGL();
}

void HierarchicalTreeGLWidget::wheelEvent(QWheelEvent *event)
{
     int numDegrees = event->delta() / 8;
     int numSteps = numDegrees / 15;
     //m_renderer->mouseWheel( numSteps );
     updateGL();

}


void HierarchicalTreeGLWidget::mouseReleaseEvent( QMouseEvent *event )
{
    updateGL();
}

void HierarchicalTreeGLWidget::update()
{
    if ( !m_visible )
    {
        return;
    }
    updateGL();
}

void HierarchicalTreeGLWidget::setWidgetVisible( bool visible )
{
    m_visible = visible;
}