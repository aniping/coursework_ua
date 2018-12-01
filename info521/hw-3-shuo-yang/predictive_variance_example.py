# Extension of predictive_variance_example.py
# Port of predictive_variance_example.m
# From A First Course in Machine Learning, Chapter 2.
# Simon Rogers, 01/11/11 [simon.rogers@glasgow.ac.uk]
# Predictive variance example

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

# sample 100 points from function
x, t = sample_from_function(100, noise_var, xmin, xmax)

# Chop out some x data:
xmin_remove = -2  # -0.5
xmax_remove = 2   # 2.5
# the following line expresses a boolean function over the values in x;
# this produces a list of the indices of list x for which the test
# was not met; these indices are then deleted from x and t.
pos = ((x >= xmin_remove) & (x <= xmax_remove)).nonzero()
x = np.delete(x, pos, 0)
t = np.delete(t, pos, 0)

# Plot just the sampled data
plt.figure(0)
plt.scatter(np.asarray(x), np.asarray(t), color='k', edgecolor='k')
plt.xlabel('x')
plt.ylabel('t')
plt.title('Sampled data from {0}, $x \in [{1},{2}]$'
          .format(true_function.__doc__, xmin, xmax))
plt.pause(.1)  # required on some systems so that rendering can happen
if SAVE_FIGURES:
    plt.savefig(figure_path + 'data')

# Fit models of various orders
orders = [1, 3, 5, 9]

# Make a set of 100 evenly-spaced x values between xmin and xmax
testx = np.linspace(xmin, xmax, 100)

# Generate plots of predicted variance (error bars) for various model orders
for i in orders:
    # create input representation for given model polynomial order
    X = np.zeros(shape=(x.shape[0], i + 1))
    testX = np.zeros(shape=(testx.shape[0], i + 1))
    for k in range(i + 1):
        X[:, k] = np.power(x, k)
        testX[:, k] = np.power(testx, k)
    N = X.shape[0]

    # fit model parameters
    w = np.dot(np.linalg.inv(np.dot(X.T, X)), np.dot(X.T, t))
    ss = (1./N)*(np.dot(t, t) - np.dot(t, np.dot(X, w)))

    # calculate predictions
    testmean = np.dot(testX, w)
    testvar = ss * np.diag(np.dot(testX, np.dot(np.linalg.inv(np.dot(X.T, X)), testX.T)))

    # Plot the data and predictions
    plt.figure()
    plt.scatter(x, t, color='k', edgecolor='k')
    plt.xlabel('x')
    plt.ylabel('t')
    plt.errorbar(testx, testmean, testvar)
    
    # find ylim plot bounds automagically...
    min_model = min(testmean - testvar)
    max_model = max(testmean + testvar)
    min_testvar = min(min(t), min_model)
    max_testvar = max(max(t), max_model)
    plt.ylim(min_testvar, max_testvar)
    
    ti = 'Plot of predicted variance for model with polynomial order {:g}'.format(i)
    plt.title(ti)
    plt.pause(.1)  # required on some systems so that rendering can happen

    if SAVE_FIGURES:
        filename = 'error-{0}'.format(i)
        plt.savefig(figure_path + filename)

# Generate plots of functions whose parameters are sampled based on cov(\hat{w})
num_function_samples = 20
for i in orders:
    # create input representation for given model polynomial order
    X = np.zeros(shape=(x.shape[0], i+1))
    testX = np.zeros(shape=(testx.shape[0], i+1))
    for k in range(i + 1):
        X[:, k] = np.power(x, k)
        testX[:, k] = np.power(testx, k)

    # fit model parameters
    w = np.dot(np.linalg.inv(np.dot(X.T, X)), np.dot(X.T, t))
    ss = (1./N) * (np.dot(t, t) - np.dot(t, np.dot(X, w)))
    
    # Sample functions with parameters w sampled from a Gaussian with
    # $\mu = \hat{\mathbf{w}}$
    # $\Sigma = \sigma^2(\mathbf{X}^T\mathbf{X})^{-1}$
    # determine cov(w)
    covw = ss * np.linalg.inv(np.dot(X.T, X))
    # The following samples num_function_samples of w from Gaussian based on covw
    wsamp = np.random.multivariate_normal(w, covw, num_function_samples)

    # Calculate means for each function
    testmean = np.dot(testX, wsamp.T)
    
    # Plot the data and functions
    plt.figure()
    plt.scatter(x, t, color='k', edgecolor='k')
    plt.xlabel('x')
    plt.ylabel('t')
    plt.plot(testx, testmean, color='b')
    
    # find reasonable ylim bounds
    plt.xlim(xmin_remove-2, xmax_remove+2)  # (-2,4) # (-3, 3)
    min_model = min(testmean.flatten())
    max_model = max(testmean.flatten())
    min_testvar = min(min(t), min_model)
    max_testvar = max(max(t), max_model)
    plt.ylim(min_testvar, max_testvar)  # (-400,400)
    
    ti = 'Plot of {0} functions where parameters '\
         .format(num_function_samples, i) + \
         r'$\widehat{\bf w}$ were sampled from' + '\n' + r'cov($\bf w$)' + \
         ' of model with polynomial order {1}' \
         .format(num_function_samples, i)
    plt.title(ti)
    plt.pause(.1)  # required on some systems so that rendering can happen
    
    if SAVE_FIGURES:
        filename = 'sampled-fns-{0}'.format(i)
        plt.savefig(figure_path + filename)



plt.show()  # Holds the plots open until you close them
