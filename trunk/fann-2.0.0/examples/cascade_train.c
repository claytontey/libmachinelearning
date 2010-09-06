/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003 Steffen Nissen (lukesky@diku.dk)

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
*/

#include <stdio.h>

#include "fann.h"


int main()
{
	struct fann *ann;
	struct fann_train_data *train_data, *test_data;
	const float desired_error = (const float) 0.001;
	unsigned int max_neurons = 40;
	unsigned int neurons_between_reports = 1;

	printf("Reading data.\n");

	train_data = fann_read_train_from_file("../benchmarks/datasets/two-spiral.train");
	test_data = fann_read_train_from_file("../benchmarks/datasets/two-spiral.test");

	fann_scale_train_data(train_data, 0, 1);
	fann_scale_train_data(test_data, 0, 1);

	printf("Creating network.\n");

	ann = fann_create_shortcut(2, fann_num_input_train_data(train_data), fann_num_output_train_data(train_data));

	fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_LINEAR_PIECE);
	fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);

	fann_print_parameters(ann);

	printf("Training network.\n");

	fann_cascadetrain_on_data(ann, train_data, max_neurons, neurons_between_reports, desired_error);

	fann_print_connections(ann);

	printf("\nTrain error: %f, Test error: %f\n\n", fann_test_data(ann, train_data),
		   fann_test_data(ann, test_data));

	printf("Saving network.\n");

	fann_save(ann, "two_spirali.net");

	printf("Cleaning up.\n");
	fann_destroy_train(train_data);
	fann_destroy_train(test_data);
	fann_destroy(ann);

	return 0;
}
