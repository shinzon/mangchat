/* TriniChat Trinity Core IRC Client
 *
 * This Program Is Free Software; You Can Redistribute It And/Or Modify It Under The Terms
 * Of The GNU General Public License
 * Written And Developed By |Death|(death@hell360.net)
 * - Based Off Of MangChat 1.0 By Cybrax(cybraxvf@gmail.com)
 * PLEASE RETAIN THE COPYRIGHT OF THE AUTHORS.*/
#ifndef MC_CONFIG_H
#define MC_CONFIG_H

#include "../../framework/Platform/CompilerDefs.h"

// Format is YYYYMMDDRR where RR is the change in the conf file
// for that day.
#define MangChat_CONF_VERSION    2008011901

#if PLATFORM == PLATFORM_WINDOWS
  #define _MangChat_CONFIG  "mangchat.conf"
#else
  #define _MangChat_CONFIG  "@sysconfdir@/mangchat.conf"
#endif

#endif