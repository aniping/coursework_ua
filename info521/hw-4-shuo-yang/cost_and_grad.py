def autoencoder_cost_and_grad(theta, visible_size, hidden_size, lambda_, data):
    """
    The input theta is a 1-dimensional array because scipy.optimize.minimize expects
    the parameters being optimized to be a 1d array.
    First convert theta from a 1d array to the (W1, W2, b1, b2)
    matrix/vector format, so that this follows the notation convention of the
    lecture notes and tutorial.
    You must compute the:
        cost : scalar representing the overall cost J(theta)
        grad : array representing the corresponding gradient of each element of theta
    """

    ### YOUR CODE HERE ###

    ## convert theta into matrix/vector format for weights and biases

    # each row of w_l1 is a vector of weights associate each unit (input feature)
    # in layer 1 to an unit at layer 2 (hidden layer)
    w_l1 = theta[0 : hidden_size*visible_size].reshape(hidden_size, visible_size)
    # each row of w_l2 is a vector of weights associate each unit
    # in layer 2 to an unit in layer 3 (output layer)
    w_l2 = theta[hidden_size*visible_size : 2*hidden_size*visible_size].reshape(visible_size, hidden_size)

    # bias term in layer 1 associated with units in layer 2
    b_l1 = theta[2*hidden_size*visible_size : 2*hidden_size*visible_size + hidden_size]
    # bias term in layer 2 associated with units in layer 3
    b_l2 = theta[2*hidden_size*visible_size + hidden_size : ]

    m = data.shape[1] # number of training set

    ## compute feedforward

    a_l1 = data # activation at the input layer is the input data itself
    z_l2 = w_l1.dot(a_l1) + numpy.tile(b_l1, (m, 1)).transpose() # weighted sum of inputs to layer 2
    a_l2 = sigmoid(z_l2) # activation at the hidden layer (layer 2)
    z_l3 = w_l2.dot(a_l2) + numpy.tile(b_l2, (m, 1)).transpose() # weighted sum of inputs to layer 2
    a_l3 = sigmoid(z_l3) # activation at the output layer (layer 3)
    h = a_l3 # output of our hypothesis on input data

    ## calculate the overall cost J(theta)
    cost = numpy.sum((h-data) ** 2) / (2*m) + (lambda_ / 2) * \
           (numpy.sum(w_l1 ** 2) + numpy.sum(w_l2 ** 2))

    ## perform backpropagation

    delta_l3 = -(data - a_l3) * sigmoid_derivative(z_l3)
    delta_l2 = w_l2.transpose().dot(delta_l3) * sigmoid_derivative(z_l2)

    grad_w_l2 = delta_l3.dot(a_l2.transpose()) / m + lambda_ * w_l2
    grad_w_l1 = delta_l2.dot(a_l1.transpose()) / m + lambda_ * w_l1

    grad_b_l2 = numpy.sum(delta_l3, axis=1) / m
    grad_b_l1 = numpy.sum(delta_l2, axis=1) / m

    grad = numpy.concatenate((grad_w_l1.reshape(hidden_size * visible_size),
                              grad_w_l2.reshape(hidden_size * visible_size),
                              grad_b_l1.reshape(hidden_size),
                              grad_b_l2.reshape(visible_size)))

    return cost, grad
