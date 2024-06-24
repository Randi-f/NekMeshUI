/*
 * @Author: shihan
 * @Date: 2024-04-18 14:13:30
 * @version: 1.0
 * @description:
 */
#include "NekMeshObject.h"
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


void NekMeshObject::process(){
    // Run mesh process.
    for (int i = 0; i < modules.size(); ++i)
    {
        Nektar::LibUtilities::Timer t;
        t.Start();
        try
        {
            modules[i]->GetLogger().SetPrefixLen(11);
            cout << "try process for " << modules[i]->GetModuleName() << endl;
            modules[i]->Process();
        }
        catch (NekMeshError &e)
        {
            return ;
        }
        t.Stop();

        log.SetPrefix(modules[i]->GetModuleName());

    }
}

void NekMeshObject::addProcessModule(map<string, string>values){
    ModuleKey process_module;
    process_module.first=eProcessModule;
    process_module.second="peralign";
    ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
    modp->SetLogger(log);
    modules.push_back(modp);
    modp->RegisterConfig("surf1", values["surf1"]);
    modp->RegisterConfig("surf2", values["surf2"]);
    modp->RegisterConfig("dir", values["dir"]);
    // Ensure configuration options have been set.
    modp->SetDefaults();
}

void NekMeshObject::addInputModule(string inputFile){
    ModuleKey in_module;
    in_module.first=eInputModule;
    // 找到最后一个点的位置
    size_t pos = inputFile.find_last_of(".");
    if (pos != std::string::npos) {
        // 返回点之后的所有字符
        in_module.second=inputFile.substr(pos + 1);
    } else {
        // 如果没有找到点，返回空字符串
        return ;
    }
    // Create module.
    ModuleSharedPtr mod = GetModuleFactory().CreateInstance(in_module, mesh);
    mod->SetLogger(log);
    mod->RegisterConfig("infile", inputFile);
    // Ensure configuration options have been set.
    mod->SetDefaults();
    modules.push_back(mod);
}

void NekMeshObject::addOutputFile(string filePath, string fileType){
    ModuleKey out_module;
    out_module.first=eOutputModule;
    out_module.second=fileType;
    ModuleSharedPtr mod1 = GetModuleFactory().CreateInstance(out_module, mesh);
    mod1->SetLogger(log);
    modules.push_back(mod1);
    string fileName = "/Users/shihan/Desktop/output."+fileType;
    mod1->RegisterConfig("outfile", fileName);
    // Ensure configuration options have been set.
    mod1->SetDefaults();
}
