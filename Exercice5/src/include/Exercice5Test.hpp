#ifndef Exercice5Test_hpp
#define Exercice5Test_hpp

#include <functional>
#include <algorithm>
#include <cmath>
#include <omp.h>

namespace merging {

  /**
   * @class Exercice5Test Exercice5Test.hpp
   *
   * Version OpenMP de l'algorithme merge de la bibliothèque standard.
   * 
   * @note L'implémentation proposée est celle de l'algorithme de fusion stable
   *   et parallèle décrite dans C. Siebert and J.L. Träff, "Perfectly 
   *   load-balanced, optimal, stable, parallel merge", CoRR, pp -1--1, 2013.
   * @note Cette implémentation ne peut être employée qu'avec une relation
   *   d'ordre de type <= ou >= mais pas < ou >.
   */
  class ParallelStableMerge {
  public:

    /**
     * Implémentation parallèle.
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
     * @param[in] threads - le nombre de threads disponibles.
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
	  const int& threads) {

      // Types synonymes permettant de ne rien préjuger des types entiers
      // manipulés.
      typedef std::iterator_traits< InputRandomAccessIterator1 > TraitsInput1;
      typedef std::iterator_traits< InputRandomAccessIterator2 > TraitsInput2;
      typedef std::iterator_traits< OutputRandomAccessIterator > TraitsOutput;
      typedef typename TraitsInput1::difference_type InputSize1;
      typedef typename TraitsInput2::difference_type InputSize2;
      typedef typename TraitsOutput::difference_type OutputSize;

      // Tailles respectives des deux conteneurs à fusionner.
      const InputSize1  m = last1 - first1;
      const InputSize2 n = last2 - first2;

      // Calcul de la taille du conteneur accueillant la fusion.
      const OutputSize mpn = m + n;

      // Calcul de la taille des fragments dans le conteneur cible de la fusion.
      const OutputSize taille = std::ceil(mpn * 1.0 / threads);


      // à modifier

      // Boucle parallèle sur les fragments.
      #pragma omp parallel for num_threads(threads)
      for (OutputSize ir = 0; ir < threads; ir++) {
        // Calcul du couple (j_{r}, k_{r}) correspondant au 
        // rang i_{r} dans le conteneur cible.
        InputSize1 jr;
        InputSize2 kr;
        coRank(ir, 
              first1, 
              m, 
              first2, 
              n, 
              comp, 
              jr, 
              kr);

        // Calcul de la valeur de i_{r+1} qu'il faut éventu-
        // ellement ajuster.
        OutputSize irp1 = ir + taille;
        if (irp1 > mpn) {
            irp1 = mpn;
        }

          // Calcul du couple (j_{r+1}, k_{r+1}) correspondant 
          // au rang i_{r+1} dans le conteneur cible.
          InputSize1 jrp1;
          InputSize2 krp1;
          coRank(irp1, 
              first1, 
              m, 
              first2, 
              n, 
              comp, 
              jrp1, 
              krp1);

          // Nous disposons de toutes les infos pour réaliser
          // la fusion dont le fragment courant est la cible.
          // Cette opération est réalisée via l'algorithme
          // merge de la bibliothèque standard.
          std::merge(first1 + jr, 
                     first1 + jrp1,
                     first2 + kr, 
                     first2 + krp1,
                     result + ir,
                     comp);
        }

      
      /*
      tbb::parallel_for(static_cast< OutputSize >(0), // Borne inférieure.
      			mpn,                          // Borne supérieure.
      			taille,                       // Pas.
                        [&](const OutputSize& ir) -> void {  // Fonction.

			  // Calcul du couple (j_{r}, k_{r}) correspondant au 
			  // rang i_{r} dans le conteneur cible.
			  InputSize1 jr;
			  InputSize2 kr;
			  coRank(ir, 
			  	 first1, 
			  	 m, 
			  	 first2, 
			  	 n, 
			  	 comp, 
			  	 jr, 
			  	 kr);

			  // Calcul de la valeur de i_{r+1} qu'il faut éventu-
			  // ellement ajuster.
			  OutputSize irp1 = ir + taille;
			  if (irp1 > mpn) {
			    irp1 = mpn;
			  }

			  // Calcul du couple (j_{r+1}, k_{r+1}) correspondant 
			  // au rang i_{r+1} dans le conteneur cible.
			  InputSize1 jrp1;
			  InputSize2 krp1;
			  coRank(irp1, 
			  	 first1, 
			  	 m, 
			  	 first2, 
			  	 n, 
			  	 comp, 
			  	 jrp1, 
			  	 krp1);

			  // Nous disposons de toutes les infos pour réaliser
			  // la fusion dont le fragment courant est la cible.
			  // Cette opération est réalisée via l'algorithme
			  // merge de la bibliothèque standard.
			  std::merge(first1 + jr, 
			  	     first1 + jrp1,
			  	     first2 + kr, 
			  	     first2 + krp1,
			  	     result + ir,
			  	     comp);

			});
            */

