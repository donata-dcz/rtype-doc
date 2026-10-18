# Document de décision technique - Choix des bibliothèques graphiques et réseau

## Contexte

Le projet nécessite deux composants techniques principaux :

* **Affichage graphique** : création de la fenêtre, affichage des éléments, gestion du clavier et de la souris.
* **Communication réseau** : échange de données entre plusieurs instances du programme, avec la possibilité de gérer plusieurs connexions sans bloquer l'application.

L'objectif est d'utiliser des bibliothèques **légères, simples à intégrer et compatibles avec le langage C++**, tout en évitant d'ajouter une complexité inutile au projet.

---

# Bibliothèque graphique - Raylib

## Options étudiées

Plusieurs solutions ont été envisagées :

* Raylib
* SDL
* SFML
* OpenGL utilisé directement

## Choix

Nous avons choisi **Raylib** pour la partie graphique.

## Raisons

Raylib répond directement aux besoins du projet tout en restant simple à utiliser.

Ses principaux avantages sont :

* Une interface de programmation simple et intuitive.
* Une bibliothèque légère.
* Une bonne compatibilité avec le C++.
* La gestion de la fenêtre et des entrées utilisateur.
* La possibilité de réaliser du rendu en deux ou trois dimensions.
* Une bonne portabilité entre les différents systèmes.
* Une prise en main rapide permettant de faciliter le développement et les essais.

### Pourquoi ne pas utiliser OpenGL directement ?

OpenGL est une solution plus bas niveau. Son utilisation aurait nécessité de gérer nous-mêmes plusieurs éléments supplémentaires, notamment la création de la fenêtre, la gestion du contexte graphique et différentes opérations liées au rendu.

Cela aurait augmenté la complexité du projet sans apporter de bénéfice nécessaire à nos besoins.

### Pourquoi ne pas utiliser SDL ?

SDL est une bibliothèque très complète et adaptée au développement multimédia. Cependant, elle fournit une approche davantage orientée bas niveau.

Raylib correspond mieux à notre besoin car elle permet d'obtenir rapidement un affichage fonctionnel avec une quantité de code réduite.

---

# Bibliothèque réseau - ASIO

## Options étudiées

Plusieurs solutions ont été envisagées :

* ASIO
* Boost ASIO
* Les interfaces de communication réseau natives du système
* Une bibliothèque réseau de niveau plus élevé

## Choix

Nous avons choisi **ASIO** pour la communication réseau.

## Raisons

ASIO permet de gérer les communications réseau tout en conservant une bonne maîtrise de l'architecture du projet.

Ses principaux avantages sont :

* Une interface adaptée au C++.
* La gestion des communications de manière asynchrone.
* La prise en charge des communications TCP et UDP.
* Une bonne portabilité entre les différents systèmes.
* La possibilité de gérer plusieurs connexions.
* La possibilité de construire notre propre protocole de communication.
* L'absence de dépendance à l'ensemble de la bibliothèque Boost dans le cas d'une utilisation autonome.

### Pourquoi ne pas utiliser directement les interfaces réseau du système ?

L'utilisation directe des interfaces réseau aurait obligé le projet à gérer lui-même les différences entre les systèmes ainsi que la logique permettant d'effectuer des communications sans bloquer le programme.

ASIO fournit déjà cette abstraction et permet donc de réduire la quantité de code spécifique au système.

### Pourquoi ne pas utiliser une bibliothèque réseau de niveau plus élevé ?

Nous souhaitons conserver la maîtrise du protocole et du format des données échangées entre les différentes instances du programme.

ASIO fournit les fonctionnalités nécessaires à la communication tout en nous laissant la liberté de définir notre propre fonctionnement réseau.

---

# Synthèse

| Besoin               | Bibliothèque choisie | Raisons principales                               |
| -------------------- | -------------------- | ------------------------------------------------- |
| Affichage graphique  | **Raylib**           | Simple, légère, portable et adaptée à nos besoins |
| Communication réseau | **ASIO**             | Asynchrone, portable, flexible et adaptée au C++  |

## Décision finale

**Raylib** a été choisie pour la partie graphique afin de disposer d'une solution simple et légère permettant de gérer efficacement l'affichage et les interactions avec l'utilisateur.

**ASIO** a été choisie pour la partie réseau afin de disposer d'une solution portable et asynchrone permettant de gérer les communications sans bloquer le fonctionnement général de l'application.

Ces deux choix permettent de conserver une architecture **simple, portable et facilement maintenable**, tout en disposant des fonctionnalités nécessaires au projet.
