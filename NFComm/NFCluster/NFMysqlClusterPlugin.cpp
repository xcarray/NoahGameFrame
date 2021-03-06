// NFDataBasePlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFDataBasePlugin.h"
#include "NFCDataBaseModule.h"

NFIPlugin* pPlugin = NULL;
NFIPluginManager* pPluginManager = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPluginManager = pm;

    CREATE_PLUGIN(pm, NFDataBasePlugin, pPlugin)
};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, pPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFMysqlClusterPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMysqlClusterPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFMysqlClusterPlugin)
}

void NFMysqlClusterPlugin::Install()
{
    //因为有好几个进程都在用此插件，隐藏此插件不能设置Title
    //SetConsoleTitle( "NFDataBaseServer" );
    REGISTER_MODULE(pPluginManager, "NFCDataBaseModule", NFCDataBaseModule)
}

void NFMysqlClusterPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCDataBaseModule", NFCDataBaseModule)
}