#ifndef LONGEST_SUBSET_HPP
#define LONGEST_SUBSET_HPP

#include "Count.hpp"

/**
* @class Longest_Subset Longest_Subset.hpp
*
* Template de la classe Longest_Subset permettant de trouver la plus longue sous sequences d'une meme valeur
*/
template <typename T, T... list>
class Longest_Subset;

//Calcule sur une liste jusqu'a qu'il ne reste plus rien dans la liste
template <typename T, T car, T ... cdr>
class Longest_Subset<T, car, cdr ...> {
public:
    //Dans un premier temps on calcule la taille sucessive + 1 de la liste
    enum : int { actual = 1 + Count<T, car, cdr ...>::value };
    //Dans un deuxieme temps celui du reste de la liste (ce qui va faire un appel recursif)
    enum : int { next = Longest_Subset<T, cdr ...>::value };
    //On compare pour changer value a chaque fois que des occurences succesives plus grandes sont trouvee
    enum : int { value = (actual > next) ? actual : next };
};

// Si liste vide dans ce cas longeur de 0
template <typename T>
class Longest_Subset<T> {
public:

    enum : int { value = 0 };
};

#endif // LONGEST_SUBSET_HPP