# Saulcy Road

> **Note sur le projet**
> Ce projet a été réalisé en groupe dans le cadre d'un projet universitaire en groupe. Je n'ai donc pas réalisé l'intégralité du travail présenté ici.
>
> Mes contributions principales sur ce projet ont été :
> *   Contribution à la conception de la représentation des données.
> *   Réalisation d'un état de l’art et étude du fonctionnement de Crossy Road.
> *   Gestion de projet : planification sur Asana, rédaction de la fiche projet et des comptes-rendus de réunion.
> *   Développement de la fonctionnalité "vestiaire" et du menu principal.
> *   Réalisation de sprites.

## À propos du projet

Saulcy Road est un jeu développé en C, inspiré du célèbre jeu "Crossy Road". L'objectif est de guider un personnage à travers un plateau de jeu rempli d'obstacles, en essayant d'aller le plus loin possible. Chaque pas en avant augmente le score, et le meilleur score est sauvegardé.

![Screenshot du jeu Saulcy Road](screenshot.png)

## Fonctionnalités

### Modes de jeu

Le jeu propose plusieurs modes :

-   **Version Textuelle** : Une version simplifiée qui se joue directement dans le terminal. Le joueur contrôle le personnage avec les touches ZQSD ou les flèches.
-   **Version Graphique** : Une version 2D plus immersive avec une interface graphique réalisée à l'aide de la librairie SDL2.
-   **Mode IA** : Dans les deux versions, il est possible de laisser une intelligence artificielle jouer à votre place et tenter d'obtenir le meilleur score.

### Personnages et Obstacles

-   **Vestiaire** : Un menu permet de choisir l'apparence du personnage parmi plusieurs options : Poulet, Canard ou Crocodile.
-   **Plateau de jeu** : Le plateau est généré aléatoirement à chaque partie et est composé de différents types de bandes : route, rivière, prairie et rails de train.
-   **Obstacles** :
    -   **Mortels** : Voitures, camions, trains et l'eau.
    -   **Ralentisseurs** : Buissons et arbres qui entravent la progression.

## Explications Techniques

### Langage et Librairies

-   **Langage** : Le projet est entièrement développé en **C**.
-   **Librairies** :
    -   `SDL2`, `SDL2_image`, `SDL2_mixer` : Pour la version graphique, la gestion des sprites, des animations et de l'audio.
    -   `ncurses` : Pour la gestion de l'affichage dans la version textuelle.
    -   `CUnit` : Pour les tests unitaires.

### Structure du code

Le code est organisé de manière modulaire pour une meilleure clarté et maintenance :

-   `partie.c` : Contient le noyau de la logique du jeu (déplacements, collisions, score).
-   `renderer.c` : Gère l'affichage de la version graphique (fenêtre, textures, menus).
-   **Structures de données** : Le jeu utilise des listes chaînées pour représenter les différents éléments du plateau (lignes, obstacles statiques et dynamiques).
-   **IA** : Un algorithme a été implémenté pour permettre à un joueur virtuel de trouver le chemin le plus sûr.

## Comment exécuter le projet

### Prérequis

Assurez-vous d'avoir les librairies suivantes installées sur votre système :
-   `SDL2`
-   `SDL2_image`
-   `SDL2_mixer`
-   `CUnit`
-   `ncurses`

Sur macOS, vous pouvez les installer avec Homebrew :
```bash
brew install sdl2 sdl2_image sdl2_mixer cunit
```
Sur les distributions basées sur Debian (comme Ubuntu) :
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libcunit1-dev
```

### Compilation et Exécution

Un `Makefile` est fourni pour simplifier la compilation et l'exécution du projet.

1.  **Compiler le jeu** :
    ```bash
    make build
    ```

2.  **Lancer le jeu** :
    ```bash
    make run
    ```
    ou
    ```bash
    ./app
    ```

3.  **Compiler et lancer les tests unitaires** :
    ```bash
    make test
    ```

4.  **Nettoyer les fichiers de compilation** :
    ```bash
    make clean
    ```
