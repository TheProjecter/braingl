/*
 * dataset3d.cpp
 *
 *  Created on: May 9, 2012
 *      Author: schurade
 */
#include "datasetdwi.h"

#include <QtCore/QDebug>

DatasetDWI::DatasetDWI( QString filename, QVector<ColumnVector> data, QVector<float> b0Data, QVector<float> bvals, QVector<QVector3D> bvecs, nifti_image* header ) :
    DatasetNifti( filename, Fn::DatasetType::NIFTI_DWI, header ), m_data( data ), m_b0Data( b0Data ), m_bvals( bvals ), m_bvecs( bvecs )
{
    examineDataset();
}

DatasetDWI::~DatasetDWI()
{
    m_data.clear();
    m_b0Data.clear();
    m_bvals.clear();
    m_bvecs.clear();
}

QVector<ColumnVector>* DatasetDWI::getData()
{
    return &m_data;
}

QVector<float>* DatasetDWI::getB0Data()
{
    return &m_b0Data;
}

QVector<float> DatasetDWI::getBvals()
{
    return m_bvals;
}

QVector<QVector3D> DatasetDWI::getBvecs()
{
    return m_bvecs;
}

void DatasetDWI::examineDataset()
{
    int datatype = m_properties.get( Fn::Property::DATATYPE ).toInt();
    int nx = m_properties.get( Fn::Property::NX ).toInt();
    int ny = m_properties.get( Fn::Property::NY ).toInt();
    int nz = m_properties.get( Fn::Property::NZ ).toInt();


    int dim = m_data.at( 0 ).Nrows();
    m_properties.set( Fn::Property::DIM, dim );
    int size = nx * ny * nz * dim;

    if ( datatype == DT_UNSIGNED_CHAR )
    {
        m_properties.set( Fn::Property::SIZE, static_cast<int>( size * sizeof(unsigned char) ) );
        m_properties.set( Fn::Property::MIN, 0 );
        m_properties.set( Fn::Property::MAX, 255 );    }

    if ( datatype == DT_SIGNED_SHORT )
    {
        m_properties.set( Fn::Property::SIZE, static_cast<int>( size * sizeof(signed short) ) );

        float max = -32767;
        float min = 32768;

        for ( int i = 0; i < m_b0Data.size(); ++i )
        {
            min = qMin( min, m_b0Data[i] );
            max = qMax( max, m_b0Data[i] );
        }
        m_properties.set( Fn::Property::MIN, min );
        m_properties.set( Fn::Property::MAX, max );
    }

    if ( datatype == DT_FLOAT )
    {
        m_properties.set( Fn::Property::SIZE, static_cast<int>( size * sizeof(float) ) );

        m_properties.set( Fn::Property::MIN, -1.0f );
        m_properties.set( Fn::Property::MAX, 1.0f );
    }
    m_properties.set( Fn::Property::LOWER_THRESHOLD, m_properties.get( Fn::Property::MIN ).toFloat() );
    m_properties.set( Fn::Property::UPPER_THRESHOLD, m_properties.get( Fn::Property::MAX ).toFloat() );
}

void DatasetDWI::createTexture()
{
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, &m_textureGLuint );

    glBindTexture( GL_TEXTURE_3D, m_textureGLuint );

    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );

    int nx = m_properties.get( Fn::Property::NX ).toInt();
    int ny = m_properties.get( Fn::Property::NY ).toInt();
    int nz = m_properties.get( Fn::Property::NZ ).toInt();


    float* tmpData = new float[nx * ny * nz];
    float max = m_properties.get( Fn::Property::MAX ).toFloat();
    for ( int i = 0; i < nx * ny * nz; ++i )
    {
        tmpData[i] = m_b0Data[i] / max;
    }

    glTexImage3D( GL_TEXTURE_3D, 0, GL_LUMINANCE_ALPHA, nx, ny, nz, 0, GL_LUMINANCE, GL_FLOAT, tmpData );
    delete[] tmpData;

}

void DatasetDWI::flipX()
{
}

void DatasetDWI::draw( QMatrix4x4 mvpMatrix, QMatrix4x4 mvMatrixInverse, QAbstractItemModel* globalModel, QAbstractItemModel* roiModel )
{
}

QString DatasetDWI::getValueAsString( int x, int y, int z )
{
    return QString( "" );
}
