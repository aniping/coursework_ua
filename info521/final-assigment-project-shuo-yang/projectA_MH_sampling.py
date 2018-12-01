import numpy as np
from scipy.stats import multivariate_normal
import matplotlib.pyplot as plt

DEBUG = True

def plot_data(pis, pfs, num_samples, camera):
    '''
    Plot samples against iteration
    '''
    s = np.linspace(0, num_samples, num_samples + 1)

    plt.figure()
    plt.suptitle('3d points (Pi, Pf) samples plotted against iteration: {}'.format(camera))
    plt.subplots_adjust(hspace=.8)

    plt.subplot(3,2,1)
    plt.plot(s, pis[0:len(pis):3])
    plt.xlabel('samples')
    plt.ylabel('Pi:x')

    plt.subplot(3,2,3)
    plt.plot(s, pis[1:len(pis):3])
    plt.xlabel('sample')
    plt.ylabel('Pi:y')

    plt.subplot(3,2,5)
    plt.plot(s, pis[2:len(pis):3])
    plt.xlabel('sample')
    plt.ylabel('Pi:z')

    plt.subplot(3,2,2)
    plt.plot(s, pfs[0:len(pfs):3])
    plt.xlabel('sample')
    plt.ylabel('Pf:x')

    plt.subplot(3,2,4)
    plt.plot(s, pfs[1:len(pfs):3])
    plt.xlabel('sample')
    plt.ylabel('Pf:y')

    plt.subplot(3,2,6)
    plt.plot(s, pfs[2:len(pfs):3])
    plt.xlabel('sample')
    plt.ylabel('Pf:z')

    plt.savefig('./plots/{}.png'.format(camera), fmt='png')

    plt.show()


def load_data_from_csv(fname, delimiter=','):
    """
    Load data from the given csv file as an numpy array
    """
    return np.genfromtxt(fname, delimiter=delimiter)


def point_3D_to_2D(point_3d, camera_matrix):
    tmp = camera_matrix.dot(np.append(point_3d, 1))
    tmp = tmp/tmp[2]
    point_2d = tmp[0:2]
    return point_2d


def likelihood(inputs, qi, qf, points_noisy_2d, covar_2d_noise):
    # calculate qs, which is used as mean for computing likelihood
    qs = np.zeros((inputs.shape[0], 2))
    for i in range(0, inputs.shape[0]):
        qs[i] = qi + (qf - qi) * inputs[i]

    lh = np.zeros((inputs.shape[0], 1))
    for i in range(0, inputs.shape[0]):
        lh[i] = multivariate_normal.logpdf(points_noisy_2d[i],
                                           mean=qs[i],
                                           cov=covar_2d_noise)

    return lh


