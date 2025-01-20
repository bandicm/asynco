add_executable(asynco_test main.cpp)

# Linkaj test sa Asynco bibliotekom
target_link_libraries(asynco_test asynco Boost::system)
