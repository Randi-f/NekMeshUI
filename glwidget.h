#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>

#include <QWheelEvent>
#include "NekMeshObject.h"
#include "GeoParser.h"



class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    void setMesh(MeshSharedPtr m_mesh);
    void setCADData(std::shared_ptr<CADData> cadData);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;


private:
    QMatrix4x4 projection;

    float rotationX;
    float rotationY;
    float translateX;
    float translateY;
    float scaleFactor;
    QPoint lastMousePosition;

    MeshSharedPtr mesh;

    void drawCube();
    void updateRotation();
    void drawMesh(MeshSharedPtr mesh);

    std::shared_ptr<CADData> cadData;
    void drawCAD(MeshSharedPtr mesh);

    void useNacaMesh();



};

#endif // GLWIDGET_H
