// -*-c++-*--------------------------------------------------------------------
// Copyright 2022 Bernd Pfrommer <bernd.pfrommer@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef FREQUENCY_CAM__FREQUENCY_CAM_ROS1_H_
#define FREQUENCY_CAM__FREQUENCY_CAM_ROS1_H_

#include <event_array_codecs/decoder_factory.h>
#include <event_array_msgs/EventArray.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Header.h>

#include "frequency_cam/frequency_cam.h"
#include "frequency_cam/image_maker.h"

namespace frequency_cam
{
class FrequencyCamROS
{
public:
  using EventArray = event_array_msgs::EventArray;

  FrequencyCamROS(const ros::NodeHandle & nh);
  FrequencyCamROS(const FrequencyCamROS &) = delete;
  FrequencyCamROS & operator=(const FrequencyCamROS &) = delete;

private:
  void frameTimerExpired(const ros::TimerEvent &);
  bool initialize();
  void imageConnectCallback(const image_transport::SingleSubscriberPublisher &);
  void eventMsg(const EventArray::ConstPtr & msg);
  void statisticsTimerExpired(const ros::TimerEvent &);

  // ------ variables ----
  ros::NodeHandle nh_;
  ros::Time lastPubTime_{0};
  ros::Subscriber eventSub_;  // subscribes to events
  ros::Timer frameTimer_;     // fires once per frame
  ros::Timer statsTimer_;     // for statistics printout
  std_msgs::Header header_;   // header with frame id etc
  uint32_t seq_{0};           // ROS1 header seqno
  bool isSubscribedToEvents_{false};
  uint32_t width_{0};          // image width
  uint32_t height_{0};         // image height
  double eventImageDt_{0.01};  // time between published images
  bool overlayEvents_{false};
  bool useLogFrequency_{false};
  image_transport::Publisher imagePub_;
  event_array_codecs::DecoderFactory<FrequencyCam> decoderFactory_;
  sensor_msgs::Image imageMsgTemplate_;
  FrequencyCam cam_;
  ImageMaker imageMaker_;
  // --- statistics
  uint64_t totTime_{0};
  uint64_t msgCount_{0};
  int64_t lastSeq_{0};
  int64_t droppedSeq_{0};
  // --- debugging
  uint16_t debugX_;
  uint16_t debugY_;
};
}  // namespace frequency_cam
#endif  // FREQUENCY_CAM__FREQUENCY_CAM_ROS1_H_
