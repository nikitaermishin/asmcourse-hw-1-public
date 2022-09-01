#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * cmp_counter  - number of comparisons;
 * swap_counter - number of swaps;
 */

static int cmp_counter = 0;
static int swap_counter = 0;

/**
 * comparison function for qsort [stdlib.h] (direct order)
 * @param a - left array element pointer;
 * @param b - right array element pointer;
 * @return - number, which is:
 *  < 0 if element at a is greater than element at b;
 *  0 if elements has the same value;
 *  > 0 if element at a is less than element at b;
 */
static int qsort_cmp_d(const void *a, const void *b) {
    return *(long long int *)a - *(long long int *)b;
}

/**
 * comparison function for qsort [stdlib.h] (reversed order)
 * @param a - left compared element pointer;
 * @param b - right compared element pointer;
 * @return - number, which is:
 *  < 0 if element at a is less than element at b;
 *  0 if elements has the same value;
 *  > 0 if element at a is greater than element at b;
 */
static int qsort_cmp_r(const void *a, const void *b) {
    return *(long long int *)b - *(long long int *)a;
}

/**
 * direct order comparison function with cmp_counter incrementation
 * @param a - left compared element;
 * @param b - right compared element;
 * @return - number, which is:
 *  < 0 if element at a is greater than element at b;
 *  0 if elements has the same value;
 *  > 0 if element at a is less than element at b;
 */
static int _cmp(long long int a, long long int b) {
    cmp_counter++;
    return a - b;
}
/**
 * swaps values by pointers with swap_counter incrementation
 * @param a - first swap pointer;
 * @param b - second swap pointer;
 */
static void _swap(long long int *a, long long int *b) {
    swap_counter++;
    long long int tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * generates random parametrized array for sort testing
 *
 * @param param defines type of array:
 * 1 - random direct ordered array;
 * 2 - random reverse ordered array;
 * 3, 4 - random array;
 * @param n - number of elements in generated array;
 * @return pointer at the first element of array or NULL for allocation failure
 */
static long long int *gen_arr(int param, int n) {
    if (param < 1 || param > 4) {
        return NULL;
    }

    long long int *arr = calloc(n, sizeof (long long int));

    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }

    switch (param) {
        case 1:
            qsort(arr, n, sizeof(long long int), qsort_cmp_d);
            break;
        case 2:
            qsort(arr, n, sizeof(long long int), qsort_cmp_r);
            break;
        default:
            break;
    }

    return arr;
}

/**
 * performs selection sort on given array
 * @param n - array length
 * @param a - first element pointer
 */
static void selection_sort(int n, long long int *a) {
    long long int min;
    int min_i;
    for (int i = 0; i < n - 1; i++) {
        min_i = i;
        min = a[i];
        for (int j = i + 1; j < n; j++) {
            if (_cmp(a[j], min) < 0) {
                min_i = j;
                min = a[j];
            }
        }
        _swap(&(a[min_i]), &(a[i]));
    }
}

/**
 * recursive quick sort function
 * @param l - left boundary of array
 * @param r - right boundary of array
 * @param a - first element pointer
 */
static void qs_aux(int l, int r, long long int *a) {
    int i = l, j = r;
    long long int x = a[(l+r) / 2];
    do {
        while (_cmp(a[i], x) < 0) {
            i++;
        }
        while (_cmp(x, a[j]) < 0) {
            j--;
        }
        if (i <= j) {
            _swap(&(a[i]), &(a[j]));
            i++;
            j--;
        }
    } while (!(i > j));
    if (l < j) {
        qs_aux(l, j, a);
    }
    if (i < r) {
        qs_aux(i, r, a);
    }
}

/**
 * quick sort function (calls qs_aux)
 * @param n - array length
 * @param a - first element pointer
 */
static void quick_sort(int n, long long int *a) {
    qs_aux(0, n - 1, a);
}

