#include<iostream>
#include<bits/stdc++.h>

using namespace std;

int main(){

    cout << "Hello World" << endl ;

    
    vector<int> v(10);

    for(int i = 0; i < 10; i++) {
    v[i] = (i+1)*(i+1);
    }

    for(int i = 9; i > 0; i--) {
    v[i] -= v[i-1];
    }

    

    return 0;
}