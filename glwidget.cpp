#include "glwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), angleX(0.0f), angleY(0.0f)
{
}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);    // 启用深度测试
}

void GLWidget::resizeGL(int w, int h)
{
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.01f, 100.0f);  // 设置透视投影矩阵
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲区和深度缓冲区

    QMatrix4x4 modelView;
    modelView.translate(0.0f, 0.0f, -5.0f); // 平移相机位置
    modelView.rotate(angleX, 1.0f, 0.0f, 0.0f); // 绕X轴旋转
    modelView.rotate(angleY, 0.0f, 1.0f, 0.0f);  // 绕Y轴旋转立方体

    QMatrix4x4 mvp = projection * modelView;    // 计算模型视图投影矩阵

    glLoadMatrixf(mvp.constData()); // 加载模型视图投影矩阵到OpenGL

    drawCube(); // 绘制立方体
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos(); // 记录鼠标按下位置
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastMousePosition.x(); // 计算鼠标移动距离
    int dy = event->y() - lastMousePosition.y();

    angleX += dy; // 更新X轴旋转角度
    angleY += dx; // 更新Y轴旋转角度

    update(); // 请求更新窗口，触发重绘

    lastMousePosition = event->pos(); // 更新最后的鼠标位置
}

void GLWidget::drawCube()
{
    static const GLfloat vertices[] = {
        1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
    };

    static const GLubyte indices[] = {
        0, 1, 2, 3,  // front
        4, 5, 6, 7,  // back
        0, 3, 7, 4,  // right
        1, 2, 6, 5,  // left
        0, 1, 5, 4,  // top
        2, 3, 7, 6   // bottom
    };

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glVertexPointer(3, GL_FLOAT, 0, vertices); // 设置顶点数组指针
    //glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices); // 绘制立方体的各个面

    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, indices);
    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
}
