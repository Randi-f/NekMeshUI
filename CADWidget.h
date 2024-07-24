#ifndef CADWIDGET_H
#define CADWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>

#include <QWheelEvent>
#include "NekMeshObject.h"
#include "GeoParser.h"



class CADWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    CADWidget(QWidget *parent = nullptr);
    ~CADWidget();

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

    void updateRotation();

    std::shared_ptr<CADData> cadData;
    void drawCAD();
    void drawVert();
    void drawCurve();

};

#endif
