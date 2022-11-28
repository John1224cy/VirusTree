#include<iostream>
#include"virTree.h"
#include"SandBox.h"

using namespace std;

int main(){
    cout << "Please input a virusTree:\n";
    virTree VT;
    char function_select;
    cout << "Please select a function: \n    1.find public parent   2.vaccinum time  3.sandbox simulation\n    (press q to quit)\n";
    while (1){
        cin >> function_select; 
        if (function_select == '1'){
            int cd1, cd2;
            cout << "Please input codes: ";
            cin >> cd1 >> cd2;
            virus* pub = VT.public_parent(cd1, cd2);
            cout << "Their public parent is code-" << pub->code() << "-virus" << endl;
        }
        else if (function_select == '2')
            VT.succ_vaccinum();

        else if (function_select == '3'){   
            long crowds_num, days; 
            cout << "Please input the number of SandBox: ";
            cin >> crowds_num;
            cout << "Please input the number of days: ";
            cin >> days;
            SandBox(VT, crowds_num, days);
        }
        else if (function_select == 'q')
            break;
            
        cout << endl << "Please select a function: ";
    }
    return 0;
}