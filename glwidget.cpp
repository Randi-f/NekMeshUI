#include "glwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>
#include <LibUtilities/BasicConst/GitRevision.h>
#include <LibUtilities/BasicUtils/Timer.h>
#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <NekMesh/Module/Module.h>
#include <map>
#include <cmath>  // for abs

#include "NekMeshObject.h"
#include "GeoParser.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), rotationX(0.0f), rotationY(0.0f), scaleFactor(1.0f),
    translateX(0.0f), translateY(0.0f)
{
}

void GLWidget::useNacaMesh(){
    auto logOutput = std::make_shared<StreamOutput>(std::cout);
    Logger m_log(logOutput, INFO);

    mesh = std::shared_ptr<Mesh>(new Mesh());
    ModuleSharedPtr module;

    ////**** CAD ****////
    module = GetModuleFactory().CreateInstance(
        ModuleKey(eProcessModule, "loadcad"), mesh);
    module->SetLogger(m_log);
    module->RegisterConfig("filename", "6412"); //m_cadfile
    module->RegisterConfig("voidpoints", "");   //m_voidPts
    bool m_2D = true;
    if (m_2D)
    {
        module->RegisterConfig("2D", "");
    }
    bool m_naca=true;
    if (m_naca)
    {
        module->RegisterConfig("NACA", "-1.0,-1.0,3.0,1.0,15.0"); //m_nacadomain
    }

    module->SetDefaults();
    module->Process();

    // 打印数据
    cout << "surf: "<< mesh->m_cad->GetNumSurf() << endl;



    ////**** OCTREE ****////
    module = GetModuleFactory().CreateInstance(
        ModuleKey(eProcessModule, "loadoctree"), mesh);
    module->SetLogger(m_log);
    module->RegisterConfig("mindel", "0.04");   //m_minDelta
    module->RegisterConfig("maxdel", "3.0");    //m_maxDelta
    module->RegisterConfig("eps", "0.1");   //m_eps
    bool m_refine = true;
    if (m_refine)
    {
        module->RegisterConfig("refinement", "0.0,0.0,0.0,3.0,0.0,0.0,0.3,0.2");    //m_refinement
    }
    bool m_curverefine=false;
    if (m_curverefine)
    {
        module->RegisterConfig("curve_refinement", ""); //m_curverefinement
    }
    bool m_woct = false;
    if (m_woct)
    {
        module->RegisterConfig("writeoctree", "");
    }

    module->SetDefaults();
    module->Process();



    bool m_makeBL = true;
    string m_blsurfs="5,6";
    string m_blthick = "0.07";
    bool m_adjust = false;
    string m_adjustment = "";
    bool m_adjustall = false;
    bool m_smoothbl = false;
    bool m_spaceoutbl = false;
    string m_spaceoutblthr = "";
    string m_nospaceoutsurf = "";
    string m_periodic = "";
    bool m_manifold = false;
    string m_bllayers = "3";
    string m_blprog = "2";

    ////**** LINEAR MESHING ****////
    if (m_2D)
    {
        ////**** 2DGenerator ****////
        mesh->m_expDim   = 2;
        mesh->m_spaceDim = 2;
        module             = GetModuleFactory().CreateInstance(
            ModuleKey(eProcessModule, "2dgenerator"), mesh);

        module->SetLogger(m_log);

        if (m_makeBL)
        {
            module->RegisterConfig("blcurves", m_blsurfs);
            module->RegisterConfig("blthick", m_blthick);

            if (m_adjust)
            {
                module->RegisterConfig("bltadjust", m_adjustment);

                if (m_adjustall)
                {
                    module->RegisterConfig("adjustblteverywhere", "");
                }
            }

            if (m_smoothbl)
            {
                module->RegisterConfig("smoothbl", "");
            }

            if (m_spaceoutbl)
            {
                module->RegisterConfig("spaceoutbl", m_spaceoutblthr);
                module->RegisterConfig("nospaceoutsurf", m_nospaceoutsurf);
            }
        }
        if (m_periodic.size())
        {
            module->RegisterConfig("periodic", m_periodic);
        }

        try
        {
            module->SetDefaults();
            module->Process();
        }
        catch (runtime_error &e)
        {
            m_log(WARNING) << "2D linear mesh generator failed with message:"
                           << endl;
            m_log(WARNING) << e.what() << endl;
            m_log(FATAL) << "No mesh file has been created." << endl;
        }
    }
    else
    {
        ////**** SurfaceMesh ****////
        module = GetModuleFactory().CreateInstance(
            ModuleKey(eProcessModule, "surfacemesh"), mesh);

        try
        {
            module->SetLogger(m_log);
            module->SetDefaults();
            module->Process();
        }
        catch (runtime_error &e)
        {
            m_log(WARNING) << "Surface meshing has failed with message:"
                           << endl;
            m_log(WARNING) << e.what() << endl;
            m_log(WARNING) << "Any surfaces which were successfully meshed will"
                           << " be written as a manifold mesh." << endl;
        }

        if (m_manifold)
        {
            // Don't want to volume mesh.
            mesh->m_expDim = 2;
        }
        else
        {
            ////**** VolumeMesh ****////
            module = GetModuleFactory().CreateInstance(
                ModuleKey(eProcessModule, "volumemesh"), mesh);

            module->SetLogger(m_log);

            if (m_makeBL)
            {
                module->RegisterConfig("blsurfs", m_blsurfs);
                module->RegisterConfig("blthick", m_blthick);
                module->RegisterConfig("bllayers", m_bllayers);
                module->RegisterConfig("blprog", m_blprog);
            }

            try
            {
                module->SetDefaults();
                module->Process();
            }
            catch (runtime_error &e)
            {
                m_log(WARNING)
                    << "Volume meshing has failed with message:" << endl;
                m_log(WARNING) << e.what() << endl;
                m_log(WARNING) << "The linear surface mesh be written as a "
                               << "manifold mesh" << endl;
            }
        }
    }

    // cout << mesh->GetNumElements() << endl;
    // if (!mesh->m_edgeSet.empty()) {
    //     // Get the first element
    //     for (const auto& edgeSharedPtr : mesh->m_edgeSet) {
    //         // Dereference the shared pointer to call GetNodeCount()
    //         std::cout << "Edge pair: id=  " << edgeSharedPtr->m_n1->m_id << "," << edgeSharedPtr->m_n1->m_x << ", " << edgeSharedPtr->m_n1->m_y << ", " << edgeSharedPtr->m_n1->m_z<< std::endl;
    //         std::cout << "Edge pair: id=  " << edgeSharedPtr->m_n2->m_id << "," << edgeSharedPtr->m_n2->m_x << ", " << edgeSharedPtr->m_n2->m_y << ", " << edgeSharedPtr->m_n2->m_z<< std::endl;
    //     }
    //     // auto firstEdge = *mesh->m_edgeSet.begin(); // need to derenference

    //     // // Dereference the shared pointer to call GetNodeCount()
    //     // std::cout << "First edge node : " << firstEdge->m_n1->m_x << "," << firstEdge->m_n1->m_y << ", " << firstEdge->m_n1->m_z << std::endl;
    // } else {
    //     std::cout << "EdgeSet is empty." << std::endl;
    // }
}
GLWidget::~GLWidget() {}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);    // 启用深度测试
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // 灰色背景
}

