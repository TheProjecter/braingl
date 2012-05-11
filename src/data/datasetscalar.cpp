/*
 * datasetscalar.cpp
 *
 *  Created on: May 4, 2012
 *      Author: schurade
 */

#include <QtCore/QDebug>

#include "datasetscalar.h"

DatasetScalar::DatasetScalar( QString filename, void* data ) :
    DatasetNifti( filename, FNDT_NIFTI_SCALAR, data )
{
}

DatasetScalar::~DatasetScalar()
{
}

void DatasetScalar::createTexture()
{
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, &m_textureGLuint );

    glBindTexture( GL_TEXTURE_3D, m_textureGLuint );

    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );

    if ( getDatatype() == DT_UNSIGNED_CHAR )
    {
        unsigned char* charData = reinterpret_cast<unsigned char*>(m_data);

        unsigned char min = 255;
        unsigned char max = 0;

        int size = nx * ny * nz;
        for ( size_t i = 0; i < size; ++i )
        {
            min = qMin( min, charData[i] );
            max = qMax( max, charData[i] );
        }
        glTexImage3D( GL_TEXTURE_3D, 0, GL_LUMINANCE_ALPHA, nx, ny, nz, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_data );

        m_min = static_cast<float>( min );
        m_max = static_cast<float>( max );
    }
}
