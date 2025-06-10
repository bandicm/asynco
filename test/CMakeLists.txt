# add_executable(asynco_test main.cpp)

# # Linkaj test sa Asynco bibliotekom
# target_link_libraries(asynco_test asynco Boost::system)

add_executable(asynco_default main_default.cpp)

# Linkaj test sa Asynco bibliotekom
target_link_libraries(asynco_default asynco Boost::system)

add_executable(asynco_asynco main_asynco.cpp)

# Linkaj test sa Asynco bibliotekom
target_link_libraries(asynco_asynco asynco Boost::system)