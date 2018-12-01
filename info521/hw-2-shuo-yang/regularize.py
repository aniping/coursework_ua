# Script for ISTA 421 / INFO 521 Fall 2016, HW 2, Problem 4
# Author: Clayton T. Morrison, 13 September 2015
# Based on cv_demo.m
# From A First Course in Machine Learning, Chapter 1.
# Simon Rogers, 31/10/11 [simon.rogers@glasgow.ac.uk]

# NOTE: In its released form, this script will NOT run
#       You will get a syntax error on line 79 because w has not been defined

# NOTE:
# When summarizing log errors, DO NOT take the mean of the log
# instead, first take the mean of the errors, then take the log of the mean

import numpy
import matplotlib.pyplot as plt


# -------------------------------------------------------------------------
# Utilities

def fitpoly(X, t, model_order, lam_value, N):
    """
    Given "training" data in input feature sequence x and
    corresponding target value sequence t, and a specified
    polynomial of order model_order, determine the linear
    least mean squared (LMS) error best fit for parameters w,
    using the generalized matrix normal equation.
    model_order is a non-negative integer, n, representing the
    highest polynomial order term of the polynomial model:
        t = w0*x^0 + w1*x^1 + w2*x^2 + ... wn*x^n
    :param X: design matrix
    :param t: sequence of target response values
    :param lam_value: lambda value
    :param model_order: integer representing the maximum order of the polynomial model
    :param N: size of data set
    :return: parameter vector w
    """

    # Compute pararmeter vector w using the formula:
    # w = (X^TX)^-1X^Tt
    w = None  # Calculate w vector (as an numpy.array)
    XtX = numpy.dot(X.T, X) # calculate X^TX
    NlambdaI = N * lam_value * numpy.identity(model_order+1)
    XtX = XtX + NlambdaI
    XtX_inv = numpy.linalg.inv(XtX) # inverse of X^TX
    # XtX_inv = numpy.linalg.inv(XtX + NlambdaI) # inverse of X^TX
    Xtt = numpy.dot(X.T, t) # calculate X^Tt
    w = numpy.dot(XtX_inv, Xtt)

    return w


# The following permutation tools are used below in run_cv() as a convenience
# for randomly sorting the order of the data (the indices of the input data)

def random_permutation_matrix(n):
    """
    Generate a permutation matrix: an NxN matrix in which each row
    and each column has only one 1, with 0's everywhere else.
    See: https://en.wikipedia.org/wiki/Permutation_matrix
    :param n: size of the square permutation matrix
    :return: NxN permutation matrix
    """
    rows = numpy.random.permutation(n)
    cols = numpy.random.permutation(n)
    m = numpy.zeros((n, n))
    for r, c in zip(rows, cols):
        m[r][c] = 1
    return m


def permute_rows(X, P=None):
    """
    Permute the rows of a 2-d array (matrix) according to
    permutation matrix P.
    If no P is provided, a random permutation matrix is generated.
    :param X: 2-d array
    :param P: Optional permutation matrix; default=None
    :return: new version of X with rows permuted according to P
    """
    if P is None:
        P = random_permutation_matrix(X.shape[0])
    return numpy.dot(P, X)


def permute_cols(X, P=None):
    """
    Permute the columns of a 2-d array (matrix) according to
    permutation matrix P.
    If no P is provided, a random permutation matrix is generated.
    :param X: 2-d array
    :param P: Optional permutation matrix; default=None
    :return: new version of X with columns permuted according to P
    """
    if P is None:
        P = random_permutation_matrix(X.shape[0])
    return numpy.dot(X, P)


