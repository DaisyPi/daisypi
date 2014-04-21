#include <stdio.h>
#include <stdlib.h>
#include <time.h> 


#define LARGE_NUM 500
//#define TESTING

#ifdef TESTING
    int array[LARGE_NUM] = {0};
#else
    float array[LARGE_NUM] = {0};
#endif

int binary_search(int len, int x) 
{
    int hi = len, lo = -1, mid;
    while (hi - lo > 1) 
    {
        mid = (lo + hi) / 2;
        if (array[mid] < x)
            lo = mid;
        else
            hi = mid;
    }
    
    if (hi == len)
        return -1;
    else
        return hi;
}

int main ()
{
    int i, j, value, nro = 0, nr = 0; 
    float median, aux, low = 2.0f, high = 2.0f, randm = -2.0f, randM = 5.0f;
#ifdef TESTING
    int output[LARGE_NUM] = {0};
#else
    float output[LARGE_NUM] = {0};
#endif

    srand (time(NULL));
    while (1)
    {
        /* Random generate a number of LARGE_NUM numbers */
        if (nr >= LARGE_NUM)
            break;
#ifdef TESTING
        array[nr] = rand() % 500;
        if (nr % 10 == 0)
            array[nr] = 0;
#else
        array[nr] = (float) (randm + (float)rand()/((float)RAND_MAX/(randM-randm)));
#endif
        /* Sort the elements */
        value = binary_search(nr, array[nr]);
        if (value != -1)
        {
            aux = array[nr];
            for (i = nr; i > value; i--)
                array[i] = array[i-1];
            array[value] = aux;
        }
        nr++;
    }

    if (nr % 2 == 0)
        median = (array[nr/2] + array[(nr/2)-1]) / 2;
    else
        median = (array[nr/2] + array[(nr/2)-1] + array[nr/2+1]) / 3;

    /* Remove values that are not in the established boundaries */
    int test_val = 0;
    for (i = 0; i < nr; i++)
    {
        if (array[i] > (int)median*high || array[i] < (int)median/low)
        {
            if (array[i] > (int)median*high)
                break;
            continue;
        }
        else
        {
            output[nro]=array[i];
            nro++;
        }
    }

    for (i = 0; i < nro; i++)
    {
        #ifdef TESTING
            printf("%d ", output[i]);
        #else
            printf("%f ", output[i]);  
        #endif
    }
    printf("\n");
    printf("\nThere are: %d elements.\n", nro);
	
    return 0;
}