            // fin à modifier

      // Respect de la sémantique de l'algorithme merge.
      return result + mpn;

    } // apply

   /**
     * Implémentation parallèle pour la relation d'ordre total inférieur ou
     * égal.
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
	  const int& threads) {

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
		   std::less_equal< const value_type& >(),
		   threads);
      
    } // apply

  protected:

    /**
     * Recherche dichotomique de la paire (j, k) représentant les rangs,
     * respectivement dans le premier et le second conteneur, des éléments
     * susceptibles d'être accueillis à la position de rang i dans le conteneur
     * cible de la fusion. 
     *
     * @param[in] i - le rang de l'élément actuellement traité dans le conteneur
     *   cible de la fusion ;
     * @param[in] a - un itérateur repérant l'élément de tête du premier 
     *   conteneur ;
     * @param[in] m - le nombre d'éléments du premier conteneur ;
     * @param[in] b - un itérateur repérant l'élément de tête du second 
     *   conteneur ;
     * @param[in] n - le nombre d'éléments du second conteneur ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les éléments des conteneurs àfusionner ;
     * @param[in,out] j - le rang du candidat potentiel dans le premier 
     *   conteneur ;
     * @param[in,out] k - le rang du candidat potentiel dans le second 
     *   sous-conteneur.
     * 
     * @note Les types abstraits autres que InputRandomAccessIterator1,
     *   InputRandomAccessIterator2 et Compare ne sont là que pour simplifier
     *   l'écriture de cette méthode et ne rien préjuger des types concrets
     *   qui serviront à les instancier.
     */
    template< typename OutputSize,
	      typename InputRandomAccessIterator1, 
	      typename InputSize1,
	      typename InputRandomAccessIterator2,
	      typename InputSize2,
	      typename Compare >
    static void coRank(const OutputSize& i,
		       const InputRandomAccessIterator1& a,
		       const InputSize1& m, 
		       const InputRandomAccessIterator2& b,
		       const InputSize2& n,
		       const Compare& comp,
		       InputSize1& j,
		       InputSize2& k) {

      j = std::min(i, m);
      k = i - j;
      InputSize1 jlow = i > n ? i - n : 0;
      bool active = true;
      InputSize2 klow = 0; 
      while (active) {
	if (j > 0 && k < n && ! comp(*(a + j - 1), *(b + k))) {
	  const InputSize1 delta = std::ceil((j - jlow) / 2.0);
	  klow = k;
	  j = j - delta;
	  k = k + delta;
	}
	else if (k > 0 && j < m && comp(*(a + j), *(b + k - 1))) {
	  const InputSize2 delta = std::ceil((k - klow) / 2.0);
	  jlow = j;
	  j = j + delta;
	  k = k - delta;
	}
	else {
	  active = false;
	}

      }

    } // coRank

  }; // ParallelStableMerge

} // merging

#endif
