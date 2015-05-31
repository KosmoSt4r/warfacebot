/**
 * WarfaceBot, a blind XMPP client for Warface (FPS)
 * Copyright (C) 2015 Levak Borok <levak92@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <wb_tools.h>
#include <wb_stream.h>
#include <wb_xml.h>
#include <wb_xmpp.h>
#include <wb_xmpp_wf.h>

#include <stdlib.h>
#include <unistd.h>

#ifdef __MINGW32__
# include <windows.h>
# define sleep(x) Sleep(x)
#endif

void xmpp_send_message(int wfs,
                       const char *from_login, const char *from_jid,
                       const char *to_login, const char *to_jid,
                       const char *msg, const char *answer_id)
{
    char *serialized = xml_serialize(msg);
    const char *mid = NULL;
    t_uid id;

    if (answer_id)
        mid = answer_id;
    else
    {
        idh_generate_unique_id(&id);
        mid = (char *) &id;
        sleep(rand() % 2 + 1); /* Take our time to answer */
    }

    send_stream_format(wfs,
                       "<iq from='%s' to='%s' type='%s' id='%s'>"
                       "<query xmlns='urn:cryonline:k01'>"
                       "<message from='%s' nick='%s' message='%s'/>"
                       "</query>"
                       "</iq>",
                       from_jid, to_jid,
                       answer_id ? "result" : "get",
                       mid,
                       from_login, to_login, serialized);

    free(serialized);
}
