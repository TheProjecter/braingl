/*
 * fiberstipplingrenderer.cpp
 *
 *  Created on: May 6, 2014
 *      Author: schurade
 */

#include "fiberstipplingrenderer.h"

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

FiberStipplingRenderer::FiberStipplingRenderer( QString name ) :
    ObjectRenderer(),
    m_name( name ),
    vboIds( new GLuint[ 4 ] ),
    m_width( 0 ),
    m_height( 0 ),
    m_zoom( 1 ),
    m_moveX( 0 ),
    m_moveY( 0 ),
    m_moveXOld( 0 ),
    m_moveYOld( 0 ),
    m_middleDownX( 0 ),
    m_middleDownY( 0 ),
    m_iso1VertsDirty( true ),
    m_iso2VertsDirty( true ),
    m_iso1ColorDirty( true ),
    m_iso2ColorDirty( true )
{
}

FiberStipplingRenderer::~FiberStipplingRenderer()
{
}

void FiberStipplingRenderer::init()
{
}

void FiberStipplingRenderer::initGL()
{
    qDebug() << "gl init " << m_name << " widget";

    glewExperimental = true;
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

    glEnable( GL_MULTISAMPLE );

    glGenBuffers( 4, vboIds );
}

void FiberStipplingRenderer::draw()
{
    glViewport( 0, 0, m_width, m_height );

//    QColor color = Models::g()->data( Models::g()->index( (int)Fn::Property::G_BACKGROUND_COLOR_NAV2, 0 ) ).value<QColor>();
//    glClearColor( color.redF(), color.greenF(), color.blueF(), 1.0 );

    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Reset projection
    QMatrix4x4 pMatrix;
    pMatrix.setToIdentity();
    pMatrix.ortho( 0, 200, 0, 200, -3000, 3000 );

    if ( m_iso1Verts.size() != 0 )
    {
        drawIso1( pMatrix );
    }

    if ( m_iso2Verts.size() != 0 )
    {
        drawIso2( pMatrix );
    }
}

void FiberStipplingRenderer::resizeGL( int width, int height )
{
    m_width = width;
    m_height = height;
}

void FiberStipplingRenderer::leftMouseDown( int x, int y )
{
}

void FiberStipplingRenderer::leftMouseDrag( int x, int y )
{
    leftMouseDown( x, y );
}

void FiberStipplingRenderer::setShaderVars()
{
}

void FiberStipplingRenderer::mouseWheel( int step )
{
    m_zoom += step;
    m_zoom = qMax( 1, m_zoom );
}

void FiberStipplingRenderer::middleMouseDown( int x, int y )
{
    m_moveXOld = m_moveX;
    m_moveYOld = m_moveY;
    m_middleDownX = x;
    m_middleDownY = y;
}

void FiberStipplingRenderer::middleMouseDrag( int x, int y )
{
    m_moveX = m_moveXOld - ( m_middleDownX - x ) * 10;
    m_moveY = m_moveYOld + m_middleDownY - y;
}

void FiberStipplingRenderer::setIso1Verts( std::vector<float> verts )
{
    m_iso1Verts = verts;
    m_iso1VertsDirty = true;
}

void FiberStipplingRenderer::setIso2Verts( std::vector<float> verts )
{
    m_iso2Verts = verts;
    m_iso2VertsDirty = true;
}

void FiberStipplingRenderer::setIso1Color( QColor color )
{
    m_iso1Color = color;
    m_iso1ColorDirty = true;
}

void FiberStipplingRenderer::setIso2Color( QColor color )
{
    m_iso2Color = color;
    m_iso2ColorDirty = true;
}

