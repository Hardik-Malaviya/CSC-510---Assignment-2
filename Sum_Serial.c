#include <stdio.h>
#include <math.h> 

double Calculate_The_Sum(int n) {
    double pi = 0.0;
    int denominator = 1;
    int i;
    

    for (i = 0; i <=n; i++) {
        if (i % 2 == 0) 
        {
            pi += 4.0 / denominator;
        }  
        else {
            pi -= 4.0 / denominator;
        }
        denominator += 2;
    }

    return pi;
}



//Calculating Pi values:
double calculatePi(int n, int rank, int size) 
{
    double sum = 0.0;
    int i;

    for (i = rank; i <=n; i += size) 
    {
        double term = pow(-1.0, i) / (2.0 * i + 1.0);
        sum += term;
    }
    
    return sum;
}




int main() 
{
  
    int n;
    printf("Enter n:");
    fflush(stdout);
    scanf("%d",&n);
 
    double result = Calculate_The_Sum(n);

    printf("Serial Result of Sum_Formula (Pi), where n=%d is:  %.6f\n",n, result);

    return 0;
}
