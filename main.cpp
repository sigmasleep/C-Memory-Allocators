#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
//#ifdef WINDOWS
#include <windows.h>

//#endif
#define internal static
#define global static
#define local_persist static


struct memory
{
  unsigned int Size;
  void *AllocatedSpace;
};

#include "stack_allocator.cpp"
#include "list_allocator.cpp"

#include <time.h>
#include <limits.h>

static void
SubtractLargeIntegers(LARGE_INTEGER *X, LARGE_INTEGER *Y, LARGE_INTEGER *Result)
{
  Result->HighPart = X->HighPart - Y->HighPart;
  Result->LowPart = X->LowPart - Y->LowPart;

  //if((Y->LowPart < X->LowPart || Y->LowPart == X->LowPart) && Y->HighPart < X->HighPart
  //   && Result->LowPart < )
  //{

  //}
}

int
main()
{
  LARGE_INTEGER PerformanceCounterFrequency;
  LARGE_INTEGER OldTime;
  LARGE_INTEGER NewTime;

  QueryPerformanceFrequency(&PerformanceCounterFrequency);

  srand((unsigned int)time(NULL));
#define TestCount 20
  stack Stack;

  QueryPerformanceCounter(&OldTime);
  InitializeStack(&Stack, TestCount * (sizeof(long)+alignof(long)));
  QueryPerformanceCounter(&NewTime);

  LARGE_INTEGER StackInitializationTime;
  SubtractLargeIntegers(&NewTime, &OldTime, &StackInitializationTime);

  unsigned char *CharArray[TestCount];
  int CharArraySize = 0;
  unsigned int *IntArray[TestCount];
  int IntArraySize = 0;
  unsigned long *LongArray[TestCount];
  int LongArraySize = 0;

  int TestChoiceOrder[TestCount];

  printf("Stack Allocation Starting\n");
  printf("\t\t\tCurrent Top Address: %u\t", (size_t)Stack.TopOfMemory);
  printf("Space Remaining : %d\n", Stack.SpaceRemaining);

  QueryPerformanceCounter(&OldTime);

  for(int TestIndex = 0; TestIndex < TestCount; ++TestIndex)
  {
    int TestChoice = rand() % 3;
    TestChoiceOrder[TestIndex] = TestChoice;
    switch(TestChoice)
    {
      case 0:
      {
        CharArray[CharArraySize] = (unsigned char *)AllocateSpaceOnStack(&Stack, char);
        //*CharArray[CharArraySize] = (char)(rand() % UCHAR_MAX);
        *CharArray[CharArraySize] = (unsigned char)(CharArraySize);
        ++CharArraySize;
        printf("-Allocated(char) \t");
      } break;
      case 1:
      {
        IntArray[IntArraySize] = (unsigned int *)AllocateSpaceOnStack(&Stack, int);
        //*IntArray[IntArraySize] = (int)(rand() % UINT_MAX);
        *IntArray[IntArraySize] = (unsigned int)(IntArraySize);
        ++IntArraySize;
        printf("-Allocated(int) \t");
      } break;
      case 2:
      {
        LongArray[LongArraySize] = (unsigned long *)AllocateSpaceOnStack(&Stack, long);
        //*LongArray[LongArraySize] = (long)(rand() % ULONG_MAX);
        *LongArray[LongArraySize] = (unsigned long)(LongArraySize);
        ++LongArraySize;
        printf("-Allocated(long) \t");
      } break;
    }
    printf("Current Top Address: %u\t", (size_t)Stack.TopOfMemory);
    printf("Space Remaining : %d\t", Stack.SpaceRemaining);
    if(Stack.LastAlignmentIsHeader)
    {
      printf("Last Alignment Is Header\n");
    }
    else
    {
      printf("Last Alignment Is Footer\n");
    }
  }

  QueryPerformanceCounter(&NewTime);
  LARGE_INTEGER StackAllocationTime;
  SubtractLargeIntegers(&NewTime, &OldTime, &StackAllocationTime);

  printf("\nChars:\n");
  for(int CharArrayIndex = 0; CharArrayIndex < CharArraySize; ++CharArrayIndex)
  {
    printf("[%i]:%d\t", CharArrayIndex, *CharArray[CharArrayIndex]);
  }
  printf("\n\nInts:\n");
  for(int IntArrayIndex = 0; IntArrayIndex < IntArraySize; ++IntArrayIndex)
  {
    printf("[%i]:%d\t", IntArrayIndex, *IntArray[IntArrayIndex]);
  }
  printf("\n\nLongs:\n");
  for(int LongArrayIndex = 0; LongArrayIndex < LongArraySize; ++LongArrayIndex)
  {
    printf("[%i]:%d\t", LongArrayIndex, *LongArray[LongArrayIndex]);
  }

  printf("\n\n");
  printf("Stack Deallocation Starting\n");
  printf("\t\t\tCurrent Top Address: %u\t", (size_t)Stack.TopOfMemory);
  printf("Space Remaining : %d\t", Stack.SpaceRemaining);
  if(Stack.LastAlignmentIsHeader)
  {
    printf("Last Alignment Is Header\n");
  }
  else
  {
    printf("Last Alignment Is Footer\n");
  }

  QueryPerformanceCounter(&OldTime);

  for(int TestIndex = TestCount - 1; TestIndex >= 0; --TestIndex)
  {
    switch(TestChoiceOrder[TestIndex])
    {
      case 0:
      {
        DeallocateSpaceOnStack(&Stack, char);
        printf("-Deallocated(char)\t");
      } break;
      case 1:
      {
        DeallocateSpaceOnStack(&Stack, int);
        printf("-Deallocated(int)\t");
      } break;
      case 2:
      {
        DeallocateSpaceOnStack(&Stack, long);
        printf("-Deallocated(long)\t");
      } break;
    }
    printf("Current Top Address: %u\t", (size_t)Stack.TopOfMemory);
    printf("Space Remaining : %d\t", Stack.SpaceRemaining);
    if(Stack.LastAlignmentIsHeader)
    {
      printf("Last Alignment Is Header\n");
    }
    else
    {
      printf("Last Alignment Is Footer\n");
    }
  }  

  QueryPerformanceCounter(&NewTime);
  LARGE_INTEGER StackDeallocationTime;
  SubtractLargeIntegers(&NewTime, &OldTime, &StackDeallocationTime);

  //double StackInitializationTimeInMS = (StackInitializationTime * 1000.f) / PerformanceCounterFrequency;
  //double StackAllocationTimeInMS = (StackAllocationTime * 1000.f) / PerformanceCounterFrequency);
  //double StackDeallocationTimeInMS = (StackDeallocationTime * 1000.f) / PerformanceCounterFrequency);

  printf("Stack Initialization Time: %fms [%u %u]\n", StackInitializationTime.LowPart * 1000.f / PerformanceCounterFrequency.LowPart,
         StackInitializationTime.HighPart, StackInitializationTime.LowPart);
  printf("Stack Allocation Time: %fms [%u %u]\n", StackAllocationTime.LowPart * 1000.f / PerformanceCounterFrequency.LowPart,
         StackAllocationTime.HighPart, StackAllocationTime.LowPart);
  printf("Stack Deallocation Time: %fms [%u %u]\n", StackDeallocationTime.LowPart * 1000.f / PerformanceCounterFrequency.LowPart,
         StackDeallocationTime.HighPart, StackDeallocationTime.LowPart);
  printf("QueryPerformanceFrequency: [%u %u]\n", PerformanceCounterFrequency.HighPart, PerformanceCounterFrequency.LowPart);
  printf("\n\n\n");

  CharArraySize = 0;
  IntArraySize = 0;
  LongArraySize = 0;

  list List;
  InitializeList(&List, TestCount * (sizeof(long) + alignof(long)) * 2);
  printf("Current Top Address: %u\n", (size_t)List.Head);

  for (int TestIndex = 0; TestIndex < TestCount; ++TestIndex)
  {
    int TestChoice = 0;//rand() % 3;

    switch (TestChoice)
    {
      case 0:
      {
        CharArray[CharArraySize] = (unsigned char *)AllocateSpaceOnList(&List, char);
        if (CharArray[IntArraySize] == NULL) {
          break;
        }
        //*CharArray[CharArraySize] = (char)(rand() % UCHAR_MAX);
        *CharArray[CharArraySize] = (unsigned char)(CharArraySize);
        printf("-Allocated(char)\tA:%u\t", (size_t)CharArray[CharArraySize]);
        printf("Size Allocated : %u\t", GetSizeOfAllocation(CharArray[CharArraySize]));
        printf("Padding : %u\t", GetPaddingOfAllocation(CharArray[CharArraySize]));
        ++CharArraySize;
      } break;
      case 1:
      {
        IntArray[IntArraySize] = (unsigned int *)AllocateSpaceOnList(&List, int);
        if (IntArray[IntArraySize] == NULL) {
          break;
        }
        //*IntArray[IntArraySize] = (int)(rand() % UINT_MAX);
        *IntArray[IntArraySize] = (unsigned int)(IntArraySize);
        printf("-Allocated(int) \tA:%u\t", (size_t)IntArray[IntArraySize]);
        printf("Size Allocated : %u\t", GetSizeOfAllocation(IntArray[IntArraySize]));
        printf("Padding : %u\t", GetPaddingOfAllocation(IntArray[IntArraySize]));
        ++IntArraySize;
      } break;
      case 2:
      {
        LongArray[LongArraySize] = (unsigned long *)AllocateSpaceOnList(&List, long);
        if (LongArray[LongArraySize] == NULL) {
          break;
        }
        //*LongArray[LongArraySize] = (long)(rand() % ULONG_MAX);
        *LongArray[LongArraySize] = (unsigned long)(LongArraySize);
        printf("-Allocated(long)\tA:%u\t", (size_t)LongArray[LongArraySize]);
        printf("Size Allocated : %u\t", GetSizeOfAllocation(LongArray[LongArraySize]));
        printf("Padding : %u\t", GetPaddingOfAllocation(LongArray[LongArraySize]));
        ++LongArraySize;
      } break;
    }
    
    printf("Space Remaining : %d\n", List.SpaceRemaining);
  }
  
  printf("\nChars:\n");
  for (int CharArrayIndex = 0; CharArrayIndex < CharArraySize; ++CharArrayIndex)
  {
    printf("[%i]:%d\t", CharArrayIndex, *CharArray[CharArrayIndex]);
  }
  printf("\n\nInts:\n");
  for (int IntArrayIndex = 0; IntArrayIndex < IntArraySize; ++IntArrayIndex)
  {
    printf("[%i]:%d\t", IntArrayIndex, *IntArray[IntArrayIndex]);
  }
  printf("\n\nLongs:\n");
  for (int LongArrayIndex = 0; LongArrayIndex < LongArraySize; ++LongArrayIndex)
  {
    printf("[%i]:%d\t", LongArrayIndex, *LongArray[LongArrayIndex]);
  }

  printf("\n\n");

  for (int TestIndex = TestCount - 1; TestIndex >= 0; --TestIndex)
  {
    int TestChoice = 0;//rand() % 3;
    
    switch (TestChoice)
    {
      case 0:
      {
        if (CharArraySize > 0)
        {
          DeallocateSpaceOnList(&List, CharArray[CharArraySize-1]);
          CharArraySize--;
          printf("-Deallocated(char)\t");
          printf("Space Remaining : %d\n", List.SpaceRemaining);
        }
      } break;
      case 1:
      {
        if (IntArraySize > 0)
        {
          DeallocateSpaceOnList(&List, IntArray[IntArraySize-1]);
          IntArraySize--;
          printf("-Deallocated(int)\t");
          printf("Space Remaining : %d\n", List.SpaceRemaining);
        }
      } break;
      case 2:
      {
        if (LongArraySize > 0)
        {
          DeallocateSpaceOnList(&List, LongArray[LongArraySize-1]);
          LongArraySize--;
          printf("-Deallocated(long)\t");
          printf("Space Remaining : %d\n", List.SpaceRemaining);
        }
      } break;
    }
  }

  return 0;
}