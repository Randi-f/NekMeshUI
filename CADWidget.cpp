#include "CADWidget.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>

#include "NekMeshObject.h"
#include "GeoParser.h"


#include <LibUtilities/BasicConst/GitRevision.h>
#include <LibUtilities/BasicUtils/Timer.h>
#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <string>

#include <NekMesh/Module/Module.h>

#include <map>


CADWidget::CADWidget(QWidget *parent)
    : QOpenGLWidget(parent), rotationX(0.0f), rotationY(0.0f), scaleFactor(1.0f),
    translateX(0.0f), translateY(0.0f)
{
}

CADWidget::~CADWidget() {}

void CADWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);    // 启用深度测试
}

void CADWidget::resizeGL(int w, int h)
{
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.01f, 100.0f);  // 设置透视投影矩阵
}

void CADWidget::setMesh(MeshSharedPtr m_mesh){
    mesh = m_mesh;
}


void CADWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲区和深度缓冲区

    QMatrix4x4 modelView;
    modelView.translate(translateX, translateY, -1.0f); // 平移相机位置
    modelView.scale(scaleFactor);
    modelView.rotate(rotationX, 1.0f, 0.0f, 0.0f);  // 绕X轴旋转
    modelView.rotate(rotationY, 0.0f, 1.0f, 0.0f);  // 绕Y轴旋转立方体

    QMatrix4x4 mvp = projection * modelView;    // 计算模型视图投影矩阵

    glLoadMatrixf(mvp.constData()); // 加载模型视图投影矩阵到OpenGL

    // drawCAD();
    // drawVert();
    drawCurve();
}



void CADWidget::setCADData(std::shared_ptr<CADData> cadData) {
    this->cadData = cadData;
    update();
}

void CADWidget::drawCAD(){
    if (!cadData) return;

    glBegin(GL_LINES);
    for (const auto &lineEntry : cadData->lines) {
        auto line = lineEntry.second;
        auto start = cadData->points[line->start];
        auto end = cadData->points[line->end];

        glVertex3f(start->x, start->y, start->z);
        glVertex3f(end->x, end->y, end->z);
    }

    for (const auto &loop : cadData->lineLoops) {
        for (size_t i = 0; i < loop.second->lines.size(); ++i) {
            int lineId = loop.second->lines[i];
            auto line = cadData->lines[lineId];
            auto start = cadData->points[line->start];
            auto end = cadData->points[line->end];

            glVertex3f(start->x, start->y, start->z);
            glVertex3f(end->x, end->y, end->z);
        }
    }

    for (const auto &loop : cadData->splines) {
        for (size_t i = 0; i < loop.second->points.size(); ++i) {
            int pointId = loop.second->points[i];
            auto point = cadData->points[pointId];
            glVertex3f(point->x, point->y, point->z);
        }
    }

    glEnd();

}

void CADWidget::drawVert(){

    std::map<int, CADVertSharedPtr> verts = mesh->m_cad->GetVerts();
    glBegin(GL_LINES);
    // 检查 verts 是否为空
    // 使用迭代器遍历 verts
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        // int id = it->first;
        CADVertSharedPtr vert = it->second;

        // std::cout << "Vert ID: " << id << std::endl;
        std::array<Nektar::NekDouble, 3> loc = vert->GetLoc();
        glVertex3f(static_cast<double>(loc[0]), static_cast<double>(loc[1]), static_cast<double>(loc[2]));
    }
    glEnd();

    // if (!verts.empty()) {
    //     // 获取第一个元素
    //     auto it = verts.begin();
    //     int id = it->first;
    //     CADVertSharedPtr vert = it->second;

    //     std::cout << "First Vert ID: " << id << std::endl;

    // } else {
    //     std::cout << "The verts map is empty." << std::endl;
    // }


}

void CADWidget::drawCurve(){
    glColor3f(1.0, 0.0, 0.0); // 红色
    glBegin(GL_LINES);
    for (int i = 1; i <= mesh->m_cad->GetNumCurve(); i++){
        CADCurveSharedPtr curve = mesh->m_cad->GetCurve(i);
        std::vector<CADVertSharedPtr> verts = curve->GetVertex();
        for (const auto& vert : verts) {
            std::array<Nektar::NekDouble, 3> loc = vert->GetLoc();
            glVertex3f(static_cast<double>(loc[0]), static_cast<double>(loc[1]), static_cast<double>(loc[2]));
        }
    }
    glEnd();
}

void CADWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos(); // 记录鼠标按下位置
}

void CADWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();

    if (event->modifiers() & Qt::ShiftModifier) {
        // 平移
        translateX += deltaX / (float(width()) / 2.0f) * scaleFactor;
        translateY -= deltaY / (float(height()) / 2.0f) * scaleFactor;
    } else {
        // 旋转
        rotationX += deltaY * 0.5f;
        rotationY += deltaX * 0.5f;
    }

    lastMousePosition = event->pos();

    update();
}

void CADWidget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
    {
        scaleFactor *= 1.1f; // 放大
    }
    else
    {
        scaleFactor /= 1.1f; // 缩小
    }
    update();
}

