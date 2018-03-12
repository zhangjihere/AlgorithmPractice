#include<iostream>

using namespace std;

int main(int argc, char **argv) {
    int test_case;
    int T;
    /* 
    The freopen function below opens input.txt in read only mode and 
    sets your standard input to work with the opened file. 
    When you test your code with the sample data, you can use the function
    below to read in from the sample data file instead of the standard input.
    So. you can uncomment the following line for your local test. But you
    have to comment the following line when you submit for your scores.
    */

    freopen("input.txt", "r", stdin);
    cin >> T;

    /*
       Read each test case from standard input.
    */
    for (test_case = 1; test_case <= T; ++test_case) {

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*
            Please, implement your algorithm from this section.
        */
        /////////////////////////////////////////////////////////////////////////////////////////////
        int arr_len;
        cin >> arr_len;
        int A[arr_len];
        for (int i = 0; i < arr_len; i++) {
            cin >> A[i];
        }

        int sum[arr_len];
        sum[0] = A[0];
        for (int i = 1; i < arr_len; i++)
            sum[i] = sum[i - 1] + A[i];
        int maxSum = sum[0];
        int min = 0;
        for (int j = 1; j < arr_len; j++) {
            if (sum[j - 1] < min) min = sum[j - 1];
            if (sum[j] - min > maxSum) maxSum = sum[j] - min;
        }

        cout << "#" << test_case << " " << maxSum << endl;
    }
    return 0;//Your program should return 0 on normal termination.
}