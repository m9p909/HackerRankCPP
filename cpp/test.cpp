#include <iostream>
using namespace std;

int main(){
    char start = 'p';
    int counter = 0;
    while(start != ' ' && counter <200){
        cin >> start;
        cout << start;
        ++counter;
    }
}