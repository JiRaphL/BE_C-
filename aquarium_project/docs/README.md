# BE Bestioles - Guide de prise en main

Ce projet est une refonte modulaire d'une simulation d'aquarium en C++17.  
L'objectif est de faire evoluer une population de bestioles dans un milieu 2D, avec comportements dynamiques, capteurs, accessoires, collisions, cycle de vie, statistiques et interface graphique basee sur `CImg`.

## Ce que montre le projet

- l'utilisation de `Strategy` pour les comportements ;
- l'utilisation de `Factory` pour la creation centralisee des bestioles ;
- une composition modulaire des capteurs et accessoires ;
- une simulation executable, testable et observable par statistiques.

## Organisation du projet

- `include/` : headers C++ du projet.
- `src/` : implementation de la simulation.
- `src/app/` : orchestration globale et collecte des statistiques.
- `src/managers/` : collisions et cycle de vie.
- `src/behavior/` : comportements concrets des bestioles.
- `src/sensors/` : capteurs (`EyeSensor`, `EarSensor`).
- `src/accessories/` : accessoires (`Fin`, `Shell`, `Camouflage`).
- `tests/` : tests unitaires et d'integration.
- `docs/uml/` : diagrammes UML PlantUML pour la soutenance.
- `scripts/plot_stats.py` : generation des graphiques a partir du CSV.
- `plots/` : graphiques generes apres simulation.

## Prerequis

Sous Windows / PowerShell, les commandes ci-dessous supposent :

- `g++` disponible dans le terminal ;
- `mingw32-make` disponible ;
- `python` disponible ;
- la bibliotheque Python `matplotlib` installee pour les plots.

## Aller a la racine du projet

```powershell
cd "C:\Users\DHERALE\Desktop\C++\aquarium_project"
```

## Compiler la simulation

```powershell
make app
```

Le binaire genere est :

- `aquarium.exe`

## Lancer la simulation

```powershell
.\aquarium.exe
```

A la fermeture de la simulation, deux fichiers de sortie sont generes a la racine du projet :

- `simulation_stats.csv`
- `simulation_stats.json`

## Commandes utiles pendant la simulation

- `ESC` : quitter la simulation.
- `SPACE` : pause / reprise.
- `H` : afficher / masquer le HUD.
- `N` : forcer une naissance.
- `K` : forcer une mort.
- `C` : forcer un changement de comportement.
- `F` : declencher un evenement externe de type `FoodPulse`.
- `P` : declencher un evenement externe de type `Pollution`.
- `D` : declencher un evenement externe de type `Disturbance`.

## Compiler les tests

```powershell
make tests
```

Le binaire genere est :

- `aquarium_tests.exe`

## Lancer les tests

```powershell
.\aquarium_tests.exe
```

Les tests affichent des lignes `PASS` pour chaque scenario valide.

## Generer les graphiques de statistiques

Apres avoir lance au moins une simulation :

```powershell
python .\scripts\plot_stats.py
```

Les images generees sont enregistrees dans :

- `plots/population.png`
- `plots/events_per_tick.png`
- `plots/average_age.png`

Si besoin, on peut aussi preciser explicitement le fichier CSV et le dossier de sortie :

```powershell
python .\scripts\plot_stats.py --input simulation_stats.csv --outdir plots
```

## Verifier rapidement les fichiers de sortie

```powershell
Get-Content .\simulation_stats.csv -TotalCount 20
Get-Content .\simulation_stats.json -TotalCount 40
```

## Nettoyer les fichiers compiles

```powershell
make clean
```

## UML et livrables de soutenance

Les diagrammes UML sont dans `docs/uml/`.

Fichiers principaux :

- `class_diagram_detailed.puml`
- `class_diagram_soft.puml`
- `sequence_01_simulation_orchestration.puml`
- `sequence_02_perception_strategy_behavior.puml`
- `sequence_03_collision_lifecycle_factory.puml`
- `sequence_04_external_events_ihm.puml`
- `use_case_diagram.puml`

Si `plantuml` est installe, on peut generer les diagrammes depuis le terminal :

```powershell
plantuml docs/uml/class_diagram_detailed.puml
plantuml docs/uml/class_diagram_soft.puml
plantuml docs/uml/sequence_01_simulation_orchestration.puml
plantuml docs/uml/sequence_02_perception_strategy_behavior.puml
plantuml docs/uml/sequence_03_collision_lifecycle_factory.puml
plantuml docs/uml/sequence_04_external_events_ihm.puml
plantuml docs/uml/use_case_diagram.puml
```

## Parcours minimal pour verifier le projet de A a Z

1. Ouvrir un terminal PowerShell a la racine du projet.
2. Compiler l'application avec `make app`.
3. Lancer `.\aquarium.exe`.
4. Interagir avec la simulation puis la fermer.
5. Verifier la presence de `simulation_stats.csv` et `simulation_stats.json`.
6. Generer les graphiques avec `python .\scripts\plot_stats.py`.
7. Compiler les tests avec `make tests`.
8. Lancer `.\aquarium_tests.exe`.
