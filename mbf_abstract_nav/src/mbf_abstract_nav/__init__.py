# Generic set of parameters required by any MBF-based navigation framework
# To use:
#
#  from mbf_abstract_nav import add_mbf_abstract_nav_params
#  gen = ParameterGenerator()
#  add_mbf_abstract_nav_params(gen)
#  ...

# need this only for dataype declarations
from dynamic_reconfigure.parameter_generator_catkin import str_t, double_t, int_t, bool_t


def add_mbf_abstract_nav_params(gen):
    gen.add("global_planner", str_t, 0,
            "The name of the plugin for the global planner to use with move_base_flex.", "navfn/NavfnROS")
    gen.add("local_planner", str_t, 0,
            "The name of the plugin for the local planner to use with move_base_flex.", "base_local_planner/TrajectoryPlannerROS")
    
    # Disabled on move_base... TODO: try to enable in mbf if turns to be useful
    # gen.add("recovery_behaviors", str_t, 0, "A list of recovery behavior plugins to use with move_base_flex.", "[{name: conservative_reset, type: clear_costmap_recovery/ClearCostmapRecovery}, {name: rotate_recovery, type: rotate_recovery/RotateRecovery}, {name: aggressive_reset, type: clear_costmap_recovery/ClearCostmapRecovery}]")
    
    gen.add("planner_frequency", double_t, 0,
            "The rate in Hz at which to run the planning loop.", 0, 0, 100)
    gen.add("planner_patience", double_t, 0,
            "How long the planner will wait in seconds in an attempt to find a valid plan before giving up.", 5.0, 0, 100)
    gen.add("planner_max_retries", int_t, 0,
            "How many times we will recall the planner in an attempt to find a valid plan before giving up", -1, -1, 1000)
    gen.add("controller_frequency", double_t, 0,
            "The rate in Hz at which to run the control loop and send velocity commands to the base.", 20, 0, 100)
    gen.add("controller_patience", double_t, 0,
            "How long the controller will wait in seconds without receiving a valid control before giving up.", 5.0, 0, 100)
    gen.add("controller_max_retries", int_t, 0,
            "How many times we will recall the controller in an attempt to find a valid comand before giving up", -1, -1, 1000)
    
    gen.add("recovery_enabled", bool_t, 0,
            "Whether or not to enable the move_base_flex recovery behaviors to attempt to clear out space.", True)
    gen.add("oscillation_timeout", double_t, 0,
            "How long in seconds to allow for oscillation before executing recovery behaviors.", 0.0, 0, 60)
    gen.add("oscillation_distance", double_t, 0,
            "How far in meters the robot must move to be considered not to be oscillating.", 0.5, 0, 10)
