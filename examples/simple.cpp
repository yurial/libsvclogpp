#include <stdlib.h>
#include <svclog.hpp>

struct subsystem
{
    enum enum_t
    {
    none = svclog_t::nosubsystem,
    s0,
    s1,
    s2,
    max,
    };
};

void test(int subsysytem)
{
svclog( subsysytem, LOG_ERR )       << "err"    << std::endl;
svclog( subsysytem, LOG_NOTICE )    << "notice" << std::endl;
svclog( subsysytem, LOG_DEBUG )     << "debug"  << std::endl;
}

int main(int argc, char* argv[])
{
svclog.open( "simple svclog test", LOG_PID, LOG_USER );

svclog.init_subsystems( subsystem::max );
svclog.setmask( subsystem::s0, LOG_MASK( LOG_ERR ) | LOG_MASK( LOG_NOTICE ), "s0: " );
svclog.setmask( subsystem::s1, LOG_MASK( LOG_NOTICE ) | LOG_MASK( LOG_DEBUG ), "s1: " );
svclog.setmask( subsystem::s2, 0, "s2: " );

svclog( subsystem::none, LOG_INFO ) << "subsystem no" << std::endl;
test( subsystem::none );

svclog( subsystem::none, LOG_INFO ) << "subsystem s0" << std::endl;
test( subsystem::s0 );

svclog( subsystem::none, LOG_INFO ) << "subsystem s1" << std::endl;
test( subsystem::s1 );

svclog( subsystem::none, LOG_INFO ) << "subsystem s2" << std::endl;
test( subsystem::s2 );

svclog.close();
return EXIT_SUCCESS;
}

