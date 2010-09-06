"""
Fast Artificial Neural Network Library (fann)
Copyright (C) 2004 Steffen Nissen (lukesky@diku.dk)

Python object by Gil Megidish, 2004

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
"""

import libfann

# import all FANN_ constants without FANN_ prefix
for name, value in libfann.__dict__.iteritems():
    if name.startswith('FANN_') and not name.endswith('_NAMES'):
        globals()[name[5:]] = value
del name, value

class fann_class:

    def __init__(self, ann):
        """
        Never call this directly.
        """
        self.__ann = ann

    def __del__(self):
        libfann.fann_destroy(self.__ann)

    def get_native_object(self):
        return self.__train_data

    def run(self, input):
        """
        Runs a input through the network, and returns the output.
        """
        return libfann.fann_run(self.__ann, input)

    def randomize_weights(self, min_weight, max_weight):
        """
        Randomize weights (from the beginning the weights are random between -0.1 and 0.1)
        """
        libfann.fann_randomize_weights(self.__ann, min_weight, max_weight)

    def save(self, filename):
        """
        Save the entire network to a configuration file.
        """
        libfann.fann_save(self.__ann, filename)

    def save_to_fixed(self, filename):
        """
        Saves the entire network to a configuration file.
        But it is saved in fixed point format no matter which
        format it is currently in.

        This is usefull for training a network in floating points,
        and then later executing it in fixed point.

        The function returns the bit position of the fix point, which
        can be used to find out how accurate the fixed point network will be.
        A high value indicates high precision, and a low value indicates low
        precision.

        A negative value indicates very low precision, and a very
        strong possibility for overflow.
        (the actual fix point will be set to 0, since a negative
        fix point does not make sence).

        Generally, a fix point lower than 6 is bad, and should be avoided.
        The best way to avoid this, is to have less connections to each neuron,
        or just less neurons in each layer.

        The fixed point use of this network is only intended for use on machines that
        have no floating point processor, like an iPAQ. On normal computers the floating
        point version is actually faster.
        """
        return libfann.fann_save_to_fixed(self.__ann, filename)

    def train(self, input, desired_output):
        """
        Train one iteration with a set of inputs, and a set of desired outputs.
        """
	libfann.fann_train(self.__ann, input, desired_output)

    def test(self, input, output):
        """
        Test with a set of inputs, and a set of desired outputs.
        This operation updates the mean square error, but does not
        change the network in any way.
        """
        libfann.fann_test(self.__ann, input, output)

    def get_MSE(self):
        """
        Reads the mean square error from the network.
        """
        return libfann.fann_get_MSE(self.__ann)

    def reset_error(self):
        """
        Resets the mean square error from the network.
        (obsolete will be removed at some point, use fann_reset_MSE)
        """
        libfann.fann_reset_error(self.__ann)

    def reset_MSE(self):
        """
        Resets the mean square error from the network.
        """
        libfann.fann_reset_MSE(self.__ann)

    def get_num_input(self):
        """
        Get the number of input neurons.
        """
        return libfann.fann_get_num_input(self.__ann)

    def get_num_output(self):
        """
        Get the number of output neurons.
        """
        return libfann.fann_get_num_output(self.__ann)

    def get_total_neurons(self):
        """
        Get the total number of neurons in the entire network.
        """
        return libfann.fann_get_total_neurons(self.__ann)

    def get_total_connections(self):
        """
        Get the total number of connections in the entire network.
        """
        return libfann.fann_get_total_connections(self.__ann)

    def get_learning_rate(self):
        """
        Get the learning rate. 
        """
        return libfann.fann_get_learning_rate(self.__ann)

    def set_learning_rate(self, learning_rate):
        """
        Set the learning rate.
        """
        return libfann.fann_set_learning_rate(self.__ann, learning_rate)

    def train_on_file(self, filename, max_iterations, iterations_between_reports, desired_error):
        libfann.fann_train_on_file(self.__ann, filename, max_iterations, iterations_between_reports, desired_error)

    def get_activation_function_hidden(self):
        """
        Get the activation function used in the hidden layers.
        """
        return libfann.fann_get_activation_function_hidden(self.__ann)

    def set_activation_function_hidden(self, activation_function):
        """
        Set the activation function for the hidden layers.
        """
        libfann.fann_set_activation_function_hidden(self.__ann, activation_function)

    def get_activation_function_output(self):
        """
        Get the activation function used in the output layer.
        """
        return libfann.fann_get_activation_function_output(self.__ann)

    def set_activation_function_output(self, activation_function):
        """
        Set the activation function for the output layer.
        """
        libfann.fann_set_activation_function_output(self.__ann, activation_function)

    def get_activation_steepness_hidden(self):
        """
        Get the steepness parameter for the sigmoid function used in the hidden layers.
        """
        return libfann.get_activation_steepness_hidden(self.__ann)

    def set_activation_steepness_hidden(self, steepness):
        """
        Set the steepness of the sigmoid function used in the hidden layers.
        Only usefull if sigmoid function is used in the hidden layers (default 0.5).
        """
        libfann.fann_set_activation_steepness_hidden(self.__ann, steepness)

    def get_activation_steepness_output(self):
        """
        Get the steepness parameter for the sigmoid function used in the output layer.
        """
        return libfann.fann_get_activation_steepness_output(self.__ann)

    def set_activation_steepness_output(self, steepness):
        """
        Set the steepness of the sigmoid function used in the output layer.
        Only usefull if sigmoid function is used in the output layer (default 0.5).
        """
        libfann.fann_set_activation_steepness_output(self.__ann, steepness)

    def train_on_data(self, data, max_epochs, epochs_between_reports, desired_error):
        """
        Trains on an entire dataset, for a maximum of max_epochs
        epochs or until mean square error is lower than desired_error.
        Reports about the progress is given every
        epochs_between_reports epochs.
        If epochs_between_reports is zero, no reports are given.
        """
        libfann.fann_train_on_data(self.__ann, data.get_native_object(), max_epochs, epochs_between_reports, desired_error)

    def train_on_file(self, filename, max_epochs, epochs_between_reports, desired_error):
        """
        Does the same as train_on_data, but reads the data directly from a file.
        """
        libfann.fann_train_on_file(self.__ann, filename, max_epochs, epochs_between_reports, desired_error)

    def print_connections(self):
        """
        Print out which connections there are in the ann
        """
        libfann.fann_print_connections(self.__ann)
 
    def print_parameters(self):
        """
        Prints all of the parameters and options of the ANN
        """
        libfann.fann_print_parameters(self.__ann)

    def	get_training_algorithm(self):
        """
        Get the training algorithm.
        """
        return libfann.fann_get_training_algorithm(self.__ann)

    def set_training_algorithm(self, training_algorithm):
        """
        Set the training algorithm.
        """
        libfann.fann_set_training_algorithm(self.__ann, training_algorithm)

## end of class fann_class ##

class train_class:

    def __init__(self, train_data):
        """
        Never call this directly.
        """
        self.__train_data = train_data
    def __del__(self):
        libfann.fann_destroy_train(self.__train_data)

    def get_native_object(self):
        return self.__train_data

    def get_num_data(self):
        return self.__train_data.num_data

    def get_num_input(self):
        return self.__train_data.num_input

    def get_num_output(self):
        return self.__train_data.num_output

    def get_input(self, index):
	return libfann.get_train_data_input(self.__train_data, index);

    def get_output(self, index):
	return libfann.get_train_data_output(self.__train_data, index);

    def shuffle(self):
        """
        Shuffles training data, randomizing the order
        """
        libfann.fann_shuffle_train_data(self.__train_data)

    def save(self, filename):
        """
        Save the training structure to a file.
        """
        libfann.fann_save_train(self.__train_data, filename)

    def save_to_fixed(self, filename):
        """
        Saves the training structure to a fixed point data file.
        (Very usefull for testing the quality of a fixed point network).
        """
        libfann.fann_save_train_to_fixed(self.__train_data, filename)

    def merge(self, other):
        """
        Merges training data into a new struct
        """
        outcome = libfann.fann_merge_train_data(self.__train_data, other.get_native_object())
        return train_class(outcome)

    def duplicate(self):
        """
        Return a copy of a fann_train_data struct
        """
        outcome = libfann.fann_duplicate_train_data(self.__train_dat)
        return train_class(outcome)

def create(connRate, layers):
    """
    Constructs a backpropagation neural network, from an connection rate,
    a learning rate, and number of neurons in each layer.

    The connection rate controls how many connections there will be in the
    network. If the connection rate is set to 1, the network will be fully
    connected, but if it is set to 0.5 only half of the connections will be set.

    There will be a bias neuron in each layer (except the output layer),
    and this bias neuron will be connected to all neurons in the next layer.
    When running the network, the bias nodes always emits 1
    """
    ann = libfann.fann_create_sparse_array(connRate,len(layers), layers)
    if libfann.fann_is_NULL(ann):
        return None # probably won't happen
    return fann_class(ann)

def create_from_file(filename):
    """
    Constructs a backpropagation neural network from a configuration file.
    """
    ann = libfann.fann_create_from_file(filename)
    if libfann.fann_is_NULL(ann):
        raise IOError, "Could not load ann from file '%s'" + filename
    return fann_class(ann)

def read_train_from_file(filename):
    """
    Reads a file that stores training data, in the format:
    num_train_data num_input num_output\n
    inputdata seperated by space\n
    outputdata seperated by space\n

    .
    .
    .
   
    inputdata seperated by space\n
    outputdata seperated by space\n
    """
    train = libfann.fann_read_train_from_file(filename)
    return train_class(train)

"""
fann_set_error_log = _libfann.fann_set_error_log
fann_get_errno = _libfann.fann_get_errno
fann_reset_errno = _libfann.fann_reset_errno
fann_reset_errstr = _libfann.fann_reset_errstr
fann_get_errstr = _libfann.fann_get_errstr
fann_print_error = _libfann.fann_print_error

fann_train_on_data_callback = _libfann.fann_train_on_data_callback
fann_train_on_file_callback = _libfann.fann_train_on_file_callback
"""
