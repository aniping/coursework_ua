# plotlinear.py

import numpy as np
import matplotlib.pyplot as plt

# Define two points for the x-axis
x = np.array([-5, 5])

# Define the different intercepts and gradients to plot
w0 = np.arange(0, 20)
w1 = np.arange(0, 8, 0.4)

# Plot all of the lines
plt.figure()
plt.plot()

for i in range(w0.shape[0]):
    plt.plot(x, w0[i] + w1[i]*x)
    print "\ny = " + str(w0[i]) + " + " + str(w1[i]) + " x"

print "\nClose the current plot window to continue"

plt.show()

# Request user input
fig = plt.figure()
plt.plot()
plt.ion()
print "\nThe following will ask you for intercept and slope values"
print "   (assuming floats) and will keep plotting lines on a new plot "
print "   until you enter 'save', which will save the plot as a pdf "
print "   called 'line_plot.pdf'."
print "(NOTE: you may see a MatplotlibDeprecationWarning -- you can safely ignore this)\n"
while True:
    intercept = raw_input("Enter intercept: ")
    if intercept == 'save':
        break
    else:
        intercept = float(intercept)

    gradient = raw_input("Enter gradient (slope): ")
    if gradient == 'save':
        break
    else:
        gradient = float(gradient)

    plt.plot(x, intercept + gradient*x)
    plt.show()
    plt.pause(.1)
    print "\ny = " + str(intercept) + " + " + str(gradient) + " x\n"

caption = '''
Line1: intercept: 1, slope: 2;
Line2: intercept: -3, slope: 2;
Line3: intercept: 2, slope: -3;
'''
plt.savefig('line_plot.pdf', format='pdf')
