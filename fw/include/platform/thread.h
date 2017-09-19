#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

class Thread {

  public:
      
    enum Type {
        NORMAL,
        FIFO,
        LOWPRIO
    };

    Thread( Type t = NORMAL);

    ~Thread();

    virtual int main() = 0;

    void join();
    
    void entry();
    
  private:
    void *_p;

};

template <typename T> class FunctionThread : public Thread {
public:
    
    virtual int main() override {
        return T::threadEntry();
    };
};

__HYDRUS_PLATFORM_END
