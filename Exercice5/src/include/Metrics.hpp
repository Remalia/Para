#ifndef Metrics_hpp
#define Metrics_hpp

/**
 * @class Metrics Metrics.hpp
 *
 * Mesureurs de performances d'une application parallèle.
 */
class Metrics {
public:

  /**
   * Calcule le facteur d'accélération.
   *
   * @param[in] seq - la durée d'exécution du meilleur algorithme séquentiel 
   *   permettant de résoudre le problème.
   * @param[in] par - la durée d'exécution du meilleur algorithme parallèle 
   *   permettant de résoudre le problème.
   * @return le facteur d'accélération.
   */
  static double speedup(const double& seq, const double& par);

  /**
   * Calcule le facteur d'efficacité.
   *
   * @param[in] seq - la durée d'exécution du meilleur algorithme séquentiel 
   *   permettant de résoudre le problème.
   * @param[in] par - la durée d'exécution du meilleur algorithme parallèle 
   *   permettant de résoudre le problème. 
   * @param[in] procs - le nombre de processeurs utilisés par le meilleur
   *   algorithme parallèle.
   * @return le facteur d'efficacité.
   */
  static double efficiency(const double& seq,
			   const double& par,
			   const unsigned& procs);

}; // Metrics

#endif
