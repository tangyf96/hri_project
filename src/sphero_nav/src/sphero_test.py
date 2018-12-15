#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32
import math
def sphero():
    vel_pub = rospy.Publisher('/cmd_vel', Twist, queue_size = 1)
    #angu_pub = rospy.Publisher('/set_angular_velocity', Float32, queue_size = 1)
    rospy.init_node('sphero_test', anonymous = True)
    rate = rospy.Rate(5)
    vel = Twist()
    #angular_vel = Float32()
    theta = 0
    vel.linear.x = 2 * math.cos(theta)
    vel.linear.y = 2 * math.sin(theta)
    # angular_vel.data = 10
    step = 0
    while not rospy.is_shutdown():
        vel_pub.publish(vel)
        theta += 10.
        vel.linear.x = 70 * math.cos(theta/180 * math.pi)
        vel.linear.y = 70 * math.sin(theta/180 * math.pi)
        print(theta/180*math.pi)
        print(vel.linear.x, vel.linear.y)
        rate.sleep()
        #angu_pub.publish(angular_vel)
        #rate.sleep()
        #print('set angular velocity')
        step += 1
        #angular_vel.data = step * 100
        if step == 1000:
            break
    print('out of loop')    

if __name__ == '__main__':
    try:
        sphero()
    except rospy.ROSInterruptException:
        print('error')
        pass

