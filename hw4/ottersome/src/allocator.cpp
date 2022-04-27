#include "allocator.h"

ByteCounter* ByteCounter::instance = nullptr;

ByteCounter* ByteCounter::getInstance(){
    if(instance == nullptr){
        instance = new ByteCounter();
    }
    return instance;
}
