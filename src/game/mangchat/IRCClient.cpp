/*
 * MangChat By |Death| And Cybrax
 *
 * This Program Is Free Software; You Can Redistribute It And/Or Modify It Under The Terms 
 * Of The GNU General Public License
 * Written and Developed by Cybrax. cybraxvd@gmail.com
 * |Death| <death@hell360.net>, Lice <lice@yeuxverts.net>, Dj_baby & Sanaell, Tase
 * Modified for patch 3.0.8 by Shinzon <shinzon@wowgollum.com>
 * With Help And Support From The MaNGOS Project Community.
 * PLEASE RETAIN THE COPYRIGHT OF THE AUTHORS.
 */
#include "IRCClient.h"
#include "../World.h"
#include "../ObjectMgr.h"
#include "../MapManager.h"
#include <ace/Thread.h>

#include "Policies/SingletonImp.h"
INSTANTIATE_SINGLETON_1( IRCClient );

#ifdef WIN32
    #define Delay(x) Sleep(x)
#else
    #define Delay(x) sleep(x / 1000)
#endif
// IRCClient Constructor
IRCClient::IRCClient()
{
    for(int i = 0;i > 5;i++)
        sIRC.Script_Lock[i] = false;
}
// IRCClient Destructor
IRCClient::~IRCClient(){}

// ZThread Entry This function is called when the thread is created in Master.cpp (mangosd)
void IRCClient::run()
{
    sIRC.iLog.WriteLog(" %s : ****** MaNGOS With MangChat Has Been Started ******", sIRC.iLog.GetLogDateTimeStr().c_str());

    // before we begin we wait a few 
    // mangos is still starting up.
    ACE_Based::Thread::Sleep(1000);
    sLog.outString("\n%s\n%s\n%s\n%s",
        "***************************************",
        "**   MangChat Threaded IRC Client    **",
        "**     With Enhanced GM Control.     **",
        "***************************************");
    sLog.outString("****** MangChat: %s ********", sIRC._Mver.c_str());
    int cCount = 0;
    // Clean Up MySQL Tables
    sLog.outString("*** MangChat: Cleaning Up Inchan Table*");
    WorldDatabase.PExecute("DELETE FROM `IRC_Inchan`");
    sIRC._Max_Script_Inst = 0;
    // Create a loop to keep the thread running untill active is set to false
    while(sIRC.Active && !World::m_stopEvent)
    {
        // Initialize socket library
        if(this->InitSock())
        {
            // Connect To The IRC Server
            sLog.outString("*** MangChat: Connecting to %s Try # %d ******", sIRC._Host.c_str(), cCount);
            if(this->Connect(sIRC._Host.c_str(), sIRC._Port))
            {
                // On connection success reset the connection counter
                cCount = 0;
                sLog.outString("*** MangChat: Connected And Logging In*");
                // Login to the IRC server
                if(this->Login(sIRC._Nick, sIRC._User, sIRC._Pass))
                {
                    sLog.outString("*** MangChat: Logged In And Running!! *");
                    // While we are connected to the irc server keep listening for data on the socket
                    while(sIRC.Connected && !World::m_stopEvent){ sIRC.SockRecv(); }
                }
                sLog.outString("*** MangChat: Connection To IRC Server Lost! ***");
            }
            // When an error occures or connection lost cleanup
            Disconnect();
            // Increase the connection counter
            cCount++;
            // if MAX_CONNECT_ATTEMPT is reached stop trying
            if(sIRC._MCA != 0 && cCount == sIRC._MCA)
                sIRC.Active = false;
            // If we need to reattempt a connection wait WAIT_CONNECT_TIME milli seconds before we try again
            if(sIRC.Active)
                ACE_Based::Thread::Sleep(sIRC._wct);
        }
        else
        {
            // Socket could not initialize cancel
            sIRC.Active = false;
            sLog.outError("** MangChat: Could not initialize socket");
        }
    }
    // we need to keep the thread alive or mangos will crash
    // when sending chat or join/leave channels.
    // even when we are not connected the functions must still
    // be availlable where chat is sent to so we keep it running
    while(!World::m_stopEvent){};
}
