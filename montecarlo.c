// Program to calculate pi using Monte Carlo which involves randomization and creating several
// thread, each of which generates random points and determines if the points fall within the
// circle. Each thread have to update thee global count of all points that fall within the circle.
// Protect against race conditions on updates to the shared global variable by using mutex locks.
//Ganesh Sharma
//CSC 306
//University of Southern Mississippi

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

//initializing variables
void PointCounter (void);
int global_var = 0; //Global variable
int totalPoints = 0; // total accurance given by user
pthread_mutex_t mutex; // mutex for syncronization

//function that counts the total points that is inside or on the circle of unit 1
void PointCounter(void)
{
	int i, hit_count = 0;
	double X, Y;
	//initialize random number x and y for the number of points given
	for (i = 0; i < totalPoints; i++)
    	{
        	X = (rand()/((double)RAND_MAX+1))* 2.0 - 1.0;
        	Y = (rand()/((double)RAND_MAX+1))* 2.0 - 1.0;

		//counting the number of points that fall within the circle
        	if (((X * X) + (Y * Y)) <= 1.0)
       		{
			++hit_count;
        	}
    	}

	//lock resource
	pthread_mutex_lock(&mutex);
	//update global variable
	global_var = hit_count;
	//unlock resource
	pthread_mutex_unlock(&mutex);
}
//child thread
void *child(void *p)
{
	PointCounter();
}
//main program
int main(int argc, char *argv[])
{
	//declaring variable
    	int i;
	//initializing mutex
	pthread_mutex_init(&mutex, NULL);

	//prompt user for input
	//printf ("\nEnter the number of points that you would like to generate to calculate the value of pi ? " );
	//scanf ("%d", &totalCount);
	//stroing the number of points entered in the command line to totalPoints
	totalPoints = atoi(argv[1]);
	//declaring thread as an array
	pthread_t threads[totalPoints];

	//thread creation
	for (i = 0; i < totalPoints; i++)
	{
		pthread_create(&threads[i], NULL, child, NULL);
		printf("Thread [%d] created\n", i+1);
	}

	//waiting for all threads to complete
	for(i = 0; i < totalPoints; i++)
	{
    		pthread_join(threads[i], NULL);
		printf("Thread [%d] completed\n",i+1);
	}

	//pthread_create(&chi,NULL, &child, (void*) NULL);
	//pthread_join(chi,NULL);

	//using given formula
    double pi = (4.0 * global_var) / totalPoints;

	//display the value of pi
	printf ("The approximate value of pi using (%d) points is: %f \n", totalPoints, pi);

	//destroy mutex
	pthread_mutex_destroy(&mutex);

	//exit thread
	pthread_exit(NULL);
	return 0;
}