/**
 * checks if a_sorted array is equal to sorted a_initial
 * @param a_initial - initial array pointer
 * @param a_sorted - sorted array pointer
 * @param n - length of arrays
 * @return 1 if a_sorted is sorted a_initial, 0 otherwise
 */
static int is_sorted(long long int *a_initial, long long int *a_sorted, int n) {
    int state = 1;

    for (int i = 1; i < n; i++) {
        if (!(a_sorted[i - 1] <= a_sorted[i])) {
            state = 0;
        }
    }

    int *buffer = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < n; j++) {
            if (a_initial[i] == a_sorted[j] && !buffer[j]) {
                buffer[j] = 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            state = 0;
            break;
        }
    }

    free(buffer);

    return state;
}
/**
 * copies array by first element pointer and length
 * @param n - initial array length
 * @param a - initial array first element pointer
 * @return new array first element pointer or NULL for allocation failure
 */
static long long int *cp_arr(int n, long long int *a) {
    long long int *b = malloc(sizeof (long long int) * n);
    for (int i = 0; i < n; i++) {
        b[i] = a[i];
    }
    return b;
}

int main (void) {
    int n;
    long long int *initial_arr[4], *selection_sort_arr[4], *quick_sort_arr[4];
    int quick_sort_cmp_arr[4], quick_sort_swap_arr[4], quick_sort_state_arr[4];
    int selection_sort_cmp_arr[4], selection_sort_swap_arr[4], selection_sort_state_arr[4];

    // validating input data
    if (scanf("%d", &n) == 0) {
        printf("Error occurred: unable to read parameter n");
        return 1;
    }

    // generating arrays
    srand(time(0));
    for (int i = 0; i < 4; i++) {
        initial_arr[i] = gen_arr(i+1, n);
    }

    // selection sorting with results saving
    for (int i = 0; i < 4; i++) {
        selection_sort_arr[i] = cp_arr(n, initial_arr[i]);

        cmp_counter = 0;
        swap_counter = 0;

        selection_sort(n, selection_sort_arr[i]);
        selection_sort_cmp_arr[i] = cmp_counter;
        selection_sort_swap_arr[i] = swap_counter;
        selection_sort_state_arr[i] = is_sorted(initial_arr[i], selection_sort_arr[i], n);
    }

    // quick sorting with results saving
    for (int i = 0; i < 4; i++) {
        quick_sort_arr[i] = cp_arr(n, initial_arr[i]);

        cmp_counter = 0;
        swap_counter = 0;

        quick_sort(n, quick_sort_arr[i]);
        quick_sort_cmp_arr[i] = cmp_counter;
        quick_sort_swap_arr[i] = swap_counter;
        quick_sort_state_arr[i] = is_sorted(initial_arr[i], quick_sort_arr[i], n);
    }

    // freeing dynamically allocated arrays
    for (int i = 0; i < 4; i++) {
        free(initial_arr[i]);
        free(selection_sort_arr[i]);
        free(quick_sort_arr[i]);
    }

    // printing results
    printf("Selection sort \n");
    printf("---------------------------------------------------------------------------------\n");
    for (int i = 0; i < 4; i++) {
        printf("| %d) Array №%d | %8d comparisons | %6d swaps | %1d - sorting result |\n", i+1, i+1, selection_sort_cmp_arr[i], selection_sort_swap_arr[i], selection_sort_state_arr[i]);
    }
    printf("---------------------------------------------------------------------------------\n");
    printf("Quick sort \n");
    printf("---------------------------------------------------------------------------------\n");
    for (int i = 0; i < 4; i++) {
        printf("| %d) Array №%d | %8d comparisons | %6d swaps | %1d - sorting result |\n", i+1, i+1, quick_sort_cmp_arr[i], quick_sort_swap_arr[i], quick_sort_state_arr[i]);
    }
    printf("---------------------------------------------------------------------------------\n");

    return 0;
}

