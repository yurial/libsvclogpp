/*
libsvclogpp - libsyslogpp extension
copyright (c) 2013  Yuri Diachenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "svclog.hpp"

std::vector<svclogbuf::subsystem_data> svclogbuf::m_subsystems;

svclog_t g_svclog;

svclog_t& svclog_get() throw(std::bad_alloc,std::runtime_error)
{
return g_svclog;
}

