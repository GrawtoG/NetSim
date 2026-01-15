#include "gtest/gtest.h"

using std::cout,std::endl;
int main(int argc, char** argv) {
    cout<<"Teściki ten tego"<<endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
