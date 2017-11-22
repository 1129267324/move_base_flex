/*
 *  Copyright 2017, Magazino GmbH, Sebastian Pütz, Jorge Santos Simón
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  wrapper_local_planner.cpp
 *
 *  authors:
 *    Sebastian Pütz <spuetz@uni-osnabrueck.de>
 *    Jorge Santos Simón <santos@magazino.eu>
 *
 */

#ifndef MOVE_BASE_FLEX__WRAPPER_LOCAL_PLANNER_H_
#define MOVE_BASE_FLEX__WRAPPER_LOCAL_PLANNER_H_

#include <nav_core/base_local_planner.h>
#include "move_base_flex_core/move_base_controller.h"

namespace move_base_flex_core {
  /**
   * @class LocalPlanner
   * @brief Provides an interface for local planners used in navigation.
   * All local planners written to work as MBF plugins must adhere to this interface. Alternatively, this
   * class can also operate as a wrapper for old API nav_core-based plugins, providing backward compatibility.
   */
  class WrapperLocalPlanner : public MoveBaseController{
    public:

      /**
       * @brief Given the current position, orientation, and velocity of the robot,
       * compute velocity commands to send to the base
       * @remark New on MBF API; replaces version without output code and message
       * @param cmd_vel Will be filled with the velocity command to be passed to the robot base
       * @param message Optional more detailed outcome as a string
       * @return Result code as described on ExePath action result, as this is a wrapper to the nav_core,
       *         only 0 (SUCCESS) and 100 (FAILURE) are supported.
       */
      virtual uint32_t mbfComputeVelocity(geometry_msgs::TwistStamped& cmd_vel, std::string& message);

      /**
       * @brief Check if the goal pose has been achieved by the local planner
       * @return True if achieved, false otherwise
       */
      virtual bool mbfIsGoalReached();

      /**
       * @brief Check if the goal pose has been achieved by the local planner within tolerance limits
       * @remark New on MBF API
       * @param xy_tolerance Distance tolerance in meters
       * @param yaw_tolerance Heading tolerance in radians
       * @return True if achieved, false otherwise
       */
      virtual bool mbfIsGoalReached(double xy_tolerance, double yaw_tolerance);

      /**
       * @brief  Set the plan that the local planner is following
       * @param plan The plan to pass to the local planner
       * @return True if the plan was updated successfully, false otherwise
       */
      virtual bool mbfSetPath(const std::vector<geometry_msgs::PoseStamped>& plan);

      /**
       * @brief Requests the planner to cancel, e.g. if it takes to much time
       * @remark New on MBF API
       * @return True if a cancel has been successfully requested, false if not implemented.
       */
      virtual bool mbfCancel();

      /**
       * @brief Constructs the local planner
       * @param name The name to give this instance of the local planner
       * @param tf A pointer to a transform listener
       * @param costmap_ros The cost map to use for assigning costs to local plans
       */
      virtual void mbfInitialize(std::string name, tf::TransformListener* tf, costmap_2d::Costmap2DROS* costmap_ros);

      /**
       * @brief Public constructor used for handling a nav_core-based plugin
       * @param plugin Backward compatible plugin
       */
      WrapperLocalPlanner(boost::shared_ptr< nav_core::BaseLocalPlanner >plugin);

      /**
       * @brief  Virtual destructor for the interface
       */
      virtual ~WrapperLocalPlanner();

    private:
      boost::shared_ptr< nav_core::BaseLocalPlanner > nav_core_plugin_;
  };
}  /* namespace move_base_flex_core */

#endif  /* MOVE_BASE_FLEX__WRAPPER_LOCAL_PLANNER_H_ */
