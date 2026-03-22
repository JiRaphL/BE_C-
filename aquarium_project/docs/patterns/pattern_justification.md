# Justification des Design Patterns

## Contexte

Le projet partait d'une architecture monolithique (logique de déplacement codée dans `Bestiole`, création non centralisée, peu d'extensibilité).  
L'objectif était de converger vers une architecture modulaire alignée avec le sujet BE et la grille d'évaluation.

## 1) Strategy - comportements (`IBehavior`)

### Pourquoi

- Les comportements sont interchangeables et évolutifs (grégaire, peureuse, kamikaze, prudente).
- Le sujet impose le changement de comportement dynamique possible.
- Évite l'explosion combinatoire d'héritage (`BestioleGregaireAvecOreillesEtCarapace`, etc.).

### Plus-value constatée

- Ajouter un comportement = `1` nouvelle classe + enregistrement dans `BestioleFactory`.
- `Bestiole` reste stable (pas de `if/else` métier sur les types de comportement).
- Tests unitaires ciblés par comportement (fichiers dédiés).

## 2) Factory - création centralisée (`BestioleFactory`)

### Pourquoi

- Le sujet demande explicitement une ressource de création partagée.
- Permet de respecter les pourcentages initiaux de comportements.
- Intègre naturellement les taux de naissance spontanée et clonage.

### Plus-value constatée

- Population initiale reproductible par `SimulationConfig`.
- L'équipement capteurs/accessoires est centralisé.
- Les managers runtime (cycle de vie) délèguent la création à la factory.

## 3) Composition - capteurs/accessoires

### Pourquoi

- Le sujet exige 0..n capteurs et 0..n accessoires par bestiole.
- Ce besoin est orthogonal au comportement.

### Plus-value constatée

- `Bestiole` compose:
  - `std::vector<std::unique_ptr<ISensor>>`
  - `std::vector<std::unique_ptr<IAccessory>>`
- Détection et modificateurs physiques sont extensibles sans modifier l'API publique globale.

## 4) Séparation d'orchestration et de rendu

### Pourquoi

- Le sujet attend une architecture propre et testable.
- Découpler boucle simulation/rendu/statistiques améliore l'intégration et les tests.

### Implémentation

- `Simulation` orchestre le tick.
- `Renderer` (interface) + `CImgRenderer` (implémentation).
- `StatisticsCollector` collecte et exporte CSV/JSON.

### Plus-value constatée

- Test d'intégration sans GUI via renderer stub.
- Exports quantifiés pour la soutenance et l'analyse.

## 5) Managers spécialisés (responsabilités isolées)

- `CollisionManager`: collision, rebond, mortalité probabiliste.
- `LifecycleManager`: âge, mort naturelle, clonage, naissance spontanée.

Cette séparation limite les classes "couteau suisse" et rend les choix d'implémentation justifiables.

## 6) Choix C++ et maintenabilité

- Smart pointers (`unique_ptr`) pour l'ownership explicite.
- `enum class BehaviorKind` dans la factory.
- Const-correctness et interfaces abstraites.
- Build multi-cibles (`app`, `tests`) avec Makefile et CMake.

## 7) Quantification synthétique (échantillon réel)

Extrait d'un run réel (`simulation_stats.csv`):

- `1919` ticks simulés
- `389` collisions (`0.203` collision/tick)
- `118` morts par collision
- `5` morts par âge
- `116` naissances totales (`89` clones + `27` spontanées)
- Population `20 -> 13` (min `5`, max `20`)

Ces chiffres montrent que les patterns choisis supportent:

- l'exécution longue durée,
- la traçabilité fine d'événements,
- l'analyse quantitative demandée par la grille.
