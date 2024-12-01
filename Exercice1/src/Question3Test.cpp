#include <iostream>
#include <cstdlib>

#include "Longest_Subset.hpp"

/**
 * Programme principal.
 *
 * @return @c EXIT_FAILURE car exécution toujours réussie.
 */
int main(){

    using namespace std;

    //Longest_Subset
    cout << "-- [Longest_Subset test - Begin] --" << endl;
    cout << '\t' << "Longest_Subset<int>::value = " << Longest_Subset<int>::value << endl;
    cout << '\t' << "Longest_Subset<char , 'a' >::value = " << Longest_Subset<char , 'a' >::value << endl;
    cout << '\t' << "Longest_Subset<int , 0 , 1, 0, 0, 1, 1, 1 >::value = " << Longest_Subset<int , 0 , 1, 0, 0, 1, 1, 1 >::value << endl;
    cout << "-- [Longest_Subset test - End] --" << endl;

    return EXIT_SUCCESS;
}