def test_permutation_math():
    # permutation matrix
    # will permute rows from 0, 1, 2 to 1, 2, 0
    # will permute cols from 0, 1, 2 to 2, 0, 1
    P = numpy.array([[0, 1, 0],
                     [0, 0, 1],
                     [1, 0, 0]])
    # original matrix
    m = numpy.array([[1, 2, 3],
                     [4, 5, 6],
                     [7, 8, 9]])
    # rows permuted to 1, 2, 0 by left-multiplying P x m
    mr = numpy.array([[4, 5, 6],
                      [7, 8, 9],
                      [1, 2, 3]])
    # cols permuted to 2, 0, 1 by right-multiplying m x P
    mc = numpy.array([[3, 1, 2],
                      [6, 4, 5],
                      [9, 7, 8]])

    # left-multiply by P to permute rows
    mrowp = numpy.dot(P, m)
    assert numpy.array_equal(mr, mrowp)

    # right-multiply by P to permute columns
    mcolp = numpy.dot(m, P)
    assert numpy.array_equal(mc, mcolp)

    print 'TEST permutation_math PASSED'

#test_permutation_math()


# -------------------------------------------------------------------------
# Utilities from fitpoly

def read_data(filepath, d=','):
    """ returns an numpy.array of the data """
    return numpy.genfromtxt(filepath, delimiter=d, dtype=None)


def plot_data(x, t):
    """
    Plot single input feature x data with corresponding response
    values t as a scatter plot
    :param x: sequence of 1-dimensional input data features
    :param t: sequence of 1-dimensional responses
    :return: None
    """
    plt.figure()  # Create a new figure object for plotting
    plt.scatter(x, t, edgecolor='b', color='w', marker='o')
    plt.xlabel('x')
    plt.ylabel('t')
    plt.title('Data')
    plt.pause(.1)  # required on some systems so that rendering can happen


def plot_model(x, w, color='r'):
    """
    Plot the curve for an n-th order polynomial model:
        t = w0*x^0 + w1*x^1 + w2*x^2 + ... wn*x^n
    This works by creating a set of x-axis (plotx) points and
    then use the model parameters w to determine the corresponding
    t-axis (plott) points on the model curve.
    :param x: sequence of 1-dimensional input data features
    :param w: n-dimensional sequence of model parameters: w0, w1, w2, ..., wn
    :param color: matplotlib color to plot model curve
    :return: the plotx and plott values for the plotted curve
    """
    # NOTE: this assumes a figure() object has already been created.
    plotx = numpy.linspace(min(x) - 0.25, max(x) + 0.25, 100)
    plotX = numpy.zeros((plotx.shape[0], w.size))
    for k in range(w.size):
        plotX[:, k] = numpy.power(plotx, k)
    plott = numpy.dot(plotX, w)
    plt.plot(plotx, plott, color=color, linewidth=2)
    plt.pause(.1)  # required on some systems so that rendering can happen
    return plotx, plott


# -------------------------------------------------------------------------
# Synthetic data generation

def generate_synthetic_data(N, w, xmin=-5, xmax=5, sigma=150):
    """
    Generate some synthetic data
    :param N: Number of sample points
    :param w: numpy array (1d) representing generating model parameters
    :param xmin: x minimum
    :param xmax: x maximum
    :param sigma: standard deviation
    :return:
    """
    # generate N random input x points between [xmin, xmax]
    x = (xmax - xmin) * numpy.random.rand(N) + xmin

    # generate response with Gaussian random noise
    X = numpy.zeros((x.size, w.size))
    for k in range(w.size):
        X[:, k] = numpy.power(x, k)
    t = numpy.dot(X, w) + sigma * numpy.random.randn(x.shape[0])

    return x, t


def plot_synthetic_data(x, t, w, filepath=None):
    plot_data(x, t)
    plt.title('Plot of synthetic data; green curve is original generating function')
    plot_model(x, w, color='g')
    if filepath:
        plt.savefig(filepath, format='pdf')


# -------------------------------------------------------------------------

def plot_cv_results(x, train_loss, cv_loss, log_scale_p=False):
    """
    Helper function to plot the results of cross-validation
    :param train_loss:
    :param cv_loss:
    :param ind_loss:
    :param log_scale_p:
    :return:
    """

    plt.figure()
    if log_scale_p:
        plt.title('Log-scale Mean Square Error Loss')
        ylabel = 'Log MSE Loss'
    else:
        plt.title('Mean Squared Error Loss')
        ylabel = 'MSE Loss'

    # x = numpy.arange(0, train_loss.shape[0])

    # put y-axis on same scale for all plots
    # min_ylim = min(list(train_loss) + list(cv_loss) + list(ind_loss))
    min_ylim = min(cv_loss)
    min_ylim = int(numpy.floor(min_ylim))
    max_ylim = max(cv_loss)
    max_ylim = int(numpy.ceil(max_ylim))

    plt.plot(x, cv_loss, linewidth=2)
    plt.xticks(numpy.arange(0, 1, 0.1))
    plt.xlabel('Lambda')
    plt.ylabel(ylabel)
    plt.title('CV Loss')
    plt.pause(.1) # required on some systems so that rendering can happen
    #plt.ylim(min_ylim, max_ylim)

    #plt.subplots_adjust(right=0.95, wspace=0.4)
    #plt.draw()


# -------------------------------------------------------------------------

