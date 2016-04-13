#include "EyeCenterDetectionHeaders.h"

Snakuscule::Snakuscule()
{
	alpha = sqrt(float(2.0));
	positions[50] = { 0 };		

	memset(hashTable, 0, sizeof(hashTable));	
}

float Snakuscule::hashEnergy(Mat grayFrame, int center_x, int center_y, int radius)
{
	if (hashTable[center_x][center_y][radius] == 0)
	{
		hashTable[center_x][center_y][radius] = snakeEnergy(grayFrame, radius, alpha, center_x, center_y);
	}
	hashChanges.push_back({{ center_x, center_y, radius }});

	return hashTable[center_x][center_y][radius];
}

Point Snakuscule::runSnakuscule(Mat frame, Point centerEstimate, int * radius)
{	
	Mat grayFrame = imageProcessingMethods.RGB2GRAY(frame);
		
	blur(grayFrame, grayFrame, Size(5, 5));
	
	int radiiTotal = 0;
	int count = 0;
	memset(positions, 0, sizeof(positions));
	int center[2] = {centerEstimate.x, centerEstimate.y};

	for (int x = 0; x < hashChanges.size(); x++)
	{
		hashTable[hashChanges[x][0]][hashChanges[x][1]][hashChanges[x][2]] = 0;
	}
	hashChanges.clear();
		
	while (true)
	{		
		float diff_xpos = hashEnergy(grayFrame, center[0], center[1], *radius) - hashEnergy(grayFrame, center[0] + 1, center[1], *radius);
		float diff_xneg = hashEnergy(grayFrame, center[0], center[1], *radius) - hashEnergy(grayFrame, center[0] - 1, center[1], *radius);
		float diff_ypos = hashEnergy(grayFrame, center[0], center[1], *radius) - hashEnergy(grayFrame, center[0], center[1] + 1, *radius);
		float diff_yneg = hashEnergy(grayFrame, center[0], center[1], *radius) - hashEnergy(grayFrame, center[0], center[1] - 1, *radius);
		float diff_radpos = hashEnergy(grayFrame, center[0], center[1], *radius) - hashEnergy(grayFrame, center[0], center[1], *radius + 1);
		float diff_radneg = hashEnergy(grayFrame, center[0], center[1], *radius) - hashEnergy(grayFrame, center[0], center[1], *radius - 1);
		
		/*
		float diff_xpos = snakeEnergy(grayFrame, *radius, alpha, center[0] + 1, center[1]) - snakeEnergy(grayFrame, *radius, alpha, center[0], center[1]);
		float diff_xneg = snakeEnergy(grayFrame, *radius, alpha, center[0] - 1, center[1]) - snakeEnergy(grayFrame, *radius, alpha, center[0], center[1]);
		float diff_ypos = snakeEnergy(grayFrame, *radius, alpha, center[0], center[1] + 1) - snakeEnergy(grayFrame, *radius, alpha, center[0], center[1]);
		float diff_yneg = snakeEnergy(grayFrame, *radius, alpha, center[0], center[1] - 1) - snakeEnergy(grayFrame, *radius, alpha, center[0], center[1]);
		float diff_radpos = snakeEnergy(grayFrame, *radius + 1, alpha, center[0], center[1]) - snakeEnergy(grayFrame, *radius, alpha, center[0], center[1]);
		float diff_radneg = snakeEnergy(grayFrame, *radius - 1, alpha, center[0], center[1]) - snakeEnergy(grayFrame, *radius, alpha, center[0], center[1]);
		*/

		float energy_array[6] = { diff_xpos, diff_xneg, diff_ypos, diff_yneg, diff_radpos, diff_radneg };
		
		float min = energy_array[0];
		int pos = 0;
		for (int i = 0; i<6; i++)
		{
			if (energy_array[i] < min)
			{
				min = energy_array[i];
				pos = i;
			}
		}

		if (pos == 0)
			center[0] = center[0] + 1;
		else if (pos == 1)
			center[0] = center[0] - 1;
		else if (pos == 2)
			center[1] = center[1] + 1;
		else if (pos == 3)
			center[1] = center[1] - 1;
		else if (pos == 4)
		{
			*radius = *radius + 1;
			radiiTotal++;
		}
		else if (pos == 5)
		{
			*radius = *radius - 1;
			radiiTotal--;
		}

		if (radiiTotal >= 3)
			break;

		if (count > 10)
			break;

		positions[count] = pos;

		if (positions[count] * positions[count - 1] == 6 || positions[count] * positions[count - 1] == 20)
			break;
		
		if ((positions[count] == 1 && positions[count - 1] == 0) || (positions[count] == 0 && positions[count - 1] == 1))
			break;
		
		count++;
	}
	return Point(center[0], center[1]);
}

float Snakuscule::snakeEnergy(Mat image, int innerRadius, float alpha, int center_x, int center_y)
{ 
	int center[2] = { center_x, center_y };
	return outerAnnulusEnergy(image, innerRadius, alpha, center) - innerCircleEnergy(image, innerRadius, center[0], center[1]);
}

float Snakuscule::outerAnnulusEnergy(Mat image, int innerRadius, float alpha, int * center)
{
	float energy = 0;
	int pixelCount = 1;
	int outerRadius = alpha*innerRadius;
	
	for (int i = center[0] - outerRadius - 1; i <= center[0] + outerRadius + 1; i++)
	{
		for (int j = center[1] - outerRadius - 1; j <= center[1] + outerRadius + 1; j++)
		{
			if ((sqrt(pow(double(center[0] - i), 2) + pow(double(center[1] - j), 2)) <= outerRadius) && (sqrt(pow(double(center[0] - i), 2) + pow(double(center[1] - j), 2)) >= innerRadius))
			{	
				energy = energy + image.at<uchar>(j, i);
				pixelCount++;
			}
		}
	}
	return energy / pixelCount;
}

float Snakuscule::innerCircleEnergy(Mat image, int radius, int center_x, int center_y)
{
	float energy = 0;
	int pixelCount = 1;
	
	for (int i = center_x - radius - 1; i <= center_x + radius + 1; i++)
	{
		for (int j = center_y - radius - 1; j <= center_y + radius + 1; j++)
		{
			if (sqrt(pow(double(center_x - i), 2) + pow(double(center_y - j), 2)) <= radius)
			{
				energy = energy + image.at<uchar>(j, i);
				pixelCount++;
			}
		}
	}
	return energy / pixelCount;
}