void FiberStipplingRenderer::drawIso1( QMatrix4x4 mvpMatrix )
{
    if ( m_iso1VertsDirty )
    {
        glDeleteBuffers( 1, &vboIds[0] );
        glGenBuffers( 1, &vboIds[0] );

        // Transfer vertex data to VBO 3
        glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 0 ] );
        glBufferData( GL_ARRAY_BUFFER, m_iso1Verts.size() * sizeof( float ), m_iso1Verts.data(), GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        m_iso1VertsDirty = false;
        m_iso1ColorDirty = true;
    }

    if ( m_iso1ColorDirty )
    {
        glDeleteBuffers( 1, &vboIds[1] );
        glGenBuffers( 1, &vboIds[1] );

        std::vector<float>colors( ( m_iso1Verts.size() / 3 ) * 4, 0.0 );
        for ( unsigned int i = 0; i < colors.size() / 4; ++i )
        {
            colors[i * 4    ] = m_iso1Color.redF();
            colors[i * 4 + 1] = m_iso1Color.greenF();
            colors[i * 4 + 2] = m_iso1Color.blueF();
            colors[i * 4 + 3] = 1.0;
        }

        glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 1 ] );
        glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof( float ), colors.data(), GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        m_iso1ColorDirty = false;
    }

    GLFunctions::getShader( "line" )->bind();
    GLFunctions::getShader( "line" )->setUniformValue( "mvp_matrix", mvpMatrix );

    // Tell OpenGL which VBOs to use
    glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 0 ] );
    // Draw cube geometry using indices from VBO 0
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = GLFunctions::getShader( "line" )->attributeLocation( "a_position" );
    GLFunctions::getShader( "line" )->enableAttributeArray( vertexLocation );
    glVertexAttribPointer( vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    // Tell OpenGL which VBOs to use
    glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 1 ] );
    // Draw cube geometry using indices from VBO 1
    int colorLocation = GLFunctions::getShader( "line" )->attributeLocation( "a_color" );
    GLFunctions::getShader( "line" )->enableAttributeArray( colorLocation );
    glVertexAttribPointer( colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0 );


    // Draw cube geometry using indices from VBO 0
    glDrawArrays( GL_LINES, 0 , m_iso1Verts.size() / 2 );
}

void FiberStipplingRenderer::drawIso2( QMatrix4x4 mvpMatrix )
{
    if ( m_iso2VertsDirty )
    {
        glDeleteBuffers( 1, &vboIds[2] );
        glGenBuffers( 1, &vboIds[2] );
        // Transfer vertex data to VBO 3
        glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 2 ] );
        glBufferData( GL_ARRAY_BUFFER, m_iso2Verts.size() * sizeof( float ), m_iso2Verts.data(), GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        m_iso2VertsDirty = false;
        m_iso2ColorDirty = true;
    }

    if ( m_iso2ColorDirty )
    {
        glDeleteBuffers( 1, &vboIds[3] );
        glGenBuffers( 1, &vboIds[3] );

        std::vector<float>colors( ( m_iso2Verts.size() / 3 ) * 4, 0.0 );
        for ( unsigned int i = 0; i < colors.size() / 4; ++i )
        {
            colors[i * 4    ] = m_iso2Color.redF();
            colors[i * 4 + 1] = m_iso2Color.greenF();
            colors[i * 4 + 2] = m_iso2Color.blueF();
            colors[i * 4 + 3] = 1.0;
        }

        glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 3 ] );
        glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof( float ), colors.data(), GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        m_iso2ColorDirty = false;
    }

    GLFunctions::getShader( "line" )->bind();
    GLFunctions::getShader( "line" )->setUniformValue( "mvp_matrix", mvpMatrix );

    // Tell OpenGL which VBOs to use
    glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 2 ] );
    // Draw cube geometry using indices from VBO 0
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = GLFunctions::getShader( "line" )->attributeLocation( "a_position" );
    GLFunctions::getShader( "line" )->enableAttributeArray( vertexLocation );
    glVertexAttribPointer( vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    // Tell OpenGL which VBOs to use
    glBindBuffer( GL_ARRAY_BUFFER, vboIds[ 3 ] );
    // Draw cube geometry using indices from VBO 1
    int colorLocation = GLFunctions::getShader( "line" )->attributeLocation( "a_color" );
    GLFunctions::getShader( "line" )->enableAttributeArray( colorLocation );
    glVertexAttribPointer( colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0 );


    // Draw cube geometry using indices from VBO 0
    glDrawArrays( GL_LINES, 0 , m_iso2Verts.size() / 2 );
}
