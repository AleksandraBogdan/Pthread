#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include "CGSSet.h"

#define NUM_ELEMENTS 100

CGSSet<int>* p_set1 = NULL;
//LSSet<int>* p_set2 = NULL;


void* writeTest(void* parameter)
{
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		p_set1->add(i);
	}
 pthread_exit(0);
}

void* readTest(void* parameter)
{
	int* acc1 = reinterpret_cast<int*>(parameter);
	for(int i = 0; i < NUM_ELEMENTS; i++)
	{
		if (p_set1->contains(i))
			++(*acc1);

	}
	std::cout << "acc1 = " << *acc1 << std::endl;
	 pthread_exit(0);
}

int main()
{

	pthread_t threadRead1, threadWrite1;
	p_set1 = new CGSSet<int>();
  	//p_set2 = new LSSet<int>();

    int acc1 = 0;
    int acc2 = 0;
    int acc3 = 0;

    if (pthread_create(&threadWrite1, NULL, writeTest, NULL) != 0)
      std::cout << "problem with threads" <<std::endl;
   if (pthread_create(&threadRead1, NULL, readTest, &acc1) != 0)
      std::cout << "problem with threads" <<std::endl;

    int acc = acc1 + acc2 + acc3;

    std::cout << "acc1 = " << acc1 << std::endl;
    std::cout << "acc2 = " << acc2 << std::endl;
    std::cout << "acc3 = " << acc3 << std::endl;
    std::cout << "acc = " << acc << std::endl;

     pthread_join(threadWrite1, NULL);
     pthread_join(threadRead1, NULL);

    return 0;

}

