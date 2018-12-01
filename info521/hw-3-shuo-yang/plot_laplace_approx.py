# Solution for the problem 7

from scipy.stats import norm
from scipy.stats import beta
import numpy as np
import math
import matplotlib.pyplot as plt

num_plots = 1

def plot_laplace_approx(alpha, beta_, N, y):
    """
    Plot both the true beta posterior and the Laplace
    approximation with the given alpha, beta, N and y values.
    """
    global num_plots
    # calculate the MAP value
    u = (y + alpha - 1.) / (N + beta_ + alpha - 2.)
    # calculate the estimated variance
    var = (u*u * (u-1.)*(u-1.)) / (u*u*(N-y+beta_-1) + (u-1.)*(u-1.)*(y+alpha-1))

    print "estimated mean: ", u
    print "estimated variance", var
    x = np.arange(0, 1, 0.01)
    plt.figure()
    plt.plot(x, beta.pdf(x, alpha + y, beta_ + N - y), 'r', label='true beta posterior')
    plt.plot(x, norm.pdf(x, loc=u, scale=math.sqrt(var)), 'b', label='Laplace approximation')
    plt.legend(fontsize=10)
    plt.savefig('./figs/laplace-{}'.format(num_plots), fmt='png')
    num_plots += 1

plot_laplace_approx(5, 5, 20, 10)
plot_laplace_approx(3, 15, 10, 3)
plot_laplace_approx(1, 30, 10, 3)
plt.show()
