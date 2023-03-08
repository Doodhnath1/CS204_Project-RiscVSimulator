// DIVYANKAR SHAH // 2021CSB1086 //
#include <bits/stdc++.h>
using namespace std;

char* dec2hex(unsigned int n)
{
    static char hex_dec[1000];
    int i=0,tn=0;
    while(n>0)
    {
        tn=n%16;
        if(tn<10){
            hex_dec[i]=tn+48;
            i++;
        }
        else{
            hex_dec[i]=tn+55;
            i++;
        }
        n=n/16;
    }
    return hex_dec;

}

int main()
{
    int n;
    cout<<"Enter the decimal number : ";
    cin>>n;
    char *ptr;
    int i=0;
    ptr=dec2hex(n);
    if(n<0){
        cout<<"Enter the positive no's only."<<'\n';
    }
    else{
        for(int i=32; i>=0; i--){
            cout<<ptr[i];
        }
    }
    return 0;
}