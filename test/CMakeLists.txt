add_executable(asynco_default main_default.cpp)
target_link_libraries(asynco_default asynco Boost::system)

add_executable(asynco_init main_init.cpp)
target_link_libraries(asynco_init asynco Boost::system)

add_executable(asynco_async_default main_async_default.cpp)
target_link_libraries(asynco_async_default asynco Boost::system)

add_executable(asynco_async main_async.cpp)
target_link_libraries(asynco_async asynco Boost::system)

add_executable(asynco_timers_default main_timers_default.cpp)
target_link_libraries(asynco_timers_default asynco Boost::system)

add_executable(asynco_timers main_timers.cpp)
target_link_libraries(asynco_timers asynco Boost::system)

add_executable(asynco_trigger_default main_trigger_default.cpp)
target_link_libraries(asynco_trigger_default asynco Boost::system)

add_executable(asynco_trigger main_trigger.cpp)
target_link_libraries(asynco_trigger asynco Boost::system)

add_executable(asynco_coroutine_default main_coroutine_default.cpp)
target_link_libraries(asynco_coroutine_default asynco Boost::system)

add_executable(asynco_coroutine main_coroutine.cpp)
target_link_libraries(asynco_coroutine asynco Boost::system)