/*
 * NavRenderer.cpp
 *
 *  Created on: 09.05.2012
 *      Author: Ralph
 */
#include "navrenderer.h"

#include <QtCore/QDebug>

#include <QtOpenGL/QGLShaderProgram>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

#include "../gl/glfunctions.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

NavRenderer::NavRenderer( QString name ) :
    ObjectRenderer(),
    m_name( name ),
    m_ratio( 1.0 ),
    vboIds( new GLuint[ 4 ] ),
    m_x( 0. ),
    m_y( 0. ),
    m_z( 0. ),
    m_xb( 0 ),
    m_yb( 0 ),
    m_zb( 0 ),
    m_xd( 0 ),
    m_yd( 0 ),
    m_zd( 0 ),
    m_xOld( -1 ),
    m_yOld( -1 ),
    m_zOld( -1 ),
    m_xoff( 0 ),
    m_yoff( 0 )
{
}

NavRenderer::~NavRenderer()
{
    destroy( m_program );
    destroy( m_crosshairProgram );
}

void NavRenderer::init()
{
}

void NavRenderer::initGL()
{
    glClearColor( 1.0, 1.0, 1.0, 1.0 );

    glEnable( GL_DEPTH_TEST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );

    glShadeModel( GL_SMOOTH );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_MULTISAMPLE );
    static GLfloat lightPosition[ 4 ] = { 0.5, 5.0, -3000.0, 1.0 };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );

    initShader();

    glGenBuffers( 4, vboIds );

    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    GLushort crosshairIndices[] = { 0, 1, 2, 3 };

    // Transfer index data to VBO 0
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboIds[ 0 ] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_STATIC_DRAW );

    // Transfer index data to VBO 3
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboIds[ 3 ] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), crosshairIndices, GL_STATIC_DRAW );
}

void NavRenderer::resizeGL( int width, int height )
{
    m_width = width;
    m_height = height;

    adjustRatios();
}

void NavRenderer::leftMouseDrag( int x, int y )
{
    leftMouseDown( x, y );
}

void NavRenderer::initShader()
{
    m_program = GLFunctions::initShader( "slice" );
    m_crosshairProgram = GLFunctions::initShader( "crosshair" );
}


void NavRenderer::setupTextures()
{
    GLFunctions::setupTextures( model() );
}

void NavRenderer::setShaderVars()
{
    GLFunctions::setSliceShaderVars( m_program, model() );
}