def MH_sampling(inputs, points_noisy_2d, num_samples,
                camera_matrix, covar_2d_noise,
                covar_3d_line_prior, mean_3d_line_prior):
    """
    Uses the Metroplis-Hastings algorithm to sample from the posterior of pi and pf
    """
    # take the first sample (initial and final 3D points)
    # from the prior distribution of the 3D line
    pi_sample, pf_sample = np.random.multivariate_normal(mean_3d_line_prior,
                                                         covar_3d_line_prior, 2)

    pi_sample_array = np.array(pi_sample)
    pf_sample_array = np.array(pf_sample)

    qi_sample = point_3D_to_2D(pi_sample, camera_matrix)
    qf_sample = point_3D_to_2D(pf_sample, camera_matrix)

    # generating samples using Metropolis Hastings
    for i in range(0, num_samples):

        # compute the likelihood for the current sample
        lh_sample = likelihood(inputs, qi_sample, qf_sample, points_noisy_2d, covar_2d_noise)

        # compute prior for the current sample
        prior_sample = multivariate_normal.logpdf(np.array([pi_sample, pf_sample]),
                                                  mean=mean_3d_line_prior,
                                                  cov=covar_3d_line_prior)

        # compute the posterior for the current sample
        post_sample = np.sum(lh_sample) + np.sum(prior_sample)

        # generate next samples given the current sample as mean
        pi_next_sample = np.random.multivariate_normal(pi_sample, covar_3d_line_prior)
        pf_next_sample = np.random.multivariate_normal(pf_sample, covar_3d_line_prior)

        pi_sample_array = np.append(pi_sample_array, pi_next_sample)
        pf_sample_array = np.append(pf_sample_array, pf_next_sample)

        qi_next_sample = point_3D_to_2D(pi_next_sample, camera_matrix)
        qf_next_sample = point_3D_to_2D(pf_next_sample, camera_matrix)

        # compute the likelihood for the current sample
        lh_next_sample = likelihood(inputs, qi_next_sample, qf_next_sample, points_noisy_2d, covar_2d_noise)

        # compute prior for the current sample
        prior_next_sample = multivariate_normal.logpdf(np.array([pi_next_sample, pf_next_sample]),
                                                       mean=mean_3d_line_prior,
                                                       cov=covar_3d_line_prior)

        # compute the posterior for the current sample
        post_next_sample = np.sum(lh_next_sample) + np.sum(prior_next_sample)

        if DEBUG:
            # print ('P_i:', pi_sample)
            # print ('P_f:', pf_sample)
            # print ('Q_i:', qi)
            # print ('Q_f:', qf)
            # print ('likelihood:\n', lh_sample)
            # print ('prior:\n', prior_sample)
            # print ('posterior (current sample): {}, posterior (next sample): {}'.format(post_sample, post_next_sample))
            pass

        log_ratio = post_next_sample - post_sample # posterior ratio in log scale
        # ratio = np.exp(log_ratio) # convert log ratio back to normal
        # print('ratio:', ratio)

        if log_ratio >= 0:
            pi_sample = pi_next_sample
            pf_sample = pf_next_sample
            qi_sample = qi_next_sample
            qf_sample = qf_next_sample
            # print('accept 100%')
        else:
            u = np.random.uniform()
            if np.log(u) < log_ratio:
                pi_sample = pi_next_sample
                pf_sample = pf_next_sample
                qi_sample = qi_next_sample
                qf_sample = qf_next_sample
                # print('accept {}%'.format(u * 100))

    print ('\nMAP estimate of the 3D line segment pi:\n {0}, pf: {1}'.format(pi_sample, pf_sample))

    # Calcuate Monte Carlo estimate of the mean of posterior distribution
    pi_x = pi_sample_array[0:len(pi_sample_array):3]
    pi_x_mean = np.sum(pi_x) / num_samples
    pi_y = pi_sample_array[1:len(pi_sample_array):3]
    pi_y_mean = np.sum(pi_y) / num_samples
    pi_z = pi_sample_array[2:len(pi_sample_array):3]
    pi_z_mean = np.sum(pi_z) / num_samples
    pi_mean = np.array([pi_x_mean, pi_y_mean, pi_z_mean])

    pf_x = pf_sample_array[0:len(pf_sample_array):3]
    pf_x_mean = np.sum(pf_x) / num_samples
    pf_y = pf_sample_array[1:len(pf_sample_array):3]
    pf_y_mean = np.sum(pf_y) / num_samples
    pf_z = pf_sample_array[2:len(pf_sample_array):3]
    pf_z_mean = np.sum(pf_z) / num_samples
    pf_mean = np.array([pf_x_mean, pf_y_mean, pf_z_mean])

    print ('\nMonte Carlo estimate of the mean of posterior distribution\n: E(pi): {0}, E(pf): {1}'.format(pi_mean, pf_mean))

    # Calcuate Monte Carlo estimate of the predicted (2D) output point
    qt = qi_sample + (qf_sample - qi_sample) * -0.5
    print ('\nMonte Carlo estimate of the predicted (2D) output point at -0.5: {}'.format(qt))

    return pi_sample_array, pf_sample_array


if __name__ == '__main__':
    num_samples = 1000 # number of samples
    # camera matrix for the first camera
    camera_matrix = np.array([[1, 0, 0, 0],
                              [0, 1, 0, 0],
                              [0, 0, 1, 0]])

    inputs = load_data_from_csv('inputs.csv')
    points_noisy_2d = load_data_from_csv('points_2d_camera_1.csv')

    # covariance matrix for Gaussian noisy 2D points
    covar_2d_noise = 0.05**2 * np.identity(2)
    # covariance matrix for the Gaussian prior belief of the 3D line
    covar_3d_line_prior = 10 * np.identity(3)
    # mean of the Gaussian prior belief of the 3D line
    mean_3d_line_prior = np.array([0, 4, 0])

    pis, pfs = MH_sampling(inputs, points_noisy_2d, num_samples,
                            camera_matrix, covar_2d_noise,
                            covar_3d_line_prior, mean_3d_line_prior)

    # plot_data(pis, pfs, num_samples, 'camera1')

    camera_matrix_2 = np.array([[0, 0, 1, -5],
                                [0, 1, 0, 0],
                                [-1, 0, 0, 5]])
    points_noisy_2d_2 = load_data_from_csv('points_2d_camera_2.csv')
    pis, pfs = MH_sampling(inputs, points_noisy_2d_2, num_samples,
                           camera_matrix_2, covar_2d_noise,
                           covar_3d_line_prior, mean_3d_line_prior)

    plot_data(pis, pfs, num_samples, 'camera2')
