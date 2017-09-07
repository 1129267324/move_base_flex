/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: Eitan Marder-Eppstein
*********************************************************************/
#ifndef MBF_CORE_BASE_GLOBAL_PLANNER_H
#define MBF_CORE_BASE_GLOBAL_PLANNER_H

#include <geometry_msgs/PoseStamped.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <nav_core/base_global_planner.h>
#include "move_base_flex_core/abstract_global_planner.h"

namespace move_base_flex_core {
  /**
   * @class BaseGlobalPlanner
   * @brief Provides an interface for global planners used in navigation.
   * All global planners written to work as MBF plugins must adhere to this interface. Alternatively, this
   * class can also operate as a wrapper for old API nav_core-based plugins, providing backward compatibility.
   */
  class BaseGlobalPlanner : public AbstractGlobalPlanner{
    public:

      typedef boost::shared_ptr< ::move_base_flex_core::BaseGlobalPlanner > Ptr;


      /**
       * @brief Given a goal pose in the world, compute a plan
       * @remark New on MBF API; replaces version without output code and message
       * @param start The start pose
       * @param goal The goal pose
       * @param tolerance The tolerance to the goal pose
       * @param plan The plan... filled by the planner
       * @param cost The cost for the the plan
       * @param plugin_code More detailed outcome; will be defaulted to DO_NOT_APPLY on planners
       * implementing the old move_base API
       * @param plugin_msg More detailed outcome as a string message
       * @return True if a valid plan was found, false otherwise
       */
      virtual bool makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal,
                            double tolerance, std::vector<geometry_msgs::PoseStamped>& plan, double& cost,
                            uint8_t& plugin_code, std::string& plugin_msg)
      {
        if (!backward_compatible_plugin)
          throw std::runtime_error("MBF API makePlan method not overridden nor backward compatible plugin provided");

        plugin_code = 255;  // DO_NOT_APPLY
        return backward_compatible_plugin->makePlan(start, goal, plan, cost);
      }

      /**
       * @brief Requests the planner to cancel, e.g. if it takes to much time.
       * @remark New on MBF API
       * @return True if a cancel has been successfully requested, false if not implemented.
       */
      virtual bool cancel()
      {
        return false;
      }

      /**
       * @brief Initialization function for the BaseGlobalPlanner
       * @param name The name of this planner
       * @param costmap_ros A pointer to the ROS wrapper of the costmap to use for planning
       */
      virtual void initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros)
      {
        if (!backward_compatible_plugin)
          throw std::runtime_error("MBF API initialize method not overridden nor backward compatible plugin provided");

        backward_compatible_plugin->initialize(name, costmap_ros);
      }

      /**
       * @brief Public constructor used for handling a nav_core-based plugin
       * @param plugin Backward compatible plugin
       */
      BaseGlobalPlanner(boost::shared_ptr< nav_core::BaseGlobalPlanner > plugin)
      {
        backward_compatible_plugin = plugin;

        if (!backward_compatible_plugin)
          throw std::runtime_error("Invalid backward compatible plugin provided");
      }

      /**
       * @brief  Virtual destructor for the interface
       */
      virtual ~BaseGlobalPlanner(){}

    protected:
      BaseGlobalPlanner(){}

    private:
      boost::shared_ptr< nav_core::BaseGlobalPlanner > backward_compatible_plugin;
  };
};  // namespace move_base_flex_core

#endif  // MBF_CORE_BASE_GLOBAL_PLANNER_H
