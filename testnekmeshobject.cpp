#include "testnekmeshobject.h"

void TestNekMeshObject::initTestCase() {
    // Create a logger.
    auto logOutput = std::make_shared<StreamOutput>(std::cout);
    // Logger log(logOutput, INFO);
    auto log = std::make_shared<Logger>(logOutput, INFO);
    // Initialization code here
    nekMeshObjectPtr = make_shared<NekMeshObject>(log);
}

void TestNekMeshObject::cleanupTestCase() {
    // Cleanup after all tests have run
}

// void TestNekMeshObject::init() {
//     // Initialization that needs to happen before each test
// }

// void TestNekMeshObject::cleanup() {
//     // Cleanup after each test
// }

void TestNekMeshObject::testExample() {
    // Example test
    QCOMPARE(1 + 1, 2); // Check if 1 + 1 equals 2
}

void TestNekMeshObject::testAddInputModule() {
    string fileName = "/Users/shihan/Desktop/naca.mcf";
    nekMeshObjectPtr->addInputModule(fileName);
    QCOMPARE(nekMeshObjectPtr->modules.size(), 1);
}

void TestNekMeshObject::testDetectCADFile(){
    // Create a logger.
    auto logOutput = std::make_shared<StreamOutput>(std::cout);
    // Logger log(logOutput, INFO);
    auto log = std::make_shared<Logger>(logOutput, INFO);
    nekMeshObjectPtr = std::make_shared<NekMeshObject>(log);
    string fileName = "/Users/shihan/Desktop/t106a.geo";
    nekMeshObjectPtr->addInputModule(fileName);
    QCOMPARE(nekMeshObjectPtr->modules[0]->GetModuleName(),"LoadCAD");
}

void TestNekMeshObject::testAddProcessModule(){
    // Create a logger.
    auto logOutput = std::make_shared<StreamOutput>(std::cout);
    // Logger log(logOutput, INFO);
    auto log = std::make_shared<Logger>(logOutput, INFO);
    nekMeshObjectPtr = std::make_shared<NekMeshObject>(log);
    map<string, string>values;
    values["moduleType"]="peralign";
    values["surf1"]="3";
    values["surf2"]="4";
    values["dir"]="x";
    nekMeshObjectPtr->addProcessModule(values);
    QCOMPARE(nekMeshObjectPtr->modules[0]->GetModuleName(),"ProcessPerAlign");
}

void TestNekMeshObject::testAddOutputModule(){
    // Create a logger.
    auto logOutput = std::make_shared<StreamOutput>(std::cout);
    // Logger log(logOutput, INFO);
    auto log = std::make_shared<Logger>(logOutput, INFO);
    nekMeshObjectPtr = std::make_shared<NekMeshObject>(log);
    nekMeshObjectPtr->addOutputModule("/Users/shihan/Desktop", "xml", "default");
    QCOMPARE(nekMeshObjectPtr->modules[0]->GetModuleName(),"OutputNekpp");
}

void TestNekMeshObject::testAnotherFunction() {
    // Test another function
    QVERIFY(true); // Assert that the expression is true
}
