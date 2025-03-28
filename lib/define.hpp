#ifndef _ASYNCO_DEFINE_
#define _ASYNCO_DEFINE_

namespace marcelb {
namespace asynco {

/**
 * Alternative names of functions - mostly for the sake of more beautiful coloring of the code
 */

#define async_ marcelb::asynco::async_
#define await_ marcelb::asynco::await_

#if __cplusplus >= 202002L
#define asyncable boost::asio::awaitable
#endif

}
}



#endif