void GLWidget::resizeGL(int w, int h)
{
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.01f, 100.0f);  // 设置透视投影矩阵
}

void GLWidget::setMesh(MeshSharedPtr m_mesh){
    mesh = m_mesh;
}

void GLWidget::wheelEvent(QWheelEvent *event)
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

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲区和深度缓冲区

    QMatrix4x4 modelView;
    modelView.translate(translateX, translateY, -1.0f); // 平移相机位置
    modelView.scale(scaleFactor);
    modelView.rotate(rotationX, 1.0f, 0.0f, 0.0f);  // 绕X轴旋转
    modelView.rotate(rotationY, 0.0f, 1.0f, 0.0f);  // 绕Y轴旋转立方体

    QMatrix4x4 mvp = projection * modelView;    // 计算模型视图投影矩阵
    glLoadMatrixf(mvp.constData()); // 加载模型视图投影矩阵到OpenGL




    // drawCube();
    // drawCAD(mesh);
    drawMesh(mesh);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos(); // 记录鼠标按下位置
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
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

void GLWidget::drawRefinement(float x1, float y1, float z1, float x2, float y2, float z2){
    glColor3f(0.0f, 0.0f, 1.0f); // 设置颜色为蓝色
    glBegin(GL_LINES);
    glVertex3f(x1,y1,z1);
    glVertex3f(x2,y2,z2);
    glEnd();
}

