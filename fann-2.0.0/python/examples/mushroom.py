#!/usr/bin/python
import os
import sys
from pyfann import fann

def print_callback(epochs, error):
	print "Epochs     %8d. Current MSE-Error: %.10f\n" % (epochs, error)
	return 0

# initialize network parameters
connection_rate = 1
learning_rate = 0.7
num_neurons_hidden = 32
desired_error = 0.000001
max_iterations = 300
iterations_between_reports = 1

# create training data, and ann object
print "Creating network."	
train_data = fann.read_train_from_file(os.path.join("..","..","benchmarks","datasets","mushroom.train"))
ann = fann.create(connection_rate, (train_data.get_num_input(), num_neurons_hidden, train_data.get_num_output()))
ann.set_learning_rate(learning_rate)


# start training the network
print "Training network"
ann.set_activation_function_hidden(fann.SIGMOID_SYMMETRIC_STEPWISE)
ann.set_activation_function_output(fann.SIGMOID_STEPWISE)
ann.set_training_algorithm(fann.TRAIN_INCREMENTAL)
	
ann.train_on_data(train_data, max_iterations, iterations_between_reports, desired_error)
	
# test outcome
print "Testing network"
test_data = fann.read_train_from_file(os.path.join("..","..","benchmarks","datasets","mushroom.test"))

ann.reset_MSE()
for i in range(test_data.get_num_data()):
    ann.test(test_data.get_input(i), test_data.get_output(i))

print "MSE error on test data: %f" % ann.get_MSE()

# save network to disk
print "Saving network"
ann.save(os.path.join("nets","mushroom_float.net"))

