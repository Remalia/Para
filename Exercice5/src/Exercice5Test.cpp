#include "Exercice5Test.hpp"
#include "Metrics.hpp"
#include <vector>
#include <numeric>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <omp.h>


/**
 * Programme principal.
 *
 * @param[in] argc le nombre d'arguments de la ligne de commandes.
 * @param[in] argv les arguments de la ligne de commandes.
 * @return @c EXIT_SUCCESS en cas d'exécution réussie ou @c EXIT_FAILURE en cas
 *   de problèmes.
 */
int main(int argc, char* argv[]) {

   // La ligne de commandes est vide : l'utilisateur demande de l'aide.
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " nb_iterations" << std::endl;
    return EXIT_SUCCESS;
  }

  // Le nombre d'arguments est différent de 1 : l'utilisateur fait n'importe
  // quoi.
  if (argc != 2) {
    std::cerr << "Nombre d'argument(s) incorrect." << std::endl;
    return EXIT_FAILURE;
  }

   // Tentative d'extraction du nombre d'itérations.
  size_t iters;
  {
    std::istringstream entree(argv[1]);
    entree >> iters;
    if (! entree || ! entree.eof()) {
      std::cerr << "Argument incorrect." << std::endl;
      return EXIT_FAILURE;
    }
  }   

  // Synonyme du type des éléments à fusionner.
  typedef int Type;

  // Relation d'ordre utilisée : strictement inférieur à.
  const auto comp = std::less< const Type& >();

  // Deux tableaux d'entiers de taille différentes à fusionner.
  std::vector< Type > lhs(128 * 1024), rhs(lhs.size() + 211);
  std::iota(lhs.begin(), lhs.end(), 19);
  std::iota(rhs.begin(), rhs.end(), 5);

  // Conteneur accueillant le résultat de la fusion.
  std::vector< Type > result(lhs.size() + rhs.size());

  // Temps auquel sont démarrées et arrêtées chaque séquence de calcul.
  std::chrono::time_point< std::chrono::steady_clock > start, stop;

  // Durée d'exécution de l'algorithme merge de la bibliothèque standard.
  start = std::chrono::steady_clock::now();
  for (size_t i = 0; i != iters; i ++) {
    std::merge(lhs.begin(), 
    	       lhs.end(),
    	       rhs.begin(), 
    	       rhs.end(),
    	       result.begin(),
    	       comp);
  }
  stop = std::chrono::steady_clock::now();
  const int seq = 
    std::chrono::duration_cast< std::chrono::milliseconds >(stop - start).count();  

  // Affichage des performances de la version séquentielle.
  std::cout << "--[ merge: begin ]--" << std::endl;
  std::cout << "\tDurée:\t\t" << seq << " msec." << std::endl;
  std::cout << "\tVerdict:\t\t"
  	    << std::boolalpha 
  	    << std::is_sorted(result.begin(), result.end(), comp)
  	    << std::endl;
  std::cout << "--[ merge: end ]--" << std::endl;
  std::cout << std::endl;



  // à changer

  // Initialisation du scheduler de TBB avec le nombre de cores logiques.
  /*
  #ifdef __TBB_info_H  
    const int threads = tbb::info::default_concurrency();
  #else  
    const int threads = tbb::task_scheduler_init::default_num_threads();
    tbb::task_scheduler_init init(threads);
  #endif  
  */

  // init du scheduler d'openmp
  const int threads = omp_get_max_threads();
  omp_set_num_threads(threads);  
  


  // L'algorithme ParallelStableMerge ne peut fonctionner qu'avec une relation
  // d'ordre telle que <= ou >= mais pas < ou >, ce qui est justement notre
  // cas. Nous allons donc ruser en deux temps :
  // 1) nous allons définir la relation d'ordre inverse : ici >=.
  // 2) nous allons fusionner nos conteneurs en les balayant tous de la droite
  //    vers la gauche.
  // 3) Bonjour le mal de tête ...
  const auto invComp = std::greater_equal< const Type& >();

  // Durées d'exécution de l'algorithme ParallelStableMerge. Nous allons 
  // utiliser plusieurs valeurs du nombre de threads disponibles.
  // for (int nb = 1; nb <= threads; nb ++) {
  start = std::chrono::steady_clock::now();
  for (size_t i = 0; i != iters; i ++) {
    merging::ParallelStableMerge::apply(lhs.rbegin(), 
					  lhs.rend(),
					  rhs.rbegin(), 
					  rhs.rend(),
					  result.rbegin(),
					  invComp,
					  threads);
  }
  stop = std::chrono::steady_clock::now();
  const int par = std::chrono::duration_cast< std::chrono::milliseconds >(stop - start).count();    

  // Affichage des résultats de la version parallèle avec, en plus, le calcul
  // des facteurs d'accélération et d'efficacité. Une accélération sur-linéaire
  // indique une meilleure utilisation des caches L2 (partagé) et L1 (privé).  
    std::cout << "--[ ParallelStableMerge: begin ]--" << std::endl;
    std::cout << "\tThread(s):\t" << threads << std::endl;
    std::cout << "\tDurée:\t\t" << par << " msec." << std::endl;
    std::cout << "\tVerdict:\t\t"
  	      << std::boolalpha 
  	      << std::is_sorted(result.begin(), result.end(), comp)
  	      << std::endl;
    std::cout << "\tSpeedup:\t" 
  	      << Metrics::speedup(seq, par)
  	      << std::endl;
    std::cout << "\tEfficiency:\t"
  	      << Metrics::efficiency(seq, par, threads)
  	      << std::endl;
    std::cout << "--[ parallelStableMerge: end ]--" << std::endl;
    std::cout << std::endl;
  

  // Tout s'est bien passé.
  return EXIT_SUCCESS;

}
