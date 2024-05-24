import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # Find the package share directory
    ros2_sim_description_share_dir = get_package_share_directory('ros2_sim_description')

    # Define the path to the URDF file
    urdf_path = os.path.join(ros2_sim_description_share_dir, '../urdf', 'rrr.urdf')

    # Create the launch description
    ld = LaunchDescription()

    # Robot state publisher node
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': urdf_path}]
    )

    # TF broadcaster node
    '''
    tf_broadcaster_node = Node(
        package='ros2_sim_tf',
        executable='tf_broadcaster',
        name='tf_broadcaster',
        output='screen',
        parameters=[{'new_angle': 1.5}]
    )
    '''

    # Joint state publisher GUI node
    joint_state_publisher_gui_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        output='screen'
    )

    # RViz2 node
    rviz2_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen'
    )

    # Add nodes to the launch description
    ld.add_action(robot_state_publisher_node)
    #ld.add_action(tf_broadcaster_node)
    ld.add_action(joint_state_publisher_gui_node)
    ld.add_action(rviz2_node)

    return ld
