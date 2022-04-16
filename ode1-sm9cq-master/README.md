# ode1: Eueler's method

In this exercise you will make use of Eueler's method to solve ODEs related to classical motion. The initial goal of the project is to numerically calculate the trajectory for a projectile starting near the surface of the earth.

Very basic started code is provided in C++ and Python. You can use either language to perform your study. But after you get your code running, verify that you can produce the same results with both languages. 

Examine the program euler.cpp or euler.py. The program provides a simple shell for you to use as the basis of your code. For starters the program is only set up to take parameters from the command line that can be used to set the initial conditions for your simulation of the projectile motion.

**The easy case: no air resistance**

Complete the program to use Euler's method to solve for the trajectory of the projectile based on the initial conditions entered into your program. This version of the problem is not very complicated and you can also calculate the analytic solution for comparison.

Study and assess the performance of your algorithm, you can do this in a variety of ways, for example:

* Observe the calculated landing location versus the time step (similarly the error wrt the analytic solution).
* Can you find a time step where the landing location is a “pretty good” match to the expected position?
* Is this observation stable with changes to the initial conditions?
* Plot the difference in x and y position (or velocity) versus time compared to the analytic solution
* Look at components of velocity/acceleration
* Plot the energy of the projectile versus time
* …

Upload/push a short discussion in a file called projectiles.pdf before our next class meeting. You should be able to complete the basic projectile simulation and make a plot or two before the end of class. Feel free to discuss with others and trade ideas. You don't need to perform an extensive study, choose a few of your own “interesting” questions to study and summarize your results. 
