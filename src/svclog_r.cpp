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

#include <pthread.h>
#include "svclog.hpp"

std::vector<svclogbuf::subsystem> svclogbuf::m_subsystems;

pthread_once_t svclog_once = PTHREAD_ONCE_INIT;
pthread_key_t  svclog_key;

void svclog_cleanup(void* p)
{
svclog_t* log = (svclog_t*)p;
delete log;
}

void svclog_init() throw(std::bad_alloc)
{
int ret = 0;
ret = pthread_key_create( &svclog_key, svclog_cleanup );
if ( 0 != ret )
    {
    throw std::bad_alloc();
    }
}

svclog_t& svclog_get() throw(std::bad_alloc,std::runtime_error)
{
int ret = pthread_once( &svclog_once, svclog_init );
if ( 0 != ret )
    {
    throw std::runtime_error( "libsvclogpp_r: internal error (svclog_init)" );
    }
svclog_t* log = (svclog_t*)pthread_getspecific( svclog_key );
if ( NULL == log )
    {
    log = new svclog_t;
    ret = pthread_setspecific( svclog_key, log );
    if ( 0 != ret )
        {
        delete log;
        throw std::bad_alloc();
        }
    }
return *log;
}

