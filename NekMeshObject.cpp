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
#include <QMessageBox>
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
            QMessageBox::information(nullptr, "Error", "This is an error message.");
            cout << "an error occurred. Please try again." << endl;
        }
        t.Stop();

        log->SetPrefix(modules[i]->GetModuleName());

    }

}

void NekMeshObject::addProcessModule(map<string, string>values){
    ModuleKey process_module;
    process_module.first=eProcessModule;
    if(values["moduleType"]=="peralign"){
        process_module.second="peralign";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(*log);
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
        modp->SetLogger(*log);
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
        modp->SetLogger(*log);
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
        modp->SetLogger(*log);
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
        modp->SetDefaults();
    }
    else if(values["moduleType"]=="volumemesh"){
        process_module.second="volumemesh";
        ModuleSharedPtr modp = GetModuleFactory().CreateInstance(process_module, mesh);
        modp->SetLogger(*log);
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
        modp->SetLogger(*log);
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
        modp->SetLogger(*log);
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
    string nacaStr = "naca-";
    const string suffix = ".geo";
    if(inputFile.find(nacaStr) != std::string::npos){
        cout << "use naca example" << endl;
        QMessageBox::information(nullptr, "Load naca example","NACA configuration: filename 6412, -1.0,-1.0,3.0,1.0,15.0");
        map<string, string>values;
        values["moduleType"]="loadcad";
        values["filename"] = "6412";
        values["voidpoints"] = "";
        values["2D"] = "true";
        values["NACA"] = "-1.0,-1.0,3.0,1.0,15.0";
        addProcessModule(values);
    }
    else if(inputFile.compare(inputFile.length() - suffix.length(), suffix.length(), suffix) == 0){
        map<string, string>values;
        // 弹出询问框，包含 "Yes", "No", "Cancel" 按钮
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Detect CAD file, need extra information", "Is this 3D? If yes, please click 'Yes'.",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        // 检查用户按下的按钮
        if (reply == QMessageBox::Yes) {
            qDebug("User clicked Yes.");
            values["2D"] = "false"; // to do: distinguish 2D and 3D here
        } else if (reply == QMessageBox::No) {
            qDebug("User clicked No.");
            values["2D"] = "true";
        } else {
            qDebug("User clicked Cancel.");
            return;
        }

        values["moduleType"]="loadcad";
        values["filename"] = inputFile;
        values["voidpoints"] = "";

        // 弹出询问框，包含 "Yes", "No", "Cancel" 按钮
        // QMessageBox::StandardButton reply2;
        // reply2 = QMessageBox::question(nullptr, "Detect CAD file, need extra information", "Is this naca? If yes, please click 'Yes'.",
        //                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        // 检查用户按下的按钮
        // if (reply2 == QMessageBox::Yes) {
        //     qDebug("User clicked Yes.");
        //     values["NACA"] = "true";
        // } else if (reply2 == QMessageBox::No) {
        //     qDebug("User clicked No.");
        //     values["NACA"] = "false";
        // } else {
        //     qDebug("User clicked Cancel.");
        //     return;
        // }
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
        mod->SetLogger(*log);
        mod->RegisterConfig("infile", inputFile);
        mod->SetDefaults();
        modules.push_back(mod);
    }

}

void NekMeshObject::addOutputModule(string filePath, string fileType, string fileName){
    ModuleKey out_module;
    out_module.first=eOutputModule;
    out_module.second=fileType;
    ModuleSharedPtr mod1 = GetModuleFactory().CreateInstance(out_module, mesh);
    mod1->SetLogger(*log);
    modules.push_back(mod1);
    string outputFileName = filePath + "/" + fileName + "." + fileType;

    mod1->RegisterConfig("outfile", outputFileName);
    // Ensure configuration options have been set.
    mod1->SetDefaults();
}
