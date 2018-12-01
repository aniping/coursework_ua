import numpy
import matplotlib.pyplot as plt

def exercise6(infile, outfile):
    """
    Solution for Problem 6.
    :param infile: input data file
    :param outfile: output data file
    """
    humu_array = numpy.loadtxt(infile)
    print('Type of variable returned by numpy.loadtxt:', type(humu_array))
    print('Size of humu array:', humu_array.size)

    height, width = humu_array.shape
    print('Shape of humu array: {}(height) x {}(width)'.
          format(height, width))

    min_val = numpy.amin(humu_array)
    max_val = numpy.amax(humu_array)

    print('Min of humu array:', min_val)
    print('Max of humu array:', max_val)

    # Scale the array to the range [0,1]
    scaled_humu_array = (humu_array - min_val) / (max_val - min_val)
    print('Shape of the scaled humu array: {}(height) x {}(width)'.
          format(scaled_humu_array.shape[0], scaled_humu_array.shape[1]))
    print('Min of scaled humu array:', numpy.amin(scaled_humu_array))
    print('Max of scaled humu array:', numpy.amax(scaled_humu_array))

    fig = plt.figure()
    plt.imshow(humu_array)
    plt.show()
    print('Default colormap: ', plt.cm.cmapname)

    plt.imshow(humu_array, cmap='gray')
    plt.show()

    rand_array = numpy.random.random(size=(height, width))
    plt.imshow(rand_array, cmap='gray')
    plt.show()

    plt.imshow(numpy.random.random(size=(height, width)), cmap='gray')
    plt.show()

    # Write the random array to a text file
    numpy.savetxt(outfile, rand_array)


def run_experiments(num_runs):
    """
    Run experiments mentioned in the problem 9.
    :param num_runs: number of experiments to run.
    """
    for run in range(num_runs):
        # throw two 6-sided dies 1000 times and estimate the
        # probability of double six
        double_six_count = 0
        for i in range(1000):
            die1 = numpy.random.randint(1, 7)
            die2 = numpy.random.randint(1, 7)
            if die1 == 6 and die2 == 6:
                double_six_count += 1

        double_six_prob = double_six_count / 1000
        print('Run {} - Probability of double sixes of 1000 throws: {:.1%}'.
              format(run+1, double_six_prob))


def exercise9():
    """
    Solution for Problem 9.
    """
    # set the seed to 8 and run the experiment for 10 times
    print('\nSet the seed to 8 and run the experiment for 10 times:\n')
    numpy.random.seed(seed=8)
    run_experiments(10)

    # set the seed to 8 again and run the same experiment for 10 times
    print('\nSet the seed to 8 agin and rerun the experiment for another 10 times:\n')
    numpy.random.seed(seed=8)
    run_experiments(10)

def exercise10():
    """
    Solution for Problem 10.
    """
    numpy.random.seed(seed=5)
    a = numpy.random.rand(3, 1)
    b = numpy.random.rand(3, 1)
    print('variable a:\n', a)
    print('variable b:\n', b)

    print('\na + b = \n', a + b)
    print('\nElement-wise multiply of a and b:\n', numpy.multiply(a, b))
    print('\nDot-product of a and b:\n', numpy.dot(numpy.transpose(a), b))

    numpy.random.seed(seed=2)
    X = numpy.random.rand(3, 3)
    print('\nvariable X:\n', X)

    print('\nOutput for question 4:\n',
          numpy.mat(numpy.transpose(a)) * numpy.mat(X))

    print('\nOutput for question 5:\n',
          numpy.mat(numpy.transpose(a)) * numpy.mat(X) * numpy.mat(b))

    print('\nOutput for question 6:\n', numpy.linalg.inv(X))

def exercise11():
    """
    Solution for Problem 11.
    """
    x = numpy.arange(11, step=0.01)
    sinx = numpy.sin(x)

    plt.figure()
    plt.plot(sinx)
    plt.xlabel('x value')
    plt.ylabel('sin(x)')
    plt.title('Sine Function for x from 0.0 to 10.0')
    plt.savefig('sin_plot.pdf', format='pdf')


if __name__ == '__main__':
    exercise6('humu.txt', 'out.txt')
    exercise9()
    exercise10()
    exercise11()
