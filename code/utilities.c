#include "utilities.h"

int find_nearest(int* sequence, int* done, int cur_pos, int size)
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
	return index;
}

int find_nearest_with_direction(int* sequence, int* done, int direction, int cur_pos, int size)
{
	int least_diff = 1205;
	int index = -1;
	for (int i = 0; i < size; i++)
	{
		if (done[i] == 0 && sequence[i] >= cur_pos && direction)
		{
			if (abs(sequence[i] - cur_pos) < least_diff)
			{
				least_diff = abs(sequence[i] - cur_pos);
				index = i;
			}
		}

		else if (direction == 0 && done[i] == 0 && sequence[i] <= cur_pos)
		{
			if (abs(sequence[i] - cur_pos) < least_diff)
			{
				least_diff = abs(sequence[i] - cur_pos);
				index = i;
			}
		}
	}
	return index;
}

void init_to_zero(int* target, int n)
{
	int j;
	for (j = 0; j < n; j++)
	{
		target[j] = 0;
	}
}