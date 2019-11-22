#include <stdio.h>
#include <stdlib.h> // Import "EXIT_FAILURE" & "EXIT_SUCCESS"
#include <string.h> // Import "strtok"
#include <math.h>

#define EXIT_FLAG -100 // flag for failed input from func
#define LINE_MAX_SIZE 1024 //Defines the max line length
#define ARRAY_DELIM " \t\r\n" //The delimiters used to separate the array input
#define NUM_DELIM "\n" //The delimiters used to separate the num input
#define DOUBLE_MAX_SIZE 9 //Defines the max num size, including floating point
#define MAX_ARRAY_VALUES 100
#define DOT '.'
#define EPSILON 0.00000001
#define MAX_STARS 20

//Digit ranges
#define LOW_DIGIT_RANGE 0
#define HIGH_DIGIT_RANGE 9

/**
 * locate the floating point in double if exists
 * @param val given str
 * @return floating point index if exists, else return str length
 */
int findFloatingPoint(char *val)
{
    int dotIndex = (int) strlen(val);
    for (int i = 0; i < (int) strlen(val); ++i)
    {
        if (val[i] == DOT)
        {
            //dot already found in num || dot is the last char
            if (dotIndex < strlen(val) || i == strlen(val) - 1)
            {
                return EXIT_FLAG;
            }
            else
            {
                dotIndex = i;
            }
        }
    }
    return dotIndex;
}

/**
 * validate char is num 0-9
 * @param c char to convert
 * @return num. if char is not num- exit
 */
int char2Num(char c)
{
    int num = c - '0';
    if (num > HIGH_DIGIT_RANGE || num < LOW_DIGIT_RANGE)
    {
        return EXIT_FLAG;
    }
    else
    {
        return num;
    }
}

/**
 * calculating the factor - distance of num from floating point, in order to multiply/ divide by 0
 * according to num digit location within num.
 * @param numIndex location within given str
 * @param num
 * @param dotIndex
 * @return double value to add
 */
double calcAdd(int numIndex, int num, int dotIndex)
{
    double newNum = 0;

    if (numIndex == dotIndex - 1)
    { //the unity digit
        newNum = ((double) num);

    }
    else if (numIndex < dotIndex)
    {
        int factor = dotIndex - numIndex - 1;
        newNum = num * pow(10.0, (double) factor);
    }
    else if (numIndex > dotIndex)
    {
        int factor = numIndex - dotIndex;
        newNum = num / pow(10.0, (double) factor);
    }
    return (newNum);
}

/**
 * calculating the double value from str
 * @param str given str
 * @return double value, or exit if value is illegal
 */
double str2Double(char *str, int dotIndex)
{
    double temp = 0;

    int i = 0;
    int firstNum = char2Num(str[i]);

    if(firstNum == EXIT_FLAG)
    {
        return EXIT_FLAG;
    }

    //validate num is not starting with 0, unless it is 0 or 0.xx (for example, not 01.11)
    if (strlen(str) > 1 && dotIndex != 1 && firstNum == 0)
    {
        return EXIT_FLAG;
    }

    temp += calcAdd(0, firstNum, dotIndex);

    for (++i; i < (int) strlen(str); ++i)
    {
        if (i != dotIndex)
        {
            int num = char2Num(str[i]);

            if (num == EXIT_FLAG)
            {
                return EXIT_FLAG;
            }

            temp += calcAdd(i, num, dotIndex);
        }
    }
    return temp;
}

/**
 * get user input for samples array
 * @return 0 upon success, 1 otherwise.
 */
int getInputSamples(double *p, size_t *length)
{
    // Initializing the variables that'll contain the line information
    char line[LINE_MAX_SIZE];
    char *value = NULL;

    // Reads the line from the stdin
    if (fgets(line, LINE_MAX_SIZE, stdin) == NULL)
    {
        return EXIT_FLAG;
    }

    // read first value. If there's no input, we'll get "NULL"
    value = strtok(line, ARRAY_DELIM);

    // Handle every value and read the next value
    while (value != NULL)
    {

        // validate value's length
        if (strlen(value) > DOUBLE_MAX_SIZE)
        {
            return EXIT_FLAG;
        }

        // validate num
        int fpLocation = findFloatingPoint(value);
        if (fpLocation == EXIT_FLAG)
        {
            return EXIT_FLAG;
        }

        double num = str2Double(value, fpLocation);
        if (num == EXIT_FLAG)
        {
            return EXIT_FLAG;
        }

        // add num to current cell in array p
        *p = num;

        // add value to array's length counter
        (*length)++;
        if (*length > MAX_ARRAY_VALUES)
        {
            return EXIT_FLAG;
        }

        // Read the next value
        value = strtok(NULL, ARRAY_DELIM);
        p++;
    }
    return EXIT_SUCCESS;
}

/**
 * get user input for num of rooms.
 * @param roomNum
 * @return 0 upon success, 1 otherwise.
 */
int getInputNumOfRooms(int *roomNum)
{
    char line[LINE_MAX_SIZE];
    char *value = NULL;

    // Reads the line from the stdin
    if (fgets(line, LINE_MAX_SIZE, stdin) == NULL)
    {
        return EXIT_FLAG;
    }

    // read first and only value. If there's no input, we'll get "NULL"
    value = strtok(line, NUM_DELIM);

    //validate there's an input
    if (value == NULL)
    {
        return EXIT_FLAG;
    }
    else
    {
        //validate n is a natural number
        // (n do not have a floating point-> floating point index is strlen(value))
        int fpLocation = findFloatingPoint(value);
        if (fpLocation != strlen(value) || fpLocation == EXIT_FLAG)
        {
            return EXIT_FLAG;
        }
        else
        {
            int num = (int) str2Double(value, fpLocation);
            if (num == EXIT_FLAG)
            {
                return EXIT_FLAG;
            }
            *roomNum = num;
        }
    }

    //validate there's no more than 1 value provided
    value = strtok(NULL, NUM_DELIM);
    if (value != NULL)
    {
        return EXIT_FLAG;
    }
    return EXIT_SUCCESS;
}

/**
 * shift values to center. starting from the ending point of the new shifted array
 * @param p pointer to array
 * @param size of experiment data
 * @param last index (after centering the array)
 */
void centerArray(double *p, size_t size)
{
    int center = (int) ceil(size / 2.0);
    int lowerBound = (int) (floor(MAX_ARRAY_VALUES / 2.0) - center);
    int i = (int) (lowerBound + size - 1);
    int j = (int) (size - 1);

    for (; i >= lowerBound; --i, --j)
    {
        p[i] = p[j];
        p[j] = 0;
    }
}

/**
 * normalize array's values
 * @param p pointer to array
 */
void normalize(double *p)
{
    double sum = 0;

    //calc sum of values
    for (int j = 0; j <= MAX_ARRAY_VALUES; ++j)
    {
        sum += p[j];
    }

    //normalize values
    for (int i = 0; i <= MAX_ARRAY_VALUES; ++i)
    {
        p[i] = (p[i] / sum);
    }
}

/**
 * perform parametrization on given index
 * @param t orig index
 * @param sizeOfArr of array
 * @return new index
 */
int parametrize(int t, int sizeOfArr)
{
    return (int) (t + floor(sizeOfArr / 2.0));
}

/**
 *
 * @param result to be updated
 * @param tempArray which holds the current convolution values
 */
void updateResult(double *result, const double *tempArray)
{
    for(int i = 0; i < MAX_ARRAY_VALUES; ++i)
    {
            result[i] = tempArray[i];
    }
}

/**
 * perform single convolution [in O(n^2)]
 * @param arrA
 * @param arrB
 * @return new convoluted array
 */
