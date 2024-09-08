#ifndef NEKMESHOBJECT_H
#define NEKMESHOBJECT_H
#include <LibUtilities/BasicConst/GitRevision.h>
#include <LibUtilities/BasicUtils/Timer.h>
#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <string>

#include <NekMesh/Module/Module.h>
using namespace std;
using namespace Nektar::NekMesh;
class NekMeshObject{
public:
    MeshSharedPtr mesh = std::shared_ptr<Mesh>(new Mesh());
    vector<ModuleSharedPtr> modules;
    std::shared_ptr<Logger> log;
    NekMeshObject(std::shared_ptr<Logger> log) : log(log){
    }
    ~NekMeshObject(){
    }
    void addInputModule(string inputFile);
    void addOutputModule(string filePath, string fileType, string fileName);
    void addProcessModule(map<string, string>values);
    void process();

};

#endif // NEKMESHOBJECT_H
