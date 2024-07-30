#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QApplication>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}

protected:
    void initializeGL() override
    {
        initializeOpenGLFunctions();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置清屏颜色
    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, w, h);
    }

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制第一条红色的线
        glColor3f(1.0f, 0.0f, 0.0f); // 设置颜色为红色
        glBegin(GL_LINES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glEnd();

        // 绘制第二条绿色的线
        glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
        glBegin(GL_LINES);
        glVertex2f(-0.5f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
    }
};

#endif // MYGLWIDGET_H
