# Scénarios de Démonstration Quantifiés

Ce document propose des scénarios de soutenance exploitant les sorties de simulation (`simulation_stats.csv`, `simulation_stats.json`).

## Indicateurs suivis

- Population: départ / fin / min / max
- Collisions totales
- Morts par collision
- Morts par âge
- Naissances par clonage
- Naissances spontanées
- Répartition des comportements en fin de run

## Scénario A - Run long réel (preuve d'endurance)

Source: `simulation_stats.csv` présent au dépôt.

### Résultats mesurés

- Ticks: `1919`
- Population: `20 -> 13`
- Population min/max: `5 / 20`
- Âge moyen final: `249.846`
- Collisions totales: `389` (`0.203` collision/tick)
- Morts par collision: `118`
- Morts par âge: `5`
- Naissances clonage: `89`
- Naissances spontanées: `27`
- Naissances totales: `116`
- Bilan net population: `-7`

### Lecture

- Le moteur runtime (collisions + lifecycle) est actif en continu.
- La dynamique est non triviale: naissances et décès coexistent.
- Les exports sont exploitables quantitativement.

## Scénario B - Intégration courte contrôlée (test)

Source: `test_stats.csv` généré par `StatisticsIntegrationTests`.

### Résultats mesurés

- Ticks: `5`
- Population: `10 -> 10`
- Collisions: `0`
- Décès: `0`
- Naissances: `0`

### Lecture

- Valide la chaîne complète `Simulation -> Milieu -> Stats -> Export`.
- Permet une preuve simple et rapide de non-régression en CI/TP.

## Scénario C - Démo orientée collisions

Objectif: montrer l'effet de `CollisionManager` et de la résistance de carapace.

### Procédure

1. Augmenter densité initiale (population > 40).
2. Garder `collisionDistance` élevé (`>= 14`).
3. Exécuter 300+ ticks.
4. Comparer:
   - `deathsByCollision` total
   - distribution comportementale finale
   - variation de population

### Critères quantifiés à présenter

- Taux de mortalité collision:
  - `deathsByCollision / collisions`
- Pression collision:
  - `collisions / ticks`

## Scénario D - Démo orientée cycle de vie

Objectif: isoler vieillissement, clonage et naissances spontanées.

### Procédure

1. Réduire lifespan (`min/max` faibles) pour accélérer les morts d'âge.
2. Augmenter `cloningRate` et `spontaneousBirthRate`.
3. Exécuter 500+ ticks.
4. Observer:
   - `deathsByAge`
   - `birthsByClone`
   - `birthsSpontaneous`
   - population finale

### Critères quantifiés à présenter

- Ratio renouvellement:
  - `(birthsByClone + birthsSpontaneous) / deathsByAge`
- Stabilisation:
  - tendance population sur fenêtre glissante de 100 ticks

## Script de démo soutenance (recommandé)

1. Lancer simulation standard et montrer la visualisation comportement/capteurs/accessoires.
2. Afficher `simulation_stats.csv` et résumer KPI du scénario A.
3. Lancer `make test` et montrer le scénario B (pipeline complet validé).
4. Présenter deux variantes de config (scénarios C et D) et comparer les KPI.
