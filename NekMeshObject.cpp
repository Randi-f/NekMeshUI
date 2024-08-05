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

#include <QDebug>

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
            modules[i]->PrintConfig();
            modules[i]->Process();
        }
        catch (NekMeshError &e)
        {
            return ;
        }
        t.Stop();

        log.SetPrefix(modules[i]->GetModuleName());

    }
    cout << "the faceset is: " << endl;
    // 遍历 faceSet
    for (const auto& facePtr : mesh->m_faceSet) {
        if (facePtr) {
            cout << facePtr->m_id << endl;
        }
        else{
            cout << "null" << endl;
        }
    }

}

void NekMeshObject::addProcessModule(map<string, string>values){
    ModuleKey process_module;
    process_module.first=eProcessModule;
    if(values["moduleType"]=="peralign"){
        process_module.second="peralign";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(log);
        modules.push_back(modp);
        modp->RegisterConfig("surf1", values["surf1"]);
        modp->RegisterConfig("surf2", values["surf2"]);
        modp->RegisterConfig("dir", values["dir"]);
        modp->SetDefaults();
    }
    else if(values["moduleType"]=="loadcad"){
        // MCF configuration
        mesh->m_expDim   = 3;
        mesh->m_spaceDim = 3;
        mesh->m_nummode  = 5;
        process_module.second="loadcad";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(log);
        modules.push_back(modp);
        modp->RegisterConfig("filename", values["filename"]);
        modp->RegisterConfig("voidpoints", values["voidpoints"]);
        if(values["2D"]=="true"){
            modp->RegisterConfig("2D", "");
        }
        if(values["NACA"]!="false"){
            modp->RegisterConfig("NACA", values["NACA"]);
        }
        modp->SetDefaults();
    }
    else if(values["moduleType"]=="loadoctree"){
        process_module.second="loadoctree";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(log);
        modules.push_back(modp);
        modp->RegisterConfig("mindel", values["MinDelta"]);
        modp->RegisterConfig("maxdel", values["MaxDelta"]);
        modp->RegisterConfig("eps", values["EPS"]);
        if(values["refinement"]!="false"){
            modp->RegisterConfig("refinement", values["refinement"]);
        }
        if(values["curve_refinement"]=="true"){
            modp->RegisterConfig("curve_refinement", "");
        }
        if(values["writeoctree"]=="true"){
            modp->RegisterConfig("writeoctree", "");
        }

        modp->SetDefaults();
    }
    else if(values["moduleType"]=="2dgenerator"){
        process_module.second="2dgenerator";
        mesh->m_expDim   = 2;
        mesh->m_spaceDim = 2;
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(log);
        modules.push_back(modp);

        if(values["makeBL"]!="false"){
            modp->RegisterConfig("blcurves", values["blcurves"]);
            modp->RegisterConfig("blthick", values["blthick"]);
            if(values["adjust"]=="true"){
                modp->RegisterConfig("bltadjust", "todo");
                if (values["adjustall"]=="true")
                {
                    modp->RegisterConfig("adjustblteverywhere", "");
                }
            }
        }

        // Ensure configuration options have been set.
        modp->SetDefaults();
    }
    else if(values["moduleType"]=="volumemesh"){
        process_module.second="volumemesh";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(log);
        modules.push_back(modp);
        if(values["makeBL"]=="true"){
            modp->RegisterConfig("blsurfs", values["blsurfs"]);
            modp->RegisterConfig("blthick", values["blthick"]);
            modp->RegisterConfig("bllayers", values["bllayers"]);
            modp->RegisterConfig("blprog", values["blprog"]);
        }

        // Ensure configuration options have been set.
        modp->SetDefaults();
    }
    else if(values["moduleType"]=="hosurface"){
        process_module.second="hosurface";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(log);
        modules.push_back(modp);
        if(values["surfopti"]=="true"){
            modp->RegisterConfig("no_opti", "");
        }

        // Ensure configuration options have been set.
        modp->SetDefaults();
    }
    else if(values["moduleType"]=="bl"){
        process_module.second="bl";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(log);
        modules.push_back(modp);
        modp->RegisterConfig("layers", values["bllayers"]);
        modp->RegisterConfig("surf", values["blsurfs"]);
        mesh->m_nummode  = boost::lexical_cast<int>(values["order"]) + 1;
        modp->RegisterConfig("nq", boost::lexical_cast<string>(mesh->m_nummode));
        modp->RegisterConfig("r", values["blprog"]);
        modp->SetDefaults();
    }

}

void NekMeshObject::addInputModule(string inputFile){
    const string suffix = ".geo";
    if(inputFile.compare(inputFile.length() - suffix.length(), suffix.length(), suffix) == 0){
        cout << "detect CAD file: " << inputFile << endl;;
        map<string, string>values;
        values["moduleType"]="loadcad";
        values["filename"] = inputFile;
        values["voidpoints"] = "";
        values["2D"] = "true";
        values["NACA"] = "false";
        addProcessModule(values);
    }
    else{
        cout << "input loading:" << inputFile;
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
        mod->SetDefaults();
        modules.push_back(mod);
    }

}

void NekMeshObject::addOutputModule(string filePath, string fileType){
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
