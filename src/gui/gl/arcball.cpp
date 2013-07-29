/*
 * arcball.cpp
 *
 * Created on: 10.05.2012
 * @author Ralph Schurade
 */
#include "arcball.h"

#include <QtDebug>

#include <math.h>

ArcBall::ArcBall( int width, int height ) :
    Epsilon( 0.00001 ),
    m_width( width ),
    m_height( height ),
    m_moveX( 0 ),
    m_moveY( 0 ),
    m_oldMoveX( 0 ),
    m_oldMoveY( 0 ),
    m_midClickX( 0 ),
    m_midClickY( 0 ),
    m_zoom( 1.0f )
{
    m_adjust_width  = 1.0 / ((width - 1.0) * 0.5);
    m_adjust_height = 1.0 / ((height - 1.0) * 0.5);

    m_currentRot.setToIdentity();
    m_lastRot.setToIdentity();
}

ArcBall::~ArcBall()
{
}

// maps the specified mouse position to the sphere defined
// with center and radius. the resulting vector lies on the
// surface of the sphere.
QVector3D ArcBall::map_sphere( int x, int y )
{
    float tmpx = ( x * m_adjust_width ) - 1.0;
    float tmpy = 1.0 - ( y * m_adjust_height );

    float length = ( tmpx * tmpx ) + ( tmpy * tmpy );

    QVector3D bm;
    if ( length > 1.0 )
    {
        float norm = 1.0 / sqrt( length );
        bm.setX( tmpx * norm );
        bm.setY( tmpy * norm );
        bm.setZ( 0.0 );
    }
    else
    {
        bm.setX( tmpx );
        bm.setY( tmpy );
        bm.setZ( sqrt( 1.0 - length ) );
    }
    return bm;
}


/// sets the window size.
void ArcBall::set_win_size( int width,  int height )
{
    m_width = width;
    m_height = height;
    m_adjust_width = 1.0 / ( ( width - 1.0 ) * 0.5 );
    m_adjust_height = 1.0 / ( ( height - 1.0 ) * 0.5 );
}

/// sets the current position and calculates the current
/// rotation matrix.
void ArcBall::drag( int x, int y )
{
    QVector3D v_to = map_sphere( x, y );
    QVector3D perp = QVector3D::crossProduct( v_from, v_to);

    if ( perp.length() > Epsilon )
    {
        q_current_rotation.setX( perp.x() );
        q_current_rotation.setY( perp.y() );
        q_current_rotation.setZ( perp.z() );;
        q_current_rotation.setScalar( ( v_from.x() * v_to.x() ) + ( v_from.y() * v_to.y() ) + ( v_from.z() * v_to.z() ) );
    }
    else
    {
        q_current_rotation.setX( 0.0 );
        q_current_rotation.setY( 0.0 );
        q_current_rotation.setZ( 0.0 );
        q_current_rotation.setScalar( 0.0 );
    }
    m_currentRot.setToIdentity();
    m_currentRot.rotate( q_current_rotation );
    m_currentRot = m_currentRot * m_lastRot ;
}

/// indicates the beginning of the dragging.
void ArcBall::click( int x, int y )
{
    m_lastRot = m_currentRot;
    v_mouse_down.setX( x );
    v_mouse_down.setY( y );
    v_mouse_down.setZ( 0.0 );
    v_from = map_sphere( x, y );
}

void ArcBall::midClick( int x, int y )
{
    m_midClickX = x;
    m_midClickY = y;
    m_oldMoveX = m_moveX;
    m_oldMoveY = m_moveY;
}

void ArcBall::mouseWheel( float step )
{
    if ( m_zoom < 2.0f )
    {
        step /= 2.0;
    }
    m_zoom += step;

    m_zoom = qMax( 1.0f, m_zoom );
}

void ArcBall::midDrag( int x, int y )
{
    m_moveX = ( m_oldMoveX + ( m_midClickX - x ) / m_zoom / 2 );
    m_moveY = ( m_oldMoveY + ( m_midClickY - y ) / m_zoom / 2 );
}

void ArcBall::setRotCenter( float x, float y, float z )
{
    m_rotCenter = QVector3D( -x, -y, -z );
}

void ArcBall::setView( Fn::Orient view )
{
    m_zoom = 1.0;
    m_moveX = 0;
    m_moveY = 0;
    m_oldMoveX = 0;
    m_oldMoveY = 0;
    m_currentRot.setToIdentity();
    m_lastRot.setToIdentity();

    QQuaternion rotx( sqrt(0.5), 0, 0, sqrt(0.5) );
    QQuaternion rot_x( -sqrt(0.5), 0, 0, sqrt(0.5) );
    QQuaternion roty( 0, sqrt(0.5), 0, sqrt(0.5) );
    QQuaternion rot_y( 0, -sqrt(0.5), 0, sqrt(0.5) );
    QQuaternion rotz( 0, 0, sqrt(0.5), sqrt(0.5) );
    QQuaternion rot_z( 0, 0, -sqrt(0.5), sqrt(0.5) );

    switch( view )
    {
        case Fn::Orient::NONE:
            break;
        case Fn::Orient::AXIAL:
            break;
        case Fn::Orient::CORONAL:
            m_currentRot.rotate( rotz );
            m_currentRot.rotate( rotx );
            m_currentRot.rotate( rotx );
            break;
        case Fn::Orient::SAGITTAL:
            m_currentRot.rotate( rot_x );
            m_currentRot.rotate( rot_y );
            break;
        case Fn::Orient::AXIAL2:
            m_currentRot.rotate( rot_y );
            m_currentRot.rotate( roty );
            break;
        case Fn::Orient::CORONAL2:
            m_currentRot.rotate( rotz );
            break;
        case Fn::Orient::SAGITTAL2:
            m_currentRot.rotate( rotx );
            m_currentRot.rotate( roty );
            break;
    }
}

/// returns the rotation matrix to be used directly
QMatrix4x4 ArcBall::getMVMat()
{
    QMatrix4x4 mv;
    mv.setToIdentity();

    mv = m_currentRot * mv ;

    mv.translate( m_rotCenter );

    return mv;
}

float ArcBall::getZoom()
{
    return m_zoom;
}

float ArcBall::getMoveX()
{
    return m_moveX;
}

float ArcBall::getMoveY()
{
    return m_moveY;
}

QList<QVariant> ArcBall::getState()
{
    QList<QVariant> state;
    state.push_back( m_currentRot );
    state.push_back( m_lastRot );
    state.push_back( m_moveX );
    state.push_back( m_moveY );
    state.push_back( m_oldMoveX );
    state.push_back( m_oldMoveY );
    state.push_back( m_midClickX );
    state.push_back( m_midClickY );
    state.push_back( m_zoom );
    state.push_back( m_rotCenter );
    state.push_back( q_current_rotation );
    return state;
}

void ArcBall::setState( QList<QVariant> state )
{
    m_currentRot = state[0].value<QMatrix4x4>();
    m_lastRot = state[1].value<QMatrix4x4>();
    m_moveX = state[2].toInt();
    m_moveY = state[3].toInt();
    m_oldMoveX = state[4].toInt();
    m_oldMoveY = state[5].toInt();
    m_midClickX = state[6].toInt();
    m_midClickY = state[7].toInt();
    m_zoom = state[8].toFloat();
    m_rotCenter = state[9].value<QVector3D>();
    q_current_rotation = state[10].value<QQuaternion>();
}