def run_cv( K, maxorder, x, t, randomize_data=False, title='CV' ):
    """
    :param K: Number of folds
    :param maxorder: Integer representing the highest polynomial order
    :param x: input data (1d observations)
    :param t: target data
    :param testx: independent test input data
    :param testt: independent test target data
    :param randomize_data: Boolean (default False) whether to randomize the order of the data
    :param title: Title for plots of results
    :return:
    """

    N = x.shape[0]  # number of data points

    lambdas = numpy.linspace(0, 1, 100) # equally spaced lambda values
    lamdda_size = len(lambdas)

    # Use when you want to ensure the order of the data has been
    # randomized before splitting into folds
    # Note that in the simple demo here, the data is already in
    # random order.  However, if you use this function more generally
    # for new data, you may need to ensure you're randomizing the
    # order of the data!
    if randomize_data:
        # use the same permutation P on both x and t, otherwise they'll
        # each be in different orders!
        P = random_permutation_matrix(x.size)
        x = permute_rows(x, P)
        t = permute_rows(t, P)

    # Storage for the design matrix used during training
    # Here we create the design matrix to hold the maximum sized polynomial order
    # When computing smaller model polynomial orders below, we'll just use
    # the first 'k+1' columns for the k-th order polynomial.  This way we don't
    # have to keep creating new design matrix arrays.
    X = numpy.zeros((x.shape[0], maxorder + 1))

    # Design matrix for independent test data
    # testX = numpy.zeros((testx.shape[0], maxorder + 1))

    # Create approximately equal-sized fold indices
    # These correspond to indices in the design matrix (X) rows
    # (where each row represents one training input x)
    fold_indices = map(lambda x: int(x), numpy.linspace(0, N, K + 1))

    # storage for recording loss across model polynomial order
    # rows = fold loss (each row is the loss for one fold)
    # columns = model polynomial order
    cv_loss = numpy.zeros((K, lamdda_size))     # cross-validation loss
    train_loss = numpy.zeros((K, lamdda_size))  # training loss

    # iterate lambda values
    for lam_index in range(lamdda_size):

        # Augment the input data by the polynomial model order
        # E.g., 2nd-order polynomial model takes input x to the 0th, 1st, and 2nd power
        for p in range(maxorder+1):
            X[:, p] = numpy.power(x, p)

        # ... do the same for the independent test data
        # testX[:, p] = numpy.power(testx, p)

        # iterate over folds
        for fold in range(K):
            # Partition the data
            # foldX, foldt contains the data for just one fold being held out
            # trainX, traint contains all other data

            foldX = X[fold_indices[fold]:fold_indices[fold+1], 0:maxorder+1]
            foldt = t[fold_indices[fold]:fold_indices[fold+1]]

            # safely copy the training data (so that deleting doesn't remove the original
            trainX = numpy.copy(X[:, 0:maxorder + 1])
            # remove the fold x from the training set
            trainX = numpy.delete(trainX, numpy.arange(fold_indices[fold], fold_indices[fold + 1]), 0)

            # safely copy the training data (so that deleting doesn't remove the original
            traint = numpy.copy(t)
            # remove the fold t from the training set
            traint = numpy.delete(traint, numpy.arange(fold_indices[fold], fold_indices[fold + 1]), 0)

            # find the least mean squares fit to the training data
            ### YOUR CODE HERE ###
            # w = None  # Calculate w vector (as an numpy.array)
            w = fitpoly(trainX, traint, maxorder, lambdas[lam_index], N)  # Calculate w vector (as an numpy.array)

            # calculate and record the mean squared losses

            train_pred = numpy.dot(trainX, w)  # model predictions on training data
            train_loss[fold, lam_index] = numpy.mean(numpy.power(train_pred - traint, 2))

            fold_pred = numpy.dot(foldX, w)  # model predictions on held-out fold
            cv_loss[fold, lam_index] = numpy.mean(numpy.power(fold_pred - foldt, 2))

            # ind_pred = numpy.dot(testX[:, 0:p + 1], w)   # model predictions on independent test data
            # ind_loss[fold, p] = numpy.mean(numpy.power(ind_pred - testt, 2))

    # The loss values can get quite large, so take the log for display purposes

    # Ensure taking log of the mean (not mean of the log!)
    mean_train_loss = numpy.mean(train_loss, 0)
    mean_cv_loss = numpy.mean(cv_loss, 0)
    # mean_ind_loss = numpy.mean(ind_loss, 0)

    log_mean_train_loss = numpy.log(mean_train_loss)
    log_mean_cv_loss = numpy.log(mean_cv_loss)
    # log_mean_ind_loss = numpy.log(mean_ind_loss)

    print '\n----------------------\nResults for {0}'.format(title)
    print 'log_mean_train_loss:\n{0}'.format(log_mean_train_loss)
    print 'log_mean_cv_loss:\n{0}'.format(log_mean_cv_loss)
    # print 'log_mean_ind_loss:\n{0}'.format(log_mean_ind_loss)

    min_mean_log_cv_loss = min(log_mean_cv_loss)
    # TODO: has to be better way to get the min index...
    best_index = [i for i, j in enumerate(log_mean_cv_loss) if j == min_mean_log_cv_loss][0]

    print 'minimum mean_log_cv_loss of {0} for lambda {1}'.format(min_mean_log_cv_loss, lambdas[best_index])

    # Plot log scale loss results
    plot_cv_results(lambdas, log_mean_train_loss, log_mean_cv_loss, log_scale_p=True)

    # Uncomment to plot direct-scale mean loss results
    # plot_cv_results(mean_train_loss, mean_cv_loss, mean_ind_loss, log_scale_p=False)

    return lambdas[best_index], min_mean_log_cv_loss


# -------------------------------------------------------------------------

def run_demo():
    """
    Top-level script to run the cv demo
    """

    # load data
    data = read_data('data/synthdata2016.csv', ',')
    x = data[:, 0]
    t = data[:, 1]

    K = 10

    best_lambda, min_loss = run_cv( K, 7, x, t, randomize_data=False, title='{0}-fold CV'.format(K) )

    ## compute the weights with the returned best lambda value

    X = numpy.zeros((x.shape[0], 8))
    # Augment the input data by the polynomial model order
    # E.g., 2nd-order polynomial model takes input x to the 0th, 1st, and 2nd power
    for p in range(8):
        X[:, p] = numpy.power(x, p)

    print 'best lambda found:', best_lambda
    w = fitpoly(X, t, 7, best_lambda, x.shape[0])
    print 'identified model parameters:'
    print ['{0:f}'.format(i) for i in w]
    plot_data(x, t)
    plot_model(x, w)

# -------------------------------------------------------------------------
# SCRIPT
# -------------------------------------------------------------------------


run_demo()

plt.show()


