/*
 * glfunctions.h
 *
 *  Created on: 15.05.2012
 *      Author: Ralph
 */

#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_

#include <QtCore/QList>

class QAbstractItemModel;
class QGLShaderProgram;

struct VertexData
{
    QVector3D position;
    QVector3D texCoord;
};


class GLFunctions
{
public:
    static void setupTextures( QAbstractItemModel* model );
    static void setTexInterpolation( QAbstractItemModel* model, int row );

    static QGLShaderProgram* initShader( QString name );
    static void loadShaders();
    static void reloadShaders();

    static void setTensorShaderVars( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setTensorShaderVarsEV( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsEV( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setSliceShaderVars( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setQBallShaderVars( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVars( QString name, QAbstractItemModel* model );

    static QList<int> getTextureIndexes( QAbstractItemModel* model );

    static QGLShaderProgram* getShader( QString name );

    static QVector<QString> getShaderNames();
    static QString getShaderCode( QString name );
    static void setShaderCode( QString name,  QString source );

private:
    GLFunctions();
    virtual ~GLFunctions();

    static void copyShaderToString( QString name );

    static QHash< QString, QString >m_shaderSources;
    static QHash< QString, QGLShaderProgram* >m_shaders;
    static QVector<QString>m_shaderNames;

    static bool shadersLoaded;
};

#endif /* GLFUNCTIONS_H_ */
