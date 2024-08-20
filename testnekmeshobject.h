#ifndef TESTNEKMESHOBJECT_H
#define TESTNEKMESHOBJECT_H
#include <QObject>
#include <QtTest/QtTest>

#include "NekMeshObject.h"
using namespace std;
class TestNekMeshObject : public QObject {
    Q_OBJECT

private:
    shared_ptr<NekMeshObject> nekMeshObjectPtr;

private slots:
    void initTestCase();    // Called before the first test function is executed
    void cleanupTestCase(); // Called after the last test function is executed
    // void init();            // Called before each test function is executed
    // void cleanup();         // Called after each test function is executed

    // Your test functions
    void testExample();
    void testAnotherFunction();
    void testAddInputModule();
    void testDetectCADFile();
    void testAddProcessModule();
    void testAddOutputModule();
};
#endif // TESTNEKMESHOBJECT_H
