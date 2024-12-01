#ifndef Found_N_Length_Subset_hpp
#define Found_N_Length_Subset_hpp
#include <cstddef> // Pour size_t

/**
* @class Found_N_Length_Subset Found_N_Length_Subset.hpp
*
* Template de la classe Found_N_Length_Subset qui a pour but de trouver une sous section de taille n dans une liste de type T
*
* La classe est ameliorable car on peut surement utiliser Count.hpp dedans
*/
template <typename T, size_t n, T ... list>
class Found_N_Length_Subset;

//Si il y a plus de trois elements dans la liste
template <typename T, size_t n, T car, T cdr, T ... rest>
class Found_N_Length_Subset<T, n, car, cdr, rest ...> {
public:

    //sauvegarde de la valeur n
    static constexpr size_t N = n;

    //Si les objets qui se suivent sont similaires, on diminue la taille de la sous sequence a trouver et on continue de maniere recursive
    //sinon on la remet a la valeur de n de base et on continue de maniere recursive
    enum : int { Yes = (car == cdr) ? (Found_N_Length_Subset<T, n-1, cdr, rest ...>::Yes) : (Found_N_Length_Subset<T, N, cdr, rest ...>::Yes) };
};

//Si il y a deux elements dans la liste
template <typename T, size_t n, T car, T cdr>
class Found_N_Length_Subset<T, n, car, cdr> {
public:
    
    //Si n est superieur a 2 c'est fichus car par assez d'objet, sinon si ils sont similaire c'est bon sinon c'est fichus
    enum : int { Yes = (n > 2) ? 0 : ((car == cdr) ? 1 : 0)};

};

//n=1, sous-section imediate
template <typename T, T car>
class Found_N_Length_Subset<T, 1, car> {
public:
    enum : int { Yes = 1 };
};

// sous-section inexistante
template <typename T, size_t n>
class Found_N_Length_Subset<T, n> {
public:
    enum : int { Yes = (n == 0) };
};

#endif