void convolution(const double *arrA, const double *arrB, double *result)
{
    int halfM = (MAX_ARRAY_VALUES / 2);

    // initialize a temporary array, that will hold the convolution result
    double tempArr[MAX_ARRAY_VALUES] = {0};

    //calc convolution per each second
    int t = (int) (-ceil(halfM) - 1);
    for (; t < (int) (floor(halfM) - 1); t++)
    {
        double sum = 0;

        //calc sum
        int m = (int) - (ceil(halfM));
        for (; m <= (int) floor(halfM); m++)
        {
            int nIndexA = parametrize((t - m), MAX_ARRAY_VALUES);
            int nIndexB = parametrize(m, MAX_ARRAY_VALUES);

            //validate indices are in range
            if ((0 <= nIndexA && nIndexA <= MAX_ARRAY_VALUES) &&
                (0 <= nIndexB && nIndexB <= MAX_ARRAY_VALUES))
            {
                double add = (arrA[nIndexA] * arrB[nIndexB]);
                sum += add;
            }
        }

        // update temp
        int idx = t + halfM + 1;
        tempArr[idx] = sum;
    }

    // result update
    updateResult(result, tempArr);
    normalize(result);
}

/**
 * perform multi convolution: (((g*h)*h)*h)...  [n times].
 * @param g first array
 * @param h second array
 * @param n num of times to perform the convolution
 */
void multiConvolution(double *g, double *h, int n, double *result)
{
    // perform first convolution
    convolution(g, h, result);

    //perform single convolution n times
    int i = 1;
    while (i < n)
    {
        // convolution & normalize
        convolution(result, h, result);
        i++;
    }
}

/**
 * calculate num of stars that represent the value. scale is from 0 to 20,
 * where max value gets 20 stars.
 * @param value
 * @param max value
 * @return num of stars to be printed
 */
int numOfStars(double value, double max)
{
    if (value == max)
    {
        return MAX_STARS;
    }
    return (int) (floor((value / max) * MAX_STARS)) ;
}

/**
 * round values [precision of 3 num after the floating point]
 * @param result
 */
void roundResult(double *result)
{
    for (int i = 0; i < MAX_ARRAY_VALUES; ++i)
    {
        result[i] = ((round(1000 * result[i])) / 1000);
    }
}

/**
 *
 * @param result array of convolution
 * @return max value in array
 */
double findMax(const double *result)
{
    double max = 0;
    for (int i = 0; i < MAX_ARRAY_VALUES; ++i)
    {
        if (result[i] > max)
        {
            max = result[i];
        }
    }
    return max;
}

void printHistogram(double *result, double max)
{
    for (int i = 0; i < MAX_ARRAY_VALUES; i++)
    {
        printf("%0.3f: ", result[i]);

        //calc num of stars
        int j = numOfStars(result[i], max);

        //print stars
        while (j > 0)
        {
            printf("*");
            j--;
        }
        printf("\n");
    }
}

/**
 * The main function that runs the program
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated
 * multibyte strings that represent the arguments passed to the program from the execution
 * environment. The value of argv[argc] is guaranteed to be a null pointer.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has execution errors.
 */
int main(int argc, const char *argv[])
{
    //initializing the parameters needed for the experiment
    double g[MAX_ARRAY_VALUES] = {0}; //holds sound level per second
    double h[MAX_ARRAY_VALUES] = {0}; //holds amplification per second

    // size of arrays
    size_t lengthG = 0;
    size_t lengthH = 0;
    int roomNum = 0;

    // initialize result array, which will hold the multi convolution results
    double result[MAX_ARRAY_VALUES] = {0};

    // get g samples
    int gSamp = getInputSamples(g, &lengthG);
    if (gSamp == EXIT_FLAG)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }

    // get h samples
    int hSamp = getInputSamples(h, &lengthH);
    if (hSamp == EXIT_FLAG)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }

    //verify array's length
    if (lengthH > lengthG)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }

    // get num of rooms
    int n = getInputNumOfRooms(&roomNum);
    if (n == EXIT_FLAG)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }

    centerArray(g, lengthG);
    centerArray(h, lengthH);
    normalize(g);
    normalize(h);

    // perform convolution on non null arrays
    if (lengthG != 0 && lengthH != 0)
    {
        multiConvolution(g, h, roomNum, result);
    }

    roundResult(result);
    double max = findMax(result);

    // print histogram
    if (max > EPSILON)
    {
        printHistogram(result, max);
    }

    return EXIT_SUCCESS;
}