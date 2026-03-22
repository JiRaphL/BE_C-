# Livrables BE Bestioles

Ce dossier regroupe les livrables non-code demandés pour viser les niveaux `+` de la grille.

## Contenu

- `uml/class_diagram.puml`
  - Diagramme de classes du design implémenté.
- `uml/use_case_diagram.puml`
  - Diagramme de cas d'utilisation (vue fonctionnelle).
- `uml/sequence_simulation_tick.puml`
  - Séquence d'un tick complet de simulation.
- `uml/sequence_birth_clone_collision.puml`
  - Séquence centrée collisions, morts, naissances spontanées et clonage.
- `patterns/pattern_justification.md`
  - Justification des design patterns retenus et comparaison avec alternatives.
- `demo/scenarios_quantified.md`
  - Scénarios de démo avec indicateurs chiffrés.

## Génération des diagrammes UML

Les fichiers UML sont en PlantUML (`.puml`).

Exemple de génération (si PlantUML est installé):

```bash
plantuml docs/uml/class_diagram.puml
plantuml docs/uml/sequence_simulation_tick.puml
plantuml docs/uml/sequence_birth_clone_collision.puml
```
