# Mario Shab

Ce projet est un jeu 2D développé en C++ utilisant la bibliothèque raylib. Ce README fournit toutes les étapes nécessaires pour récupérer le dépôt, configurer le projet avec CMake, puis lancer le jeu.

## Prérequis

Avant de commencer, assurez-vous d'avoir les outils suivants installés sur votre machine :

Git : pour cloner le dépôt.
CMake : pour configurer le projet.
Raylib : la bibliothèque utilisée pour le développement du jeu.
Un compilateur C++ (comme GCC ou Clang) compatible avec CMake.

## Étapes d'installation

### 1. Cloner le dépôt

Tout d'abord, clonez le dépôt Git du projet sur votre machine locale. Ouvrez un terminal et exécutez la commande suivante :
```bash
git clone https://github.com/votre-utilisateur/votre-repository.git
cd votre-repository
```

### 2. Installer les dépendances

Ce projet utilise raylib, qui est une bibliothèque externe. Suis les instructions ci-dessous pour installer raylib sur ta machine.
```bash
sudo apt update
sudo apt install libraylib-dev
```

### 3. Configurer le projet avec CMake

Une fois que toutes les dépendances sont installées, vous pouvez configurer le projet avec CMake.

Dans votre terminal, naviguez dans le répertoire du projet, puis créez un répertoire pour la compilation (par exemple build) et exécutez CMake :

```bash
mkdir build
cd build
cmake ..
```

Cette commande configure le projet et prépare les fichiers nécessaires à la compilation.

### 4. Compiler le projet
Après la configuration avec CMake, vous pouvez compiler le projet. Toujours dans le répertoire build, exécutez la commande suivante :

```bash
cmake --build .
```

### 5. Lancer le jeu

Une fois la compilation terminée, vous pouvez lancer le jeu. Dans le répertoire build, exécutez :

```bash
./MarioShab
```

### Commandes du jeu 

Flèches directionnelles : Utilisez les flèches gauche et droite pour vous déplacer dans le jeu.
Shift : Appuiez sur la touche Shift pour courir.
Shift en l'air : Si vous êtes en l'air, maintenir Shift permettra de réaliser un saut plus long.
Restart : Si vous tombez dans le vide, appuyez sur R. Cela vous fera réapparaitre à l'endroit d'origine.
Ennemis : Les boules violettes sont des ennemis que vous pouvez manger pour vous frayer un chemin vers la sortie. Foncez leur dessus pour les tuer !
Porte dorée : Une fois que vous avez atteint la porte dorée du jeu, maintenez la touche E pour terminer le niveau et afficher le message de victoire.

Ces commandes permettent de contrôler votre personnage et d'interagir avec les éléments du jeu pour progresser. Bonne chance !


## Résolution des problèmes
Si vous rencontrez des problèmes pendant l'installation ou l'exécution, voici quelques solutions possibles :

Problème de dépendances raylib : Assurez-vous que raylib est correctement installé et que votre environnement de développement (CMake, compilateur, etc.) peut y accéder.
Problème de compilation avec CMake : Vérifiez que vous utilisez une version compatible de CMake et que le compilateur C++ est correctement configuré.
Problème d'exécution : Si le jeu ne se lance pas, assurez-vous que toutes les dépendances sont installées et que la compilation a réussi sans erreur.