void GLWidget::setRefinement(vector<float> floatValues){
    cout << "setting refinement" << endl;
    if(floatValues.size()==8){
        x1 = floatValues[0];
        y1 = floatValues[1];
        z1 = floatValues[2];
        x2 = floatValues[3];
        y2 = floatValues[4];
        z2 = floatValues[5];
        qDebug() << "setting refinements";
    }
    else{
        x1=x2=0.0;
        y1=y2=0.0;
        z1=z2=0.0;
        qDebug()<< "floatValues not 8";
    }
}
void GLWidget::drawMesh(MeshSharedPtr mesh)
{
    if(x1!=x2 || y1!=y2 || z1!=z2){
        cout << "drawing refinement" << endl;
        drawRefinement(x1,y1,z1,x2,y2,z2);
    }

    glColor3f(1.0, 0.0, 0.0); // red lines
    glBegin(GL_LINES);

    if (mesh->m_cad) {
        for (int i = 1; i <= mesh->m_cad->GetNumCurve(); i++){
            CADCurveSharedPtr curve = mesh->m_cad->GetCurve(i);
            array<Nektar::NekDouble, 21> propotion; // 20 intermedieate vertices for one curve
            for(int i=0;i<21;i++){
                propotion[i]=0.05*i;
            }
            for(int i=0;i<20;i++){
                array<Nektar::NekDouble, 3> loc = curve->P(propotion[i]);
                glVertex3f(static_cast<double>(loc[0]), static_cast<double>(loc[1]), static_cast<double>(loc[2]));
                cout << loc[0] << ", " << loc[1] <<", " << loc[2] << endl;
                loc = curve->P(propotion[i+1]);
                glVertex3f(static_cast<double>(loc[0]), static_cast<double>(loc[1]), static_cast<double>(loc[2]));
            }
        }


    } else {
        std::cout << "no CAD loaded" << std::endl;
    }


    glEnd();



    glColor3f(0.0f, 1.0f, 0.0f); // 设置颜色为绿色
    glPointSize(5.0f); // 设置点的大小
    glBegin(GL_POINTS);
    if (mesh->m_cad){
        cout << "curve number: " << mesh->m_cad->GetNumCurve() << endl;
        for (int i = 1; i <= mesh->m_cad->GetNumCurve(); i++){
            CADCurveSharedPtr curve = mesh->m_cad->GetCurve(i);
            vector<CADVertSharedPtr> verts =  curve->GetVertex();

            for (size_t i = 0; i < verts.size(); ++i) {
                std::array<Nektar::NekDouble, 3> loc = verts[i]->GetLoc();
                glVertex3f(static_cast<double>(loc[0]), static_cast<double>(loc[1]), static_cast<double>(loc[2]));
                // cout << loc[0] << ", " << loc[1] <<", " << loc[2] << endl;
            }
        }
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // 设置颜色为白色
    glBegin(GL_LINES);
    if (!mesh->m_edgeSet.empty()) {
        qDebug() << "draw the mesh";
        cout << mesh->m_element.empty() << endl;
        for (const auto& edgeSharedPtr : mesh->m_edgeSet) {
            glVertex3f(static_cast<GLfloat>(edgeSharedPtr->m_n1->m_x), static_cast<GLfloat>(edgeSharedPtr->m_n1->m_y),  static_cast<GLfloat>(edgeSharedPtr->m_n1->m_z));
            glVertex3f(static_cast<GLfloat>(edgeSharedPtr->m_n2->m_x), static_cast<GLfloat>(edgeSharedPtr->m_n2->m_y),  static_cast<GLfloat>(edgeSharedPtr->m_n2->m_z));
        }
    } else {
        std::cout << "Mesh has an empty edge set" << std::endl;
        cout << mesh->m_edgeSet.empty() << endl;
        cout << mesh->m_faceSet.empty() << endl;
        cout << mesh->m_vertexSet.empty() << endl;
        cout << mesh->m_element.empty() << endl;
    }


    if (mesh->m_cad){
        vector<int> index;
        for (int i = 1; i <= mesh->m_cad->GetNumCurve(); i++){
            CADCurveSharedPtr curve = mesh->m_cad->GetCurve(i);
            array<Nektar::NekDouble, 3> loc = curve->P(0);
            array<Nektar::NekDouble, 3> loc1 = curve->P(1);
            if(std::abs(loc[0] - loc1[0]) <= 0.001 || std::abs(loc[1] - loc1[1]) <= 0.001){
                index.push_back(i);
            }
        }
        for (int num : index) {
            CADCurveSharedPtr curve = mesh->m_cad->GetCurve(num);
            vector<CADVertSharedPtr> verts =  curve->GetVertex();
            for (size_t i = 0; i < verts.size(); ++i) {
                std::array<Nektar::NekDouble, 3> loc = verts[i]->GetLoc();
                glVertex3f(static_cast<double>(loc[0]), static_cast<double>(loc[1]), static_cast<double>(loc[2]));
                cout << loc[0] << ", " << loc[1] <<", " << loc[2] << endl;

            }
        }
    }

    glEnd();


}

void GLWidget::drawCube()
{
    static const GLfloat vertices[] = {
        1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
    };

    static const GLubyte indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0,  // front
        4, 5, 5, 6, 6, 7, 7, 4,  // back
        0, 4, 1, 5, 2, 6, 3, 7   // sides
    };

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glVertexPointer(3, GL_FLOAT, 0, vertices); // 设置顶点数组指针
    //glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices); // 绘制立方体的各个面

    glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, indices);
    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
}

void GLWidget::setCADData(std::shared_ptr<CADData> cadData) {
    this->cadData = cadData;
    update();
}

void GLWidget::drawCAD(MeshSharedPtr mesh){

    glColor3f(1.0, 0.0, 0.0); // 红色
    glBegin(GL_LINES);
    if (mesh->m_cad->GetNumCurve()!=0) {
        qDebug() << "should draw something";
    } else {
        std::cout << "EdgeSet is empty." << std::endl;
        glVertex3f( 0, 0,  0); glVertex3f( 0,  0,  0);
    }


    // for (int i = 1; i <= mesh->m_cad->GetNumCurve(); i++){
    //     CADCurveSharedPtr curve = mesh->m_cad->GetCurve(i);
    //     std::vector<CADVertSharedPtr> verts = curve->GetVertex();
    //     for (const auto& vert : verts) {
    //         std::array<Nektar::NekDouble, 3> loc = vert->GetLoc();
    //         glVertex3f(static_cast<double>(loc[0]), static_cast<double>(loc[1]), static_cast<double>(loc[2]));
    //     }
    // }
    glEnd();


    // if (!cadData) return;



}

