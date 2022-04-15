#!/usr/bin/env python

import argparse 


parser = argparse.ArgumentParser(description='euler.py')
parser.add_argument('-v', '--vinit', default=10, type=float,
                    help="-v vinit: initial speed [default 10 m/s]\n")
parser.add_argument('-a', '--theta0', default=45, type=float,
                    help="-a theta0: initial angle above horizontal [45deg]\n")
parser.add_argument('-t', '--dt', default=0.01, type=float,
                    help="-t step: time step for approximation [0.01s]\n")
args = parser.parse_args()

vinit=args.vinit
theta0=args.theta0
dt=args.dt

print("Simulating projectile motion with params:")
print("(vinit,theta0,dt)=(%7.2lf,%7.2f,%7.2f)" % (vinit,theta0,dt))


# fill in the blanks below

# given the boundary conditions at t=0, namely:
# x(0) = y(0) = 0
# vx(0) = vinit * cos(theta0) , vy(0) = vinit * sin(theta0)
# impliment Euler's method for stepping forward in time to calculate the
# new position of the projectile in steps of dt
# your simulation should stop at the last time step before y goes negative

  
# eg, write out a data file containing at least the following information
# t   x(t)   y(t)   vx(t)   vy(t)

