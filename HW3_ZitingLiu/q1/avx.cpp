#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace std;

#define TERMS 15
int fact(int x){
    int f=x;
    for(int i=x-1;i>0;i--){
        f*=i;
    }
    return f;
}

double td(int a){
    double x=(double)a;
    double result=(1/x)+1;
    for(int i=1;i<TERMS+1;i++){
        result+=(pow(x,(double)i)/(double)fact(i));
    }
    return result;
}

float tf(int a){
    float x=(float)a;
    float result=(1/x)+1;
    for(int i=1;i<TERMS+1;i++){
        result+=(powf(x,(float)i)/(float)fact(i));
    }
    return result;
}


int main(){
    printf("enter a number: ");
    int x;
    scanf("%d",&x);
    printf("Taylor expansion of %d in double: %.20f\n",x, td(x));
    printf("Taylor expansion of %d in float: %.20f\n",x, tf(x));
    return 0;
}