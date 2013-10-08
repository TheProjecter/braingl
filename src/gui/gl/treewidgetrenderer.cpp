/*
 * TreeWidgetRenderer.cpp
 *
 * Created on: 17.09.2013
 * @author Ralph Schurade
 */
#include "treewidgetrenderer.h"

#include "../gl/glfunctions.h"

#include "../../algos/tree.h"

#include "../../data/enums.h"
#include "../../data/models.h"
#include "../../data/vptr.h"

#include "../../data/datasets/datasettree.h"

#include <QDebug>
#include <QtOpenGL/QGLShaderProgram>
#include <QVector3D>
#include <QMatrix4x4>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

TreeWidgetRenderer::TreeWidgetRenderer( QString name ) :
    ObjectRenderer(),
    m_name( name )
{
}

TreeWidgetRenderer::~TreeWidgetRenderer()
{
}

void TreeWidgetRenderer::init()
{
}

void TreeWidgetRenderer::initGL()
{
    qDebug() << "gl init " << m_name << " widget";

    GLenum errorCode = glewInit();
    if ( GLEW_OK != errorCode )
    {
        qDebug() << "Problem: glewInit failed, something is seriously wrong.";
        qDebug() << glewGetErrorString( errorCode );
        exit( false );
    }
    else
    {
        //qDebug() << "OpenGL initialized.";
    }

    glClearColor( 1.0, 1.0, 1.0, 1.0 );

    glEnable( GL_DEPTH_TEST );

    glShadeModel( GL_SMOOTH );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_MULTISAMPLE );
    static GLfloat lightPosition[ 4 ] = { 0.5, 5.0, -3000.0, 1.0 };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );
}

void TreeWidgetRenderer::draw()
{
    glViewport( 0, 0, m_width, m_height );

    QColor color = Models::g()->data( Models::g()->index( (int)Fn::Property::G_BACKGROUND_COLOR_NAV2, 0 ) ).value<QColor>();
    glClearColor( color.redF(), color.greenF(), color.blueF(), 1.0 );

    //qDebug() << "nav draw";
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );



   // Reset projection
   QMatrix4x4 pMatrix;
   pMatrix.setToIdentity();

    QList<int>rl;
    int countDatasets = Models::d()->rowCount();
    for ( int i = 0; i < countDatasets; ++i )
    {
        QModelIndex index = Models::d()->index( i, (int)Fn::Property::D_ACTIVE );
        if ( Models::d()->data( index, Qt::DisplayRole ).toBool() )
        {
            index = Models::d()->index( i, (int)Fn::Property::D_TYPE );
            if ( Models::d()->data( index, Qt::DisplayRole ).toInt() == (int)Fn::DatasetType::TREE )
            {
                rl.push_back( i );
            }
        }
    }

    DatasetTree* ds = 0;
    if ( rl.size() > 0 )
    {
        ds = VPtr<DatasetTree>::asPtr( Models::d()->data( Models::d()->index( rl[0], (int)Fn::Property::D_DATASET_POINTER ), Qt::DisplayRole ) );
        int leaves = ds->getTree()->getNumLeaves();
        pMatrix.ortho(  0,  leaves, 0, 1., -3000, 3000 );
        ds->drawTree( pMatrix, m_width, m_height );
    }
    else
    {
        return;
    }

}

void TreeWidgetRenderer::resizeGL( int width, int height )
{
    m_width = width;
    m_height = height;
}

void TreeWidgetRenderer::leftMouseDown( int x, int y )
{
}

void TreeWidgetRenderer::leftMouseDrag( int x, int y )
{
    leftMouseDown( x, y );
}

void TreeWidgetRenderer::setShaderVars()
{
}

void TreeWidgetRenderer::mouseWheel( int step )
{
//    m_zoom += step;
//    m_zoom = qMax( 1, m_zoom );
}

void TreeWidgetRenderer::middleMouseDown( int x, int y )
{
//    m_moveXOld = m_moveX;
//    m_moveYOld = m_moveY;
//    m_middleDownX = x;
//    m_middleDownY = y;
}

void TreeWidgetRenderer::middleMouseDrag( int x, int y )
{
//    m_moveX = m_moveXOld - ( m_middleDownX - x );
//    m_moveY = m_moveYOld + m_middleDownY - y;
}