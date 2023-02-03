#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memory_manager.h"

#define TSTART_MY_HEAP_SIZE 100

int TStart()
{
	char my_heap[TSTART_MY_HEAP_SIZE];
	mmInit(my_heap, TSTART_MY_HEAP_SIZE);

	printf("1 -- Available Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_fragment_count());

	// Allocate 10 bytes
	//   shouldn't fail
	char* ptr1 = mymalloc_ff(10);
	if(ptr1 == NULL) {
		printf("ptr1 - mymalloc_ff(10) failed\n");
		exit(EXIT_FAILURE);
	}
	strncpy(ptr1, "HELLO", 10);
	printf("ptr1 is %s\n", ptr1);

	printf("2 -- Available Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_fragment_count());

	// Allocate 45 bytes
	//   shouldn't fail
	char* ptr2 = mymalloc_wf(45);
	if(ptr2 == NULL) {
		printf("ptr2 - mymalloc_wf(45) failed\n");
		exit(EXIT_FAILURE);
	}
	strncpy(ptr2, "GOODBYE", 45);
	printf("ptr2 is %s\n", ptr2);

	printf("3 -- Available Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_fragment_count());

	// Attempt to allocate 50 bytes
	//   should fail
	char* ptr3 = mymalloc_bf(50);
	if(ptr3 == NULL) {
		printf("ptr3 - mymalloc_bf(50) failed\n");
	}
	printf("4 -- Available Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_fragment_count());

	// Free the first two pointers
	myfree(ptr1);
	myfree(ptr2);
	printf("5 -- Available Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_fragment_count());

	printf("Total successful mallocs: %d\n", get_mymalloc_count());

	// Double free, should cause a segmentation fault
	myfree(ptr2);

	mmDestroy();

	return 0;
}


#define TC0_MY_HEAP_SIZE 100

//TC0
int T0()
{
	printf("TC0: init and destroy\n");

	char my_heap[TC0_MY_HEAP_SIZE];
	mmInit(my_heap, TC0_MY_HEAP_SIZE);

	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	printf("Total successful mallocs: %d\n", get_mymalloc_count());

	mmDestroy();

	return 0;
}



//TC1
#define TC1_MY_HEAP_SIZE 100

int T1()
{
	char my_heap[TC1_MY_HEAP_SIZE];
	mmInit(my_heap, TC1_MY_HEAP_SIZE);

	printf("TC1: Single allocate with mymalloc_ff\n");
	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	// Allocate 10 bytes
	//   shouldn't fail
	char* ptr1 = mymalloc_ff(10);
	if(ptr1 == NULL) {
		printf("ptr1 - mymalloc_ff(10) failed\n");
		exit(EXIT_FAILURE);
	}
	strncpy(ptr1, "HELLO", 10);
	printf("ptr1 is %s\n", ptr1);

	printf("2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	myfree(ptr1);
	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC2
#define TC2_MY_HEAP_SIZE 100

int T2()
{
	char my_heap[TC2_MY_HEAP_SIZE];
	mmInit(my_heap, TC2_MY_HEAP_SIZE);

	printf("TC2: Single allocate with mymalloc_bf\n");
	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	// Allocate 10 bytes
	//   shouldn't fail
	char* ptr1 = mymalloc_bf(10);
	if(ptr1 == NULL) {
		printf("ptr1 - mymalloc_ff(10) failed\n");
		exit(EXIT_FAILURE);
	}
	strncpy(ptr1, "HELLO", 10);
	printf("ptr1 is %s\n", ptr1);

	printf("2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	myfree(ptr1);
	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}


//TC3
#define TC3_MY_HEAP_SIZE 100

int T3()
{
	char my_heap[TC3_MY_HEAP_SIZE];
	mmInit(my_heap, TC3_MY_HEAP_SIZE);

	printf("TC3: Single allocate with mymalloc_wf\n");
	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	// Allocate 10 bytes
	//   shouldn't fail
	char* ptr1 = mymalloc_wf(10);
	if(ptr1 == NULL) {
		printf("ptr1 - mymalloc_ff(10) failed\n");
		exit(EXIT_FAILURE);
	}
	strncpy(ptr1, "HELLO", 10);
	printf("ptr1 is %s\n", ptr1);

	printf("2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	myfree(ptr1);
	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC4
#define TC4_MY_HEAP_SIZE 100

int T4()
{
	char my_heap[TC4_MY_HEAP_SIZE];
	mmInit(my_heap, TC4_MY_HEAP_SIZE);

	printf("TC4: Large allocation should fail\n");
	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	// Allocate 10 bytes
	//   shouldn't fail
	char* ptr1 = mymalloc_ff(200);
	if(ptr1 != NULL) {
		printf("ptr1 - mymalloc_ff(10) succeeded\n");
		myfree(ptr1);
	}

	printf("2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC5
#define TC5_MY_HEAP_SIZE 4096
#define TC5_ALLOC_COUNT 100
#define TC5_ALLOC_SIZE 10

int T5()
{
	char my_heap[TC5_MY_HEAP_SIZE];
	mmInit(my_heap, TC5_MY_HEAP_SIZE);
	void* ptr[TC5_ALLOC_COUNT];

	printf("TC5: Heap size: %d: allocation count: %d, allocation size: %d - Lots of small allocationsl\n", TC5_MY_HEAP_SIZE, TC5_ALLOC_COUNT, TC5_ALLOC_SIZE);

	for(int i = 0 ; i < TC5_ALLOC_COUNT; i++) {

		printf("%d - 1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", i, get_remaining_space(), get_allocated_space(), get_fragment_count());

		ptr[i] = mymalloc_ff(TC5_ALLOC_SIZE);
		if(ptr[i] == NULL) {
			printf("ptr[%d] - mymalloc_ff(%d) failed\n", i, TC5_ALLOC_SIZE);
			exit(EXIT_FAILURE);
		}

		printf("%d 2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", i, get_remaining_space(), get_allocated_space(), get_fragment_count());
	}
	for(int i = 0; i < TC5_ALLOC_COUNT; i++) {
		printf("Freeing ptr[%d]\n", i);
		myfree(ptr[i]);
	}

	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC6
#define TC6_MY_HEAP_SIZE 4096
#define TC6_ALLOC_COUNT 100
#define TC6_ALLOC_SIZE 10

int T6()
{
	char my_heap[TC6_MY_HEAP_SIZE];
	mmInit(my_heap, TC6_MY_HEAP_SIZE);
	void* ptr[TC6_ALLOC_COUNT];

	printf("TC6: Heap size: %d: allocation count: %d, allocation size: %d - Lots of small allocations with interleaved frees\n", TC6_MY_HEAP_SIZE, TC6_ALLOC_COUNT, TC6_ALLOC_SIZE);

	for(int i = 0 ; i < TC6_ALLOC_COUNT; i++) {

		printf("%d - 1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", i, get_remaining_space(), get_allocated_space(), get_fragment_count());

		ptr[i] = mymalloc_ff(TC6_ALLOC_SIZE);
		if(ptr[i] == NULL) {
			printf("ptr[%d] - mymalloc_ff(%d) failed\n", i, TC6_ALLOC_SIZE);
			exit(EXIT_FAILURE);
		}

		printf("%d 2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", i, get_remaining_space(), get_allocated_space(), get_fragment_count());
	}
	for(int i = 0; i < TC6_ALLOC_COUNT; i += 2) {
		printf("Freeing ptr[%d], Fragment Count: %d\n", i, get_fragment_count());
		myfree(ptr[i]);
	}
	for(int i = 1; i < TC6_ALLOC_COUNT; i += 2) {
		myfree(ptr[i]);
		printf("Freeing ptr[%d], Fragment Count: %d\n", i, get_fragment_count());
	}

	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC7
#define TC7_MY_HEAP_SIZE 100

int T7()
{
	char my_heap[TC7_MY_HEAP_SIZE];
	mmInit(my_heap, TC7_MY_HEAP_SIZE);

	printf("TC7: Test interleaved allocations FF\n");
	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	void* ptr1 = mymalloc_ff(20);
	printf("2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr2 = mymalloc_ff(10);
	printf("3 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr3 = mymalloc_ff(10);
	printf("4 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	if(ptr1 == NULL) {
		printf("Unable to allocate ptr1\n");
	} else {
		myfree(ptr1);
	}
	if(ptr3 == NULL) {
		printf("Unable to allocate ptr3\n");
	} else {
		myfree(ptr3);
	}
	printf("5 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	void* ptr4 = mymalloc_ff(10);
	printf("6 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr5 = mymalloc_ff(20);
	printf("7 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	if(ptr4 == NULL) {
		printf("Unable to allocate ptr1\n");
	} else {
		myfree(ptr4);
	}

	if(ptr2 == NULL) {
		printf("Unable to allocate ptr2\n");
	} else {
		myfree(ptr2);
	}
	if(ptr5 == NULL) {
		printf("Unable to allocate ptr5\n");
	} else {
		myfree(ptr5);
	}

	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC8
#define TC8_MY_HEAP_SIZE 100

int T8()
{
	char my_heap[TC8_MY_HEAP_SIZE];
	mmInit(my_heap, TC8_MY_HEAP_SIZE);

	printf("TC8: Test interleaved allocations BF\n");
	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	void* ptr1 = mymalloc_bf(20);
	printf("2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr2 = mymalloc_bf(10);
	printf("3 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr3 = mymalloc_bf(10);
	printf("4 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	if(ptr1 == NULL) {
		printf("Unable to allocate ptr1\n");
	} else {
		myfree(ptr1);
	}
	if(ptr3 == NULL) {
		printf("Unable to allocate ptr3\n");
	} else {
		myfree(ptr3);
	}
	printf("5 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	void* ptr4 = mymalloc_bf(10);
	printf("6 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr5 = mymalloc_bf(20);
	printf("7 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	if(ptr4 == NULL) {
		printf("Unable to allocate ptr1\n");
	} else {
		myfree(ptr4);
	}

	if(ptr2 == NULL) {
		printf("Unable to allocate ptr2\n");
	} else {
		myfree(ptr2);
	}
	if(ptr5 == NULL) {
		printf("Unable to allocate ptr5\n");
	} else {
		myfree(ptr5);
	}

	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC9
#define TC9_MY_HEAP_SIZE 100

int T9()
{
	char my_heap[TC9_MY_HEAP_SIZE];
	mmInit(my_heap, TC9_MY_HEAP_SIZE);

	printf("TC9: Test interleaved allocations WF\n");
	printf("1 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	void* ptr1 = mymalloc_wf(20);
	printf("2 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr2 = mymalloc_wf(10);
	printf("3 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr3 = mymalloc_wf(10);
	printf("4 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	if(ptr1 == NULL) {
		printf("Unable to allocate ptr1\n");
	} else {
		myfree(ptr1);
	}
	if(ptr3 == NULL) {
		printf("Unable to allocate ptr3\n");
	} else {
		myfree(ptr3);
	}
	printf("5 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());

	void* ptr4 = mymalloc_wf(10);
	printf("6 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	void* ptr5 = mymalloc_wf(20);
	printf("7 -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", get_remaining_space(), get_allocated_space(), get_fragment_count());
	if(ptr4 == NULL) {
		printf("Unable to allocate ptr1\n");
	} else {
		myfree(ptr4);
	}

	if(ptr2 == NULL) {
		printf("Unable to allocate ptr2\n");
	} else {
		myfree(ptr2);
	}
	if(ptr5 == NULL) {
		printf("Unable to allocate ptr5\n");
	} else {
		myfree(ptr5);
	}

	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}



//TC10
#define TC10_MY_HEAP_SIZE 4096
#define TC10_MUL_COUNT 100

int* heap_multiply(int a, int b)
{
	int* result = mymalloc_ff(sizeof(int));
	if(result == NULL) {
		printf("COULD NOT mymalloc_ff an int\n");
		exit(EXIT_FAILURE);
	}
	*result = a * b;
	return result;
}

int T10()
{
	int* results[TC10_MUL_COUNT];
	char my_heap[TC10_MY_HEAP_SIZE];
	mmInit(my_heap, TC10_MY_HEAP_SIZE);

	printf("TC10: allocate multiple integers and perform multiplications\n");

	for(int i = 0; i < TC10_MUL_COUNT; i++) {
		printf("%d -- Available Memory: %d, Alloc Memory: %d, Fragment Count: %d\n", i, get_remaining_space(), get_allocated_space(), get_fragment_count());
		results[i] = heap_multiply(i, i + 2);
	}

	for(int i = 0; i < TC10_MUL_COUNT; i++) {
		printf("%d * %d = %d\n", i, i+2, *results[i]);
	}

	for(int i = 0; i < TC10_MUL_COUNT; i++) {
		myfree(results[i]);
	}

	printf("Total successful mallocs: %d\n", get_mymalloc_count());
	mmDestroy();

	return 0;
}




int main()
{
	int breakFromLoop = 1;
	printf("NOTE: Test case -1 is the original test case for this file which intentionally SEGFAULTs");
	while(breakFromLoop)
	{
		int testCase;
		printf("\nEnter a test case (-1-10), or any other int to quit:\n");
		scanf("%d",&testCase);

		switch(testCase)
		{
			case -1:
				TStart();
				break;
			case 0:
				T0();
				break;
			case 1:
				T1();
				break;
			case 2:
				T2();
				break;
			case 3:
				T3();
				break;
			case 4:
				T4();
				break;
			case 5:
				T5();
				break;
			case 6:
				T6();
				break;
			case 7:
				T7();
				break;
			case 8:
				T8();
				break;
			case 9:
				T9();
				break;
			case 10:
				T10();
				break;
			default:
				breakFromLoop = 0;
				break;
		}
	}

	return 0;
}
