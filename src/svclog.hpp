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

#ifndef SVCLOG_HPP
#define SVCLOG_HPP

#ifdef HAVE_SYSLOG_H
    #include <syslog.h>
    #ifndef SYSLOGPP_DEFAULT_PRIO
        #define SYSLOGPP_DEFAULT_PRIO LOG_ALERT
    #endif
#else
    #ifndef SYSLOGPP_DEFAULT_PRIO
        #define SYSLOGPP_DEFAULT_PRIO 0
    #endif
    extern "C"
        {
        void openlog(const char *ident, int option, int facility);
        void svclog(int priority, const char *format, ...);
        void closelog(void);
        void vsvclog(int priority, const char *format, va_list ap);
        int  setlogmask(int mask);
        }
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

class svclogbuf:
    public std::stringbuf
{
protected:
    enum
    {
    nosubsystem = -1
    };

    enum
    {
    masknone = 0,
    maskall = -1
    };

    struct subsystem_data
    {
    int         m_mask;
    std::string m_prefix;

    inline  subsystem_data();
    };

static std::vector<subsystem_data> m_subsystems;

                int     m_subsystem;
                int     m_prio;

inline                  svclogbuf(int subsystem, int prio);
inline          void    init_subsystems(size_t count);
inline          int     getsubsystem() const;
inline          void    setsubsystem(int subsystem);
inline          void    setprio(int prio);
inline          int     setmask(int subsystem, int mask);
inline          int     setmask(int subsystem, int mask, std::string prefix);
inline          int     getmask(int subsysytem) const;
inline          int     sync();
inline static   bool    testprio(int subsystem, int prio);
inline static   void    log(int subsystem, int prio, char* msg);
};

class svclog_t:
    protected svclogbuf,
    public std::ostream
{
public:
enum { nosubsystem = svclogbuf::nosubsystem };

inline                  svclog_t(int prio = SYSLOGPP_DEFAULT_PRIO);
inline                  svclog_t(int subsystem, int prio);
inline                  ~svclog_t();

inline  void            open(const char* procname, int option, int facility);
inline  void            close();

using                   svclogbuf::init_subsystems;
using                   svclogbuf::getmask;
using                   svclogbuf::setmask;
inline  int             setmask(int mask);

inline  std::ostream&   operator () (int prio);
inline  std::ostream&   operator () (int subsystem, int prio);

inline  void            operator () (int prio, const char* fmt, ...) const;
inline  void            operator () (int subsystem, int prio, const char* fmt, ...) const;
};

svclogbuf::subsystem_data::subsystem_data():
    m_mask( maskall )
{
}

svclogbuf::svclogbuf(int subsystem, int prio):
    m_subsystem( subsystem ), m_prio( prio )
{
}

int svclogbuf::getsubsystem() const
{
return m_subsystem;
}

void svclogbuf::setsubsystem(int subsystem)
{
pubsync();
m_subsystem = subsystem;
}

void svclogbuf::init_subsystems(size_t count)
{
m_subsystems.resize( count );
}

void svclogbuf::setprio(int prio)
{
pubsync();
m_prio = prio;
}

int svclogbuf::setmask(int subsystem, int mask)
{
if ( nosubsystem == subsystem )
    return setlogmask( mask );
int oldmask = m_subsystems.at( subsystem ).m_mask;
m_subsystems[ subsystem ].m_mask = mask;
return oldmask;
}

int svclogbuf::setmask(int subsystem, int mask, std::string prefix)
{
if ( nosubsystem == subsystem )
    return setlogmask( mask );
int oldmask = m_subsystems.at( subsystem ).m_mask;
m_subsystems[ subsystem ].m_mask = mask;
m_subsystems[ subsystem ].m_prefix = prefix;
return oldmask;
}

int svclogbuf::getmask(int subsystem) const
{
return m_subsystems[ subsystem ].m_mask;
}

bool svclogbuf::testprio(int subsystem, int prio)
{
if ( nosubsystem == subsystem )
    return true;
return m_subsystems.at( subsystem ).m_mask & (1 << prio);
}

void svclogbuf::log(int subsystem, int prio, char* msg)
{
if ( nosubsystem == subsystem )
    syslog( prio, "%s", msg );
else if ( testprio( subsystem, prio ) )
    {
    const std::string& prefix = m_subsystems[ subsystem ].m_prefix;
    syslog( prio, "%s%s", prefix.c_str(), msg );
    }
}

int svclogbuf::sync()
{
if ( in_avail() )
    {
    sputc( '\0' );
    log( m_subsystem, m_prio, gptr() );
    seekoff( 0, std::ios_base::beg, std::ios_base::in | std::ios_base::out );
    setg( pbase(), pbase(), pbase() );
    }
return 0; //success
}

svclog_t::svclog_t(int prio):
    svclogbuf( nosubsystem, prio ), std::ostream( static_cast<std::streambuf*>( this ) )
{
}

svclog_t::svclog_t(int subsystem, int prio):
    svclogbuf( subsystem, prio ), std::ostream( static_cast<std::streambuf*>( this ) )
{
}

svclog_t::~svclog_t()
{
pubsync();
}

void svclog_t::open(const char* procname, int option, int facility)
{
openlog( procname, option, facility );
}

void svclog_t::close()
{
closelog();
}

int svclog_t::setmask(int mask)
{
return setmask( nosubsystem, mask );
}

std::ostream& svclog_t::operator () (const int prio)
{
setprio( prio );
return *this;
}

std::ostream& svclog_t::operator () (const int subsystem, const int prio)
{
setsubsystem( subsystem );
setprio( prio );
return *this;
}

void svclog_t::operator () (int prio, const char* fmt, ...)const
{
if ( !testprio( getsubsystem(), prio ) )
    return;
char* msg = NULL;
va_list args;
va_start( args, fmt );
vasprintf( &msg, fmt, args );
va_end( args );
log( getsubsystem(), prio, msg );
free( msg );
}

void svclog_t::operator () (int subsystem, int prio, const char* fmt, ...) const
{
if ( !testprio( subsystem, prio ) )
    return;
char* msg = NULL;
va_list args;
va_start( args, fmt );
vasprintf( &msg, fmt, args );
va_end( args );
log( subsystem, prio, msg );
free( msg );
}

svclog_t& svclog_get() throw(std::bad_alloc,std::runtime_error);
#define svclog svclog_get()

#endif

