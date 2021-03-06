// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyServerNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyServerNet_ClientModule
//
//     @Changed         : NFCProxyConnectObject Add HeartBeat by hahaya 2013-11-1
// -------------------------------------------------------------------------

#ifndef _NFC_PROXYSERVER_NETCLIENT_MODULE_H_
#define _NFC_PROXYSERVER_NETCLIENT_MODULE_H_

#include <string>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFCHeartBeatManager.h"
#include "NFComm/NFPluginModule/NFIProxyServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"


//这个连接是连的gameserver
class NFCProxyConnectToGameServer : public NFINetModule
{
public:
    NFCProxyConnectToGameServer(int nGameServerID, const std::string& strIP, const int nPort, NFIPluginManager* p);

    virtual bool Execute(const float fFrameTime, const float fTotalTime);
    virtual void LogRecive(const char* str){}
    virtual void LogSend(const char* str){}

protected:

    int OnRecivePack(const NFIPacket& msg);
    int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const int nAddress);
    //有连接
    void OnClientConnected(const int nAddress);

    void Register();
    void UnRegister();

    void OnAckEnterGame(const NFIPacket& msg);
private:

    int mnGameServerID;

    NFIElementInfoModule* m_pElementInfoModule;
    NFILogModule* m_pLogModule;
    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIProxyLogicModule* m_pProxyLogicModule;
	NFILogicClassModule* m_pLogicClassModule;

};

//策略是，开始先连到world，然后拿到gameserverlist，再连gameserver，因此这个连接是连到worldserver
class NFCProxyServerToWorldModule : public NFIProxyServerToWorldModule
{
public:

    NFCProxyServerToWorldModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

    virtual int Transpond(int nGameServerID, const NFIPacket& msg);

    virtual bool VerifyConnectData(const std::string& strAccount, const std::string& strKey);

    virtual GameData* GetGameData(int nGameID);
    virtual GameDataMap& GetGameDataMap() { return mGameDataMap; }

protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	//连接丢失,删2层(连接对象，帐号对象)
	void OnClientDisconnect(const int nAddress);
	//有连接
	void OnClientConnected(const int nAddress);

    void Register();
    void UnRegister();

    int OnSelectServerResultProcess(const NFIPacket& msg);
    int OnGameInfoProcess(const NFIPacket& msg);
private:
    struct ConnectData 
    {
        ConnectData()
        {
            strAccount = "";
            strConnectKey = "";
        }

        std::string strAccount;
        std::string strConnectKey;
    };


private:
    NFMapEx<std::string, ConnectData> mWantToConnectMap;
    GameDataMap mGameDataMap;
private:

    NFILogModule* m_pLogModule;
    NFIProxyLogicModule* m_pProxyLogicModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;

};

#endif