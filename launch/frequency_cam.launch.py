# -----------------------------------------------------------------------------
# Copyright 2021 Bernd Pfrommer <bernd.pfrommer@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

import launch
from launch.actions import DeclareLaunchArgument as LaunchArg
from launch.actions import OpaqueFunction
from launch.substitutions import LaunchConfiguration as LaunchConfig
from launch_ros.actions import Node


def launch_setup(context, *args, **kwargs):
    """Set up launch configuration."""
    image_topic_config = LaunchConfig('image_topic')
    event_topic_config = LaunchConfig('event_topic')
    image_topic = image_topic_config.perform(context)
    event_topic = event_topic_config.perform(context)
    node = Node(
        package='frequency_cam',
        executable='frequency_cam_node',
        output='screen',
        # prefix=['valgrind --tool=callgrind --dump-instr=yes
        #  --simulate-cache=yes --collect-jumps=yes'],
        # prefix=['xterm -e gdb -ex run --args'],
        name='frequency_cam',
        parameters=[
            {
                'use_sim_time': LaunchConfig('use_sim_time'),
                'min_frequency': 10.0,
                'max_frequency': 5000.0,
                'cutoff_period': 5.0,  # prefilter cutoff period #events
                'debug_x': 240,
                'debug_y': 194,
                'use_log_frequency': False,
                'overlay_events': True,
                'bag_file': LaunchConfig('bag').perform(context),
                'frame_time_file': LaunchConfig('frame_time_file').perform(context),
                'publishing_frequency': 25.0,
            }
        ],
        remappings=[('~/events', event_topic), ('~/image', image_topic)],
    )
    return [node]


def generate_launch_description():
    """Create slicer node by calling opaque function."""
    return launch.LaunchDescription(
        [
            LaunchArg(
                'image_topic', default_value=['/event_camera/image'], description='image topic'
            ),
            LaunchArg(
                'event_topic', default_value=['/event_camera/events'], description='event topic'
            ),
            LaunchArg(
                'frame_time_file',
                default_value=[''],
                description='file with frame times (sensor time in nanosec)',
            ),
            LaunchArg('bag', default_value=[''], description='name of bag file to read'),
            LaunchArg(
                'use_sim_time',
                default_value=['false'],
                description='whether to use simulation time',
            ),
            OpaqueFunction(function=launch_setup),
        ]
    )
