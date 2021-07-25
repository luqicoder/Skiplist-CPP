#include <pthread.h>
#include <string.h>
#include <iostream>
#include <random>
#include <chrono>
#include "../skiplist.h"
void *thread_main(void *arg);

SkipList<int, std::string> skipList(20);
#define TEST_COUNT 100000

void *insertElement(void* arg) 
{
	int n = *((int *)arg);
	int cnt = TEST_COUNT * n;

	std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, cnt - 1);
	//std::cout << dist6(rng) << std::endl;

	for (int i=0; i < cnt; i++) {
		//skipList.insert_element(i, "a"); 
		skipList.insert_element(dist6(rng), "a"); 
	}
	std::cout << "skiplist size()=" << skipList.size() << std::endl;
	return NULL;
}

void *getElement(void* arg) 
{
	int n = *((int *)arg);
	int cnt = TEST_COUNT * n;

	std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, cnt - 1);

	for (int i=0; i<cnt; i++) {
		skipList.search_element(dist6(rng)); 
	}
    pthread_exit(NULL);
}

void stress_test_insert(int n = 1)
{
	pthread_t t_id;

	auto start = std::chrono::high_resolution_clock::now();

	if (pthread_create((&t_id), NULL, insertElement, (void*)&n)!=0)
	{
		std::cout << "pthread_create() error";
	}

	if (pthread_join(t_id, NULL) !=0)
	{
		std::cout << "pthread_join() error";
	}

	auto finish = std::chrono::high_resolution_clock::now(); 
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "insert elapsed:" << elapsed.count() << std::endl;
}

void stress_test_read(int n = 1)
{
	pthread_t t_id;
	insertElement((void *)&n);

	auto start = std::chrono::high_resolution_clock::now();

	if (pthread_create((&t_id), NULL, getElement, (void*)&n)!=0)
	{
		std::cout << "pthread_create() error";
	}

	if (pthread_join(t_id, NULL) !=0)
	{
		std::cout << "pthread_join() error";
	}

	auto finish = std::chrono::high_resolution_clock::now(); 
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "get elapsed:" << elapsed.count() << std::endl;
}

int main()
{
	//stress_test_insert(1);
	stress_test_read(10);
	return 0;
}
