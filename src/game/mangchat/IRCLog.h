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
#ifndef _IRC_LOG_H
#define _IRC_LOG_H

#include "Common.h"
#include <fstream>

class IRCLog
{
    public:
        IRCLog();
        ~IRCLog();

    public:
        void WriteLog(const char *what, ...);
		std::string GetLogDateStr() const;
		std::string GetLogDateTimeStr() const;
    private:
        std::ofstream ircLogfile;
};


#endif