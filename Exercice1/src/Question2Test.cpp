#include "Found_N_Length_Subset.hpp"
#include <iostream>
#include <cstdlib>

/**
 * Programme principal.
 *
 * @return @c EXIT_FAILURE car exécution toujours réussie.
 */
int main(){

    using namespace std;

    //Found_N_length_Subset
    cout << "-- [Found_N_length_Subset test - Begin] --" << endl;
    cout << '\t' << "Found N Length Subset<int , 1 , 3 >::Yes = " << Found_N_Length_Subset<int, 1 , 3 >::Yes << endl;
    cout << '\t' << "Found N Length Subset<int , 0 >::Yes = " << Found_N_Length_Subset<int, 0 >::Yes << endl;
    cout << '\t' << "Found N Length Subset<char , 2 , 'a', 'b', 'a' >::Yes = " << Found_N_Length_Subset<char, 2 , 'a', 'b', 'a' >::Yes << endl;
    cout << '\t' << "Found N Length Subset<char , 2 , 'a', 'b', 'a', 'a' >::Yes = " << Found_N_Length_Subset<char, 2 , 'a', 'b', 'a', 'a' >::Yes << endl;
    cout << "-- [Found_N_length_Subset test - End] --" << endl;

    return EXIT_SUCCESS;
}
