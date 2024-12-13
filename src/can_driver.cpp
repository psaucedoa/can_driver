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

#include "can_driver/can_driver.hpp"

namespace drivers
{
namespace can
{

CanDriver::CanDriver()
{}

CanDriver::~CanDriver()
{}

// BEGIN CAN SOCKET MANAGEMENT FUNCTIONS //


void CanDriver::setupConnection(const char * interface_name)
{
  // create socket
  std::cout << "Creating Socket" << '\n';
  s_ = socket(PF_CAN, static_cast<int32_t>(SOCK_RAW), CAN_RAW);

  std::cout << "Created Socket = " << s_ << '\n';
  // find if socket was generated?
  if (s_ < 0)
  {
    //ERROR "Socket could not be created";
    throw std::runtime_error{"Socket could not be created"};
  }

  // memset(&addr, 0, sizeof(addr));

  std::cout << "Interface name gang: " << interface_name << '\n';

  // get the interface name
  strcpy(ifr_.ifr_name, interface_name);
  ioctl(s_, SIOGIFINDEX, &ifr_);

  addr_.can_family = AF_CAN;
  addr_.can_ifindex = ifr_.ifr_ifindex;

  // // bind socket to CAN interface
  bind(s_, (struct sockaddr *)&addr_, sizeof(addr_));

  ioctl(s_, SIOCGIFNAME, &ifr_);
  std::cout << "Received a CAN frame from interface " << ifr_.ifr_name << '\n';
  // see if socket was bound
  if (bind(s_, (struct sockaddr *)&addr_, sizeof(addr_)) < 0)
  {
    //ERROR "Error binding to socket";
    throw std::runtime_error{"Error binding to socket"};
  }
}

void CanDriver::closeConnection()
{
  close(s_);
}

void CanDriver::setFilter(const struct can_filter filter)
{
  setsockopt(s_, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
}

can_frame CanDriver::receive()
{
  struct can_frame frame_out;
  can_msgs::msg::Frame frame;

  const auto nbytes = read(s_, &frame_out, sizeof(frame_out));
  
  if (nbytes < 0)
  {
    throw std::runtime_error{"Can socket read failed?"};
  }

  if (nbytes < sizeof(struct can_frame))
  {
    throw std::runtime_error{"Can socket read incomplete: Incomplete CAN frame?"};
  }

  const auto data_length = static_cast<uint64_t>(frame_out.can_dlc);
  (void)std::memcpy(frame.data.data(), static_cast<void *>(&frame_out.data[0U]), data_length);

  return frame_out;
}

void CanDriver::transmit(const struct can_frame & frame)
{
  const auto nbytes = write(s_, &frame, sizeof(struct can_frame));
}

// END CAN SOCKET MANAGEMENT FUNCTIONS //

}  // End namespace can
}  // End namespace drivers