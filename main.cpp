#include <iostream>
#include "src/Container.h"

int main() {
    Container cont(2,"s","d");
    cout << cont << endl;

    Container cont2 = cont;
    cout << cont2 << endl;

    cont.setWeight(5);
    cont.setDestPort("yosi");

    cout << "Cont: " << cont << endl;
    cout << "Cont2: " << cont2 << endl;


}