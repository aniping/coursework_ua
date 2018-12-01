# approx_expected_value_sin.py
# Port of approx_expected_value.m
# From A First Course in Machine Learning, Chapter 2.
# Simon Rogers, 01/11/11 [simon.rogers@glasgow.ac.uk]
# Approximating expected values via sampling
import numpy as np
import matplotlib.pyplot as plt


# We are trying to estimate the expected value of
# $f(x) = 60 + 0.1x + 0.5x^3 + 0.05x^4$
##
# ... where
# $p(x)=U(-10,5)$
##
# ... which is given by:
# $\int y^2 p(y) dy$
##
# The analytic result is:
# 50.375
# (NOTE: this just gives you the analytic result -- you should be abld to derive it!)

def fx(xs):
    return 60 + 0.1 * xs + 0.5 * np.power(xs,3) + 0.05 * np.power(xs,4)

# Sample 5000 uniformly random values in [-10..5]
xs = np.random.uniform(low=-10.0, high=5.0, size=5000)
# compute the expectation of y, where y is the function that squares its input
# print(fx(xs))
ex2 = np.mean(fx(xs))
print '\nSample-based approximation: {:f}'.format(ex2)

# Store the evolution of the approximation, every 100 samples
sample_sizes = np.arange(1, xs.shape[0], 100)
ex2_evol = np.zeros((sample_sizes.shape[0]))  # storage for the evolving estimate...
# the following computes the mean of the sequence up to i, as i iterates
# through the sequence, storing the mean in ex2_evol:
for i in range(sample_sizes.shape[0]):
    ex2_evol[i] = np.mean(fx(xs[0:sample_sizes[i]]))

# Create plot of evolution of the approximation
plt.figure()
# plot the curve of the estimation of the expected value of f(x)=y^2
plt.plot(sample_sizes, ex2_evol)
# The true, analytic result of the expected value of
# f(x) = 60 + 0.1x + 0.5x^3 + 0.05x^4 where x ~ U(-10,5): $\frac{1}{3}$
# plot the analytic expected result as a red line:
plt.plot(np.array([sample_sizes[0], sample_sizes[-1]]), np.array([50.375, 50.375]), color='r')
plt.xlabel('Sample size')
plt.ylabel('Approximation of expectation')
plt.title('Approximation of expectation of $f(x) = 60 + 0.1x + 0.5x^3 + 0.05x^4$')
plt.pause(.1)  # required on some sxstems so that rendering can happen

plt.savefig('figs/approx_exp.png', fmt='png') # save the plot
plt.show()  # keeps the plot open
