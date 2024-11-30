#ifndef Count_hpp
#define Count_hpp


/**
* @class Count Count.hpp
*
* Template de la classe count permettant de compter le nombres d'occurences consecutives dans une liste
*/
template <typename T,T val, T ... list>
class Count;

//Specialisation liste a plus de deux objets
template <typename T, T valA, T valB, T ... resteList >
class Count<T, valA, valB, resteList ...> {
public:
    //Si les deux objets qui se suivent sont identiques on ajoute 1 a la valeur 
    //et on vois avec le reste de la liste en appel recursif, sinon on met 0
    enum : long { value = (valA != valB) ? 0 : 1 + Count<T,valB, resteList ... >::value }; 

}; 

//Specialisation liste a 2 objets pile 
template <typename T, T valA, T valB>
class Count<T, valA, valB> {
public:
    //On rappelle pas recursivement car le prochain objet sera forcement tout seul
    enum : long { value = (valA == valB) ? 1 : 0 }; 
};

//Specialisation liste a 1 unique objet
template <typename T, T val>
class Count<T, val> {
public:
    //0 car pas de suite d'objet consecutif possible
    enum : long { value = 0 }; 
};

#endif