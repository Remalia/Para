#ifndef ParallelRecursiveMerge_hpp
#define ParallelRecursiveMerge_hpp

#include <functional>
#include <algorithm>
#include <tbb/tbb.h>
#include <iostream>
#include <sstream>

namespace merging {

  /**
   * @class ParallelRecursiveMerge ParallelRecursiveMerge.hpp
   *
   * Version TBB de l'algorithme merge de la biblothèque standard. 
   * 
   * @note L'implémentation proposée est celle du recursive merging décrite dans
   *   Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford 
   *   Stein, "Introduction to Algorithms", 3rd ed., 2009, pp 798-802. La 
   *   récursion est interrompue lorsque la somme des tailles des deux 
   *   sous-conteneurs à fusionner passe sous une certaine tolérance. La fusion 
   *   est alors effectuée via l'algorithme merge de la bibliothèque standard.
   */
  class ParallelRecursiveMerge {
  public:

    /**
     * Forme générale de l'algorithme.
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     * @return un itérateur repérant la fin de la zone de fusion dans le
     *   conteneur cible.
     */
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator,
	      typename Compare >
    static OutputRandomAccessIterator 
    apply(const InputRandomAccessIterator1& first1,
	  const InputRandomAccessIterator1& last1,
	  const InputRandomAccessIterator2& first2,
	  const InputRandomAccessIterator2& last2,
	  const OutputRandomAccessIterator& result,
	  const Compare& comp,
	  const size_t& cutoff) {

      // Invocation de la stratégie adéquate.
      strategyTasking(first1, 
		  last1, 
		  first2, 
		  last2, 
		  result, 
		  comp, 
		  cutoff);
      
      // Respect de la sémantique de l'algorithme merge.
      return result + (last1 - first1) + (last2 - first2);

    } // apply

    /**
     * Forme spécifique de l'algorithme pour la relation d'ordre total 
     * strictement inférieur à.
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     * @return un itérateur repérant la fin de la zone de fusion dans le
     *   conteneur cible.
     */
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator >
    static OutputRandomAccessIterator
    apply(const InputRandomAccessIterator1& first1,
	  const InputRandomAccessIterator1& last1,
	  const InputRandomAccessIterator2& first2,
	  const InputRandomAccessIterator2& last2,
	  const OutputRandomAccessIterator& result,
	  const size_t& cutoff) {

      // Type synonyme pour le type des éléments du premier conteneur. Le type 
      // des éléments du second devra pouvoir se convertir implicitement en le 
      // type des éléments du premier.
      typedef std::iterator_traits< InputRandomAccessIterator1 > Traits;
      typedef typename Traits::value_type value_type;

      // Fabriquer le comparateur less puis invoquer la méthode définie 
      // ci-dessus.
      return apply(first1, 
		   last1,
		   first2,
		   last2,
		   result,
		   std::less< const value_type& >(),
		   cutoff);
      
    } // apply

  protected:


    /**
     * Implementation de base et appel en recursion ensuite pour la stategie B
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     */
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator,
	      typename Compare >
    static void strategyTasking(const InputRandomAccessIterator1& first1,
			  const InputRandomAccessIterator1& last1,
			  const InputRandomAccessIterator2& first2,
			  const InputRandomAccessIterator2& last2,
			  const OutputRandomAccessIterator& result,
			  const Compare& comp,
			  const size_t& cutoff) {

      strategyTaskingRecursive(first1,last1,first2,last2,result,comp,cutoff);


    } // strategyB

    /**
     * Implementation de tbb_invoke sur un merge de maniere recursive (strategyB)
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     */    
template< typename InputRandomAccessIterator1,
          typename InputRandomAccessIterator2,
          typename OutputRandomAccessIterator,
          typename Compare >
static void strategyTaskingRecursive(const InputRandomAccessIterator1& first1,
                               const InputRandomAccessIterator1& last1,
                               const InputRandomAccessIterator2& first2,
                               const InputRandomAccessIterator2& last2,
                               const OutputRandomAccessIterator& result,
                               const Compare& comp,
                               const size_t& cutoff) {
    // Taille des deux sous-conteneurs.
    const auto size1 = last1 - first1;
    const auto size2 = last2 - first2;

    // Tolérance atteinte : appel direct à std::merge.
    if (static_cast<size_t>(size1 + size2) < cutoff) {
        std::merge(first1, last1, first2, last2, result, comp);
        return;
    }



    // Le sous-conteneur gauche est supposé être plus long.
    if (size1 < size2) {
        strategyTaskingRecursive(first2, last2, first1, last1, result, comp, cutoff);
        return;
    }

    // Calcul des positions médianes.
    const InputRandomAccessIterator1 middle1 =
    first1 + size1 / 2;
    const InputRandomAccessIterator2 middle2 =
    std::lower_bound(first2, last2, *middle1, comp);
    const OutputRandomAccessIterator middle3 =
    result + (middle1 - first1) + (middle2 - first2);

    //Recopie de l'élément médian du sous-conteneur gauche dans le 
    //sous-conteneur résultat.
    *middle3 = *middle1;

    //Groupe de taches TBB pour gerer le parallelisme.
    tbb::task_group groupeTache;

    //Premiere tache
    groupeTache.run([=]() {
        strategyTaskingRecursive(first1, middle1, first2, middle2, result, comp, cutoff);
    });

    //Deuxieme tache
    groupeTache.run([=]() {
        strategyTaskingRecursive(middle1 + 1, last1, middle2, last2, middle3 + 1, comp, cutoff);
    });

    // Attendre que toutes les taches soient terminees
    groupeTache.wait();
}

}; // merging

}
#endif
