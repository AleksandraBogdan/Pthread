#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include "CGSSet.h"
#include "LSSet.h"

#define NUM_ELEMENTS 10000

CGSSet<int>* set1 = NULL;
LSSet<int>* set2 = NULL;
int mode;


void* writeTest(void* parameter)
{
	for (int i = 1; i <= NUM_ELEMENTS; i++)
	{
		if (mode == 0)
			set1->add(i);
		else
			set2->add(i);
	}
 pthread_exit(0);
}


void* readTest(void* parameter)
{
	int* acc = reinterpret_cast<int*>(parameter);
	*acc = 0;
	for(int i = 1; i <= NUM_ELEMENTS; i++)
	{
		if(mode == 0)
		{
			if (set1->remove(i))
			{
				++(*acc);
				//set1->remove(i);
			}

		}
		else
		{
			if (set2->remove(i))
			{
				++(*acc);
				//set2->remove(i);
			}
		}
	}
	//std::cout << "acc1 = " << *acc1 << std::endl;
	 pthread_exit(0);
}

int manyWritersOneReader()
{
	set1 = new CGSSet<int>();
	set2 = new LSSet<int>();
	int test = 0;
	pthread_t threadRead1, threadWrite1, threadWrite2, threadWrite3;

	if (pthread_create(&threadWrite1, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
  	if (pthread_create(&threadWrite2, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
    if (pthread_create(&threadWrite3, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
    pthread_join(threadWrite1, NULL);
    pthread_join(threadWrite2, NULL);
    pthread_join(threadWrite3, NULL);

    if (pthread_create(&threadRead1, NULL, readTest, &test) != 0)
      std::cout << "problem with threads" <<std::endl;
	pthread_join(threadRead1, NULL);

	
    delete set1;
    delete set2;
    return test;
}

int manyWritersManyReaders()
{
	set1 = new CGSSet<int>();
	set2 = new LSSet<int>();
	int test1 = 0, test2 =0, test3 = 0, test4 = 0;
	pthread_t threadRead1, threadRead2, threadRead3, threadWrite1,  threadWrite2, threadWrite3, threadLast;

	if (pthread_create(&threadWrite1, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
  	if (pthread_create(&threadWrite2, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
    if (pthread_create(&threadWrite3, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
    pthread_join(threadWrite1, NULL);
    pthread_join(threadWrite2, NULL);
    pthread_join(threadWrite3, NULL);

    if (pthread_create(&threadRead1, NULL, readTest, &test1) != 0)
    	std::cout << "problem with threads" <<std::endl;
    if (pthread_create(&threadRead2, NULL, readTest, &test2) != 0)
    	std::cout << "problem with threads" <<std::endl;
	if (pthread_create(&threadRead3, NULL, readTest, &test3) != 0)
    	std::cout << "problem with threads" <<std::endl;


    pthread_join(threadRead1, NULL);
    pthread_join(threadRead2, NULL);
    pthread_join(threadRead3, NULL);

    if (pthread_create(&threadLast, NULL, readTest, &test4) != 0)
    	std::cout << "problem with threads" <<std::endl;
    pthread_join(threadLast, NULL);
   
    delete set1;
    delete set2;
    return test1 + test2 + test3 + test4;
}

int oneWriterManyReaders()
{
	set1 = new CGSSet<int>();
	set2 = new LSSet<int>();
	int test1 = 0, test2 =0, test3 = 0, test4;
	pthread_t threadRead1, threadRead2, threadRead3, threadWrite1, threadLast;

	if (pthread_create(&threadWrite1, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
    pthread_join(threadWrite1, NULL);

    if (pthread_create(&threadRead1, NULL, readTest, &test1) != 0)
    	std::cout << "problem with threads" <<std::endl;
  	if (pthread_create(&threadRead2, NULL, readTest, &test2) != 0)
    	std::cout << "problem with threads" <<std::endl;
	if (pthread_create(&threadRead3, NULL, readTest, &test3) != 0)
    	std::cout << "problem with threads" <<std::endl;


    pthread_join(threadRead1, NULL);
    pthread_join(threadRead2, NULL);
    pthread_join(threadRead3, NULL);
    if (pthread_create(&threadLast, NULL, readTest, &test4) != 0)
    	std::cout << "problem with threads" <<std::endl;
    pthread_join(threadLast, NULL);

    
    delete set1;
    delete set2;
    return test1 + test2 + test3 + test4;
}

int sameTime()
{
	set1 = new CGSSet<int>();
	set2 = new LSSet<int>();
	int test1 = 0,  test4 = 0, test2 = 0;
	pthread_t threadRead1, threadWrite1, threadLast, threadRead2, threadWrite2;

	if (pthread_create(&threadWrite1, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
    if (pthread_create(&threadWrite2, NULL, writeTest, NULL) != 0)
    	std::cout << "problem with threads" <<std::endl;
    if (pthread_create(&threadRead1, NULL, readTest, &test1) != 0)
    	std::cout << "problem with threads" <<std::endl;
    if (pthread_create(&threadRead2, NULL, readTest, &test2) != 0)
    	std::cout << "problem with threads" <<std::endl;


    pthread_join(threadWrite1, NULL);
    pthread_join(threadWrite2, NULL);
    pthread_join(threadRead1, NULL);
    pthread_join(threadRead2, NULL);



    if (pthread_create(&threadLast, NULL, readTest, &test4) != 0)
    	std::cout << "problem with threads" <<std::endl;
    pthread_join(threadLast, NULL);

    
    delete set1;
    delete set2;
    return test1 + test4 + test2;
}

int main()
{

    int  test1 = 0, test2 = 0, test3 = 0, test4 = 0;
    //test for CGS
	mode = 0;
    test1 = manyWritersOneReader();
    if (test1 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;

    test2 = oneWriterManyReaders();
    if (test2 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;

    test3 = manyWritersManyReaders();
    if (test3 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;

    test4 = sameTime();
    if (test4 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;

    //test for LS
    mode = 1;
    test1 = 0;
    test2 = 0;
    test3 = 0;
    test4 = 0;

    test1 = manyWritersOneReader();
    if (test1 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;

    test2 = oneWriterManyReaders();
    if (test2 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;

      test3 = manyWritersManyReaders();
    if (test3 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;
    test4 = sameTime();
    if (test4 == NUM_ELEMENTS)
    	std::cout << "Success!" << std::endl;
    else
    	std::cout << "Fault!" << std::endl;

    return 0;

}

