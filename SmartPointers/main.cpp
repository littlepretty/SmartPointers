//
//  main.cpp
//  SmartPointers
//
//  Created by 闫 佳琪 on 3/12/13.
//  Copyright (c) 2013 littlepretty. All rights reserved.
//

#include <iostream>
#include "Pointers.h"


using namespace std;

void testSuperSmartPointer()
{
    Nothing* pNothing = new Nothing();
    
    {
        SuperSmartPointer<Nothing> ptr1(pNothing);
        SuperSmartPointer<Nothing> ptr2(pNothing);
    }
    
    if (Nothing::sNumAllocations != Nothing::sNumDeletions) {
        std::cout << "TEST FAILED: " << Nothing::sNumAllocations<<" allocations and " << Nothing::sNumDeletions << " deletions" << std::endl;
    }else{
        std::cout << "TEST PASSED" << std::endl;
    }
}

int main(int argc, const char* argv[])
{
    testSuperSmartPointer();
}