//wordcount.cpp
//William Fisher
//Operating Systems
//Feb. 29, 2020

#include<iostream>
using std::cout;
using std::endl;
#include<vector>
using std::vector;
#include<fstream>
using std::ifstream;
#include<string>
using std::string;
#include<chrono>
#include<future>
#include<algorithm>
using std::sort;
using std::lower_bound;
#include<map>
using std::map;
#include<iomanip>
#include<iterator>
using std::advance;
#include<thread>
using std::thread;

vector<string> intoVector(string fileName)
{
	string input;
	vector<string> words;

	ifstream file(fileName);
	if (!file)
	{
		cout << "Error" << endl;
		return words;
	}

	while (true)
	{
		file >> input;
		if (!file)
		{
			if (file.eof())
				break;
			else
				cout << "Unkown error" << endl;
		}
		else
		{
			words.push_back(input);
		}
	}

	return words;
}

void countOccurrences(vector<string>& allWords, vector<string>& searchWords, map<string, int>& wordCount, int numThreads,
						int begin, int end)
{
	for (int i = begin; i < end; ++i)
	{
		for (int ii = 0; ii < searchWords.size(); ++ii)
		{
			if (allWords[i] == searchWords[ii])
			{
				wordCount[allWords[i]]++;
			}
		}
	}
}

int main()
{
	vector<string> allWords = intoVector("randomText.txt");
	
	//word bank
	vector<string> searchWords;
	bool inVector = false;
	for (int i = 0; i < allWords.size(); ++i)
	{
		//iterate through searchWords to check if word is there already
		for (int ii = 0; ii < searchWords.size(); ++ii)
		{
			if (searchWords[ii] == allWords[i])
			{
				inVector = true;
			}
		}
		//if word is not in searchWords already, add it
		if (!inVector)
		{
			searchWords.push_back(allWords[i]);
		}
		inVector = false;
	}

	//initialize the map
	map<string, int> wordCount;
	for (auto word : searchWords)
	{
		wordCount[word] = 0;
	}

	int numThreads = 2; //Adjust this number to change how many threads are being used
	vector<thread> threads;

	int begin = 0; //beginning of range for thread
	int end = (allWords.size() / numThreads); //end of range for thread

	auto start = std::chrono::high_resolution_clock::now(); //start the timer
	for (int i = 0; i < numThreads; ++i)
	{
		//if it is the last thread, have it deal with the remainder
		if (i == numThreads - 1)
			end += (allWords.size() % numThreads);

		thread t(countOccurrences, std::ref(allWords), std::ref(searchWords), std::ref(wordCount), numThreads, begin, end);
		
		threads.push_back(std::move(t));
		begin = end;
		end += (allWords.size() / numThreads);
	}

	for (int i = 0; i < threads.size(); ++i)
	{
		threads[i].join();
	}

	auto stop = std::chrono::high_resolution_clock::now(); //end the timer
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	for (auto word : searchWords)
	{
		cout << word << " : " << wordCount[word] << " times" << endl;
	}

	cout << "This program was run using " << numThreads << " threads." << endl;
	cout << "It took " << duration.count() << " microseconds for the threads to do their thing." << endl;
}

