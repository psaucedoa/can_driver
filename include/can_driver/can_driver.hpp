/*
 * Copyright 2024 Construction Engineering Research Laboratory (CERL)
 * Engineer Reseach and Development Center (ERDC)
 * U.S. Army Corps of Engineers
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CAN_DRIVER__CAN_DRIVER_HPP_
#define CAN_DRIVER__CAN_DRIVER_HPP_

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <thread>
#include <iostream>

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_msgs/msg/frame.hpp"
#include "can_driver/visibility_control.hpp"

// See https://docs.kernel.org/networking/can.html

namespace drivers
{
namespace can
{ 

class CanDriver
{
public:
  explicit CanDriver();
  ~CanDriver();


  int test = 1;

  int32_t s_;
  struct ifreq ifr_;
  struct sockaddr_can addr_;

  void setupConnection(const char * interface_name);
  void closeConnection();
  void setFilter(const struct can_filter filter);
  can_frame receive();
  void transmit(const struct can_frame & frame);

private:

};  // End class CanDriver

}  // End namespace can
}  // End namespace drivers

#endif  // CAN_DRIVER_J1939__CAN_DRIVER_J1939_HPP_