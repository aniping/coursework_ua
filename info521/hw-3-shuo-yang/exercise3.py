# Extension of predictive_variance_example.py
# Port of predictive_variance_example.m
# From A First Course in Machine Learning, Chapter 2.
# Simon Rogers, 01/11/11 [simon.rogers@glasgow.ac.uk]
# Predictive variance example

# Solution for the exercise 3.

import numpy as np
import matplotlib.pyplot as plt


# set to True in order to automatically save the generated plots
# SAVE_FIGURES = False
SAVE_FIGURES = True
# change this to where you'd like the figures saved
# (relative to your python current working directory)
figure_path = './figs/'

'''
def true_function_2013(x):
    """$t = 5x^3-x^2+x$"""
    return 5*x**3 - x**2 + x
'''

'''
def true_function_2014(x):
    """$t = 1 + 0.1x + 0.5x^2 + 0.05x^3$"""
    return 1 + (0.1 * x) + (0.5 * numpy.power(x, 2)) + (0.05 * numpy.power(x, 3))
'''


def true_function(x):
    """$t = x + 0.5x^2 + 0.1x^3$"""
    return x + (0.5 * np.power(x, 2)) + (0.1 * np.power(x, 3))


def sample_from_function(N=100, noise_var=1000, xmin=-5., xmax=5.):
    """ Sample data from the true function.
        N: Number of samples
        Returns a noisy sample t_sample from the function
        and the true function t. """
    x = np.random.uniform(xmin, xmax, N)
    t = true_function(x)
    # add standard normal noise using numpy.random.randn
    # (standard normal is a Gaussian N(0, 1.0)  (i.e., mean 0, variance 1),
    #  so multiplying by numpy.sqrt(noise_var) make it N(0,standard_deviation))
    t = t + np.random.randn(x.shape[0])*np.sqrt(noise_var)
    return x, t

xmin = -8.
xmax = 5.
noise_var = 6

num_datasets = 20

# Fit models of various orders
orders = [1, 3, 5, 9]

# Make a set of 100 evenly-spaced x values between xmin and xmax
testx = np.linspace(xmin, xmax, 100)

# Generate plots of predicted variance (error bars) for various model orders
for i in orders:
    testmean = np.zeros((testx.shape[0], num_datasets))
    for j in range(num_datasets):
        # sample 25 points from function
        x, t = sample_from_function(25, noise_var, xmin, xmax)

        # create input representation for given model polynomial order
        X = np.zeros(shape=(x.shape[0], i + 1))
        testX = np.zeros(shape=(testx.shape[0], i + 1))
        for k in range(i + 1):
            X[:, k] = np.power(x, k)
            testX[:, k] = np.power(testx, k)
        N = X.shape[0]

        # fit model parameters
        w = np.dot(np.linalg.inv(np.dot(X.T, X)), np.dot(X.T, t))

        # calculate predictions
        testmean[:, j] = np.dot(testX, w)

    # Plot the data and predictions
    plt.figure()
    # plt.scatter(x, t, color='k', edgecolor='k')
    plt.xlabel('x')
    plt.ylabel('t')
    plt.plot(testx, testmean, color='b', linewidth=1)
    plt.plot(testx, true_function(testx), color='r', linewidth=3)
    plt.ylim(-50, 50)

    ti = 'Plot of order {} best fit model for each of the 20 datasets'.format(i)
    plt.title(ti)
    plt.pause(.1)  # required on some systems so that rendering can happen

    if SAVE_FIGURES:
        filename = 'exer3-order-{0}'.format(i)
        plt.savefig(figure_path + filename)

plt.show()  # Holds the plots open until you close them
