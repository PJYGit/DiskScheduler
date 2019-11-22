/*
DScheduler.c

Student Name : Jiayao Pang
Student ID # : 194174300

*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dscheduler.h"


/*
Any required standard libraries and your header files here
*/

struct schedulerInput loadRequest(){
    struct schedulerInput results;
    int numRequests;
    char line_buffer[MAX_LINE_LENGTH];
    char direction;
    char *token;
    
    //Process simulation input line by line
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);

    token = strtok(line_buffer, " ");
    sscanf(token, "%d",&numRequests);
    
    token = strtok(NULL, " ");
    sscanf(token, "%d",&results.startTrack);
       
    token = strtok(NULL, " ");
    sscanf(token, "%c",&direction);
    results.direction = direction == 'u' ? 1 : -1;
    
    results.requests.elements = numRequests;
    results.requests.data = (int *)malloc(sizeof(int)*numRequests);
    if ( results.requests.data == NULL ){
        fprintf( stderr, "Was unable to allocate space for requests data.\n");
        exit( BAD_MALLOC );
    }
    
    for (int i = 0; i < numRequests; i++){
        token = strtok(NULL, " ");
        sscanf(token, "%d", &results.requests.data[i]);
    }

    return results;
}

void printResults(struct schedulerResult results){
    for (int i = 0; i < results.requests.elements; i++){
        printf("%4d", results.requests.data[i]);
    }
    printf("Total Head Movement %5d\n", results.totalHeadMovement);
}

struct schedulerResult processRequest(enum POLICIES policy, struct schedulerInput request){
    struct schedulerResult results;
	results.totalHeadMovement = 0;
    
    switch(policy){
        case FCFS:
            return process_FCFS_request(request);
        case SSTF:
            return process_SSTF_request(request);
        case SCAN:
            return process_SCAN_request(request);
        case C_SCAN:
            return process_C_SCAN_request(request);
    }
    return results;
}



/* Fill in the following functions */
struct schedulerResult process_FCFS_request(struct schedulerInput request){
    struct schedulerResult results;
	results.requests = request.requests;
	results.totalHeadMovement = 0;

	int size = request.requests.elements;
	int sum = abs(request.startTrack - results.requests.data[0]);
	for (int i = 0; i < size - 1; i++)
		sum += abs(results.requests.data[i] - results.requests.data[i + 1]);

	results.totalHeadMovement = sum;

    return results;
}

struct schedulerResult process_SSTF_request(struct schedulerInput request){
    struct schedulerResult results;
	results.totalHeadMovement = 0;

	int size = request.requests.elements;
	int* sequence = request.requests.data;
	results.requests.elements = size;
	results.requests.data = (int*)malloc(sizeof(int) * size);

	int cur_pos = request.startTrack;
	int finished = 0;

	int* done = (int*)malloc(sizeof(int) * size);
	for (int j = 0; j < size; j++)
	{
		done[j] = 0;
	}

	while (finished != size)
	{
		int least_diff = 1205;
		int index = -1;
		for (int i = 0; i < size; i++)
		{
			if (done[i] == 0)
			{
				if (abs(sequence[i] - cur_pos) < least_diff)
				{
					least_diff = abs(sequence[i] - cur_pos);
					index = i;
				}
			}
		}


		done[index] = 1;

		results.requests.data[finished] = request.requests.data[index];
		results.totalHeadMovement += abs(cur_pos - request.requests.data[index]);

		cur_pos = request.requests.data[index];
		finished++;
	}

    return results;
}

struct schedulerResult process_SCAN_request(struct schedulerInput request){
    struct schedulerResult results;
	results.totalHeadMovement = 0;

	int size = request.requests.elements;
	int* sequence = request.requests.data;
	int direction = request.direction;
	results.requests.elements = size;
	results.requests.data = (int*)malloc(sizeof(int) * size);

	int cur_pos = request.startTrack;
	int finished = 0;

	int* done = (int*)malloc(sizeof(int) * size);
	for (int j = 0; j < size; j++)
	{
		done[j] = 0;
	}

	while (finished != size)
	{
		int least_diff = 1205;
		int index = -1;
		for (int i = 0; i < size; i++)
		{
			if (direction == 1 && done[i] == 0 && sequence[i] >= cur_pos)
			{
				if (abs(sequence[i] - cur_pos) < least_diff)
				{
					least_diff = abs(sequence[i] - cur_pos);
					index = i;
				}
			}

			else if (direction == -1 && done[i] == 0 && sequence[i] <= cur_pos)
			{
				if (abs(sequence[i] - cur_pos) < least_diff)
				{
					least_diff = abs(sequence[i] - cur_pos);
					index = i;
				}
			}
		}

		if (index == -1)
		{
			if (direction == 1)
			{
				results.totalHeadMovement += abs(cur_pos - 1023);
				cur_pos = 1023;
				direction = -1;
			}
				
			else
			{
				results.totalHeadMovement += cur_pos;
				cur_pos = 0;
				direction = 1;
			}

			continue;
		}

		done[index] = 1;

		results.requests.data[finished] = request.requests.data[index];
		results.totalHeadMovement += abs(cur_pos - request.requests.data[index]);

		cur_pos = request.requests.data[index];
		finished++;
	}

    return results;
}

struct schedulerResult process_C_SCAN_request(struct schedulerInput request){
    struct schedulerResult results;
	results.totalHeadMovement = 0;

	int size = request.requests.elements;
	int* sequence = request.requests.data;
	results.requests.elements = size;
	results.requests.data = (int*)malloc(sizeof(int) * size);

	int cur_pos = request.startTrack;
	int finished = 0;

	int* done = (int*)malloc(sizeof(int) * size);
	for (int j = 0; j < size; j++)
	{
		done[j] = 0;
	}

	while (finished != size)
	{
		int least_diff = 1205;
		int index = -1;
		for (int i = 0; i < size; i++)
		{
			if (done[i] == 0 && sequence[i] >= cur_pos)
			{
				if (abs(sequence[i] - cur_pos) < least_diff)
				{
					least_diff = abs(sequence[i] - cur_pos);
					index = i;
				}
			}
		}

		if (index == -1)
		{
			results.totalHeadMovement += abs(cur_pos - 1023);
			results.totalHeadMovement += 1023;
			cur_pos = 0;
			continue;
		}

		done[index] = 1;

		results.requests.data[finished] = request.requests.data[index];
		results.totalHeadMovement += abs(cur_pos - request.requests.data[index]);

		cur_pos = request.requests.data[index];
		finished++;
	}

    return results;
}


