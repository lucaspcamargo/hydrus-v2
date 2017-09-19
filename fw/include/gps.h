#pragma once

#include "hydrus-config.h"

__HYDRUS_BEGIN

class GPS
{
public:

  typedef Traits< GPS > Tr;
  
  typedef Tr::ANGLE_DATATYPE Angle;
  
  struct Message {
    Angle lat;
    Angle lon;
    uint32_t unixtime;
    bool fix;
  };
  
  static void init();

  bool has_message();
  const Message get_message_copy();
  
  const Message & lock_message();
  void unlock_message();
  
private:
  static bool tick();
  
};

__HYDRUS_END
