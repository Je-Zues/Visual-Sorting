#include <iostream>
#include <Windows.h>
#include <vector>
#include <random>
#include <time.h>
#include "UI.h"

#define WIDTH  2000
#define HEIGHT 900


void swap(std::vector<int>& arr, int index1, int index2) {
	/* swaps 2 elements in the array */
	int tempVal = arr.at(index1);
	arr.at(index1) = arr.at(index2);
	arr.at(index2) = tempVal;
}

void VecPopulate(std::vector <int> &Nums, int amount) {
	/* empties the vector and inserts n amount of random values */
	Nums.empty();
	for (int i = 0; i < amount; i++) {
		Nums.push_back((std::rand() % 100000000 + 1));
	}
}

void VecShow(std::vector < int> &vec) {

	//loops through every element and printing it out 
	for (const int &x : vec) {
		std::cout << x << " ";
	}
	std::cout << "\n\n";
}

bool BubbleSort(std::vector < int>& Nums, int &cycle, int &swaps, int &compares) { //lowest to highest
	
	/* check if theres still items left to sort */
	if (cycle < Nums.size()) {
		/* loops through every number */
		for (int j = 0; j < Nums.size()-cycle-1; j++) {
			/* check if the next number is greater than current number */
			if (Nums.at(j) > Nums.at(j+1))	{
				/* swap the numbers and update swap tracker */
				swap(Nums, j + 1, j);
				swaps++;
			}
			/* updates compare tracker */
			compares++;
		}
		/* 1 iteration passes, updates tracker */
		cycle++;
		return false;
	}
	/* return true when sorted else false */
	return true;
}

bool SelectionSort(std::vector<int>& Nums, int& cycle, int& swaps, int& compares) {
	
	
	/* check if its done sorting */
	if (cycle < Nums.size()) {

		/* trackers for lowest number and its index */
		int tempLoc = cycle;
		

		/* loop through unsorted part of array searching for the lowest number */
		for (int j = cycle+1; j < Nums.size(); j++) {
			if (Nums.at(j) < Nums.at(tempLoc)) {
				tempLoc = j;
			}
			compares++;
		}

		/* if lowest number is not the first element in the unsorted array then swap */
		if (tempLoc > cycle) {
			swap(Nums, cycle, tempLoc);
			swaps++;
		}
		cycle++;
		return false;
	}
	/* return true if finished sorting else false */
	return true;
}

bool InsertionSort(std::vector<int>& Nums, int &cycle, int &swaps, int &compares) {
	/* first iteration doesnt change anything start it at 2nd */
	if (cycle == NULL)
		cycle = 1;
	
	/* check if done sorting */
	if (cycle < Nums.size()) {
		
		/* start on first element in unsorted array */
		for (int j = cycle; j > 0; j--) {
			compares++;
			/* work backwards into sorted array putting in place */
			if ((Nums.at(j) < Nums.at(j - 1))) {
				swap(Nums, j - 1, j);
				swaps++;
			}
			/* if nothing backwards is smaller then its in right place, break out of loop*/
			else {
				break;
			}
		}
		cycle++;
		return false;
	}
	/* return true if sorted else false */
	return true;
}

void CountSort(std::vector<int>& Nums,int exp, int& swaps) {

	constexpr uint16_t base = 10;
	std::vector<int>tempHolder = Nums;

	int count[base]{};

	for (auto x : Nums) {
		count[(x / exp)% base]++;
	}

	for (int i = 1; i < base;i++) {
		count[i] += count[i - 1];
	}

	for (int i = Nums.size()-1; i >= 0;i--) {
		//if (tempHolder[count[(Nums[i] / exp) % base] - 1] != Nums[i]) {
		tempHolder[count[(Nums[i] / exp) % base] - 1] = Nums[i];
			//swaps++;
		//}
		count[(Nums[i] / exp)% base]--;
	}

	Nums = tempHolder;

}

bool RadixSort(std::vector<int>& Nums,const int &maxNum, int& cycle, int& swaps, int& compares) {

	int exp = pow(10, cycle);
	if (maxNum / exp > 0) {
		CountSort(Nums, exp,swaps);
		cycle++;
		return false;
	}
	return true;
}

enum SORTMETHOD {
	BUBBLE,
	SELECTION,
	INSERTION,
	RADIX,
};

int main() {


	/* hides console */
	FreeConsole();

	/* generate random seed */
	srand(time(NULL));

	/* generates vectors with {NumAmount} of random numbers */
	constexpr size_t NumAmount = 100;
	std::vector <int> Numbers = {};

	VecPopulate(Numbers, NumAmount);
	
	/* creates a window object*/
	UI Wind;
	Wind.Init(L"Sorting Algorithims", 100, 100, WIDTH, HEIGHT);
	


	/* calculate width size and height of each box to appear on window */
	float widthBox = WIDTH / (float(Numbers.size()+floor(NumAmount/100)));

	int maxNum = *std::max_element(Numbers.begin(), Numbers.end());

	/* starts timer and sets delay between each loop */
	time_t startTime	{ time(NULL) };
	size_t runningTimer {};
	size_t delay		{ 1 };

	/* bool flag to check if algorithm has finished sorting */
	bool sorted = false;
	
	/* handles window messages */

	/* keeps track of sorting method */
	SORTMETHOD sm = BUBBLE;

	while (Wind.Update() ) {
		
		/* keep track of swaps, compares and where we are in the sorting*/
		static int sortCycle{}, swaps{}, compares{};
		
			/* sorts the vector with following methods updating trackers */
			/* *ONLY UNCOMMENT 1 AT A TIME* */

		//sorted = BubbleSort(Numbers, sortCycle, swaps, compares);
		//sorted = LinearSort(Numbers, sortCycle, swaps, compares);
		//sorted = SelectionSort(Numbers, sortCycle, swaps, compares);
		//sorted = RadixSort(Numbers, maxNum, sortCycle, swaps, compares);

			/* to sort by stepping through uncomment this section and algorithm*/
			/* will do 1 iteration each time UP arrow is pressed on keyboard */

		if (GetAsyncKeyState(VK_UP)&1) {

			sorted = BubbleSort(Numbers, sortCycle, swaps, compares);
			//sorted = InsertionSort(Numbers, sortCycle, swaps, compares);
			//sorted = SelectionSort(Numbers, sortCycle, swaps, compares);
			//sorted = RadixSort(Numbers, maxNum, sortCycle, swaps, compares);
		}
			/* while not sorted update the running timer */
		if (!sorted) {
			runningTimer = (size_t)difftime(time(NULL), startTime);
		}
	
			/* allows to start drawing in window and initialize a graphics object with the window buffer*/
		Wind.startDraw();
		Gdiplus::Graphics graphics(Wind.getBuffer());
		
			/* draws a rectangle for each number in the vector */

		for (int i = 0; i < Numbers.size(); i++) {
			/* color for boxes */
			Gdiplus::Color color = { 150,50,150 };
				/* each number is relative to the max number in the array */
				/* calculates height of the box to represent number */
			float boxHeight = (float(Numbers.at(i)) / maxNum) * HEIGHT;

			/* highlights box to visualize sorting */
			if ((i) == (Numbers.size() - sortCycle)&& sm == BUBBLE) {
				color = { 0,100,100 };
			}

			if ((i) == (sortCycle) && (sm == INSERTION || sm == SELECTION) ) {
				color = { 0,100,100 };
			}

				/* draws the box */
			Wind.fillRect({ widthBox * i, HEIGHT - boxHeight, widthBox,boxHeight }, color, graphics);//
				/* draws an outline to the box*/
			Wind.drawBox({ widthBox * i, HEIGHT - boxHeight, widthBox,boxHeight }, { 255,255,255 }, 2,graphics);
		}

		/* creates string to display trackers */
		std::wstring s_Timer	= L"Timer       : " + std::to_wstring(runningTimer) + L"s";
		std::wstring s_Swap		= L"Swaps      : " + std::to_wstring(swaps);
		std::wstring s_Compares = L"Compares: " + std::to_wstring(compares);
		std::wstring s_Delay	= L"Delay       : " + std::to_wstring(delay) + L"ms";
		std::wstring s_Size	    = L"Size          : " + std::to_wstring(NumAmount);


		/* draws the text onto the window */
		Wind.drawText(s_Compares.c_str(), { 0,0 }, { 255,255,255 }, 25,graphics);
		Wind.drawText(s_Swap.c_str(), { 0,25 },    { 255,255,255 }, 25,graphics);
		Wind.drawText(s_Timer.c_str(), { 0,50 },   { 255,255,255 }, 25,graphics);
		Wind.drawText(s_Delay.c_str(), { 0,75 },   { 255,255,255 }, 25,graphics);
		Wind.drawText(s_Size.c_str(), { 0,100 },   { 255,255,255 }, 25,graphics);

		/* finished drawing */
		Wind.stopDraw();

		Sleep(delay);
	}

	return 0;
}
