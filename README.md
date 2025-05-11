# Rapport de Projet : Synthèse d’Images — A Railroad and a Train

FOURRE Anaëlle
M1 Informatique

## Objectif du projet
   
Ce projet a pour objectif de concevoir une petite simulation ferroviaire 3D interactive, permettant de visualiser un circuit ferroviaire défini dynamiquement à partir d’un fichier JSON. Il vise à appliquer les concepts de modélisation 3D et d’illumination à travers une scène complexe.

## Manuel utilisateur

### Compilation

Pour compiler le projet, il faut exécuter la commande suivante dans le terminal :

```bash
mkdir build
cd build
cmake ../
make
```
### Exécution

Pour exécuter le projet, il suffit d'exécuter la commande suivante dans le terminal :

```bash
cd bin
./src_main <nom_fichier_json>
```
Le nom du fichier de configuration JSON doit être donné en argument. Il doit être placé dans le dossier `grids`.

### Commandes disponibles

Il est possible de se déplacer dans la scène 3D à l'aide des touches suivantes :

- `flèche du haut` : avancer
- `flèche du bas` : reculer
- `flèche de droite` : aller à gauche
- `flèche de gauche` : aller à droite
- `f` : monter
- `d` : descendre
- `c` : tourner à gauche
- `v` : tourner à droite

Il est de plus possible de réaliser les actions suivantes :

- `q` : quitter la scène
- `l` : n'afficher que les lignes 
- `p` : afficher le remplissage des polygones
- `r` : switcher entre le flat shading et l'illumination réaliste
- `s` : faire avancer/stopper le train

## Fonctionnalités implémentées

### Modélisation du circuit ferroviaire

Le circuit de rails est modélisé à partir de la lceture d'un fichier json. Ce dernier donne l'emplacement de chaque rail dans l'ordre du circuit (qui doit un chemin fermé et sans trous).
Les rails sont ensuite modélisés à partir de deux objets : un rail droit et un rail courbe, qui respectent les dimensions du sujet. Ces modélisations peuvent être trouvées dans le fichier `draw_tracks.cpp`.

### Modélisation du train et de la gare

Le train et la gare sont également placés sur le circuit à partir des informations données dans le fichier json.
Le train occupe la taille d'une case et la gare occupe la taille de 4 cases.

Ces deux objets sont modélisés à partir d'objets 3D simples. De plus, j'ai choisi de texturer les murs de la gare pour plus de réalisme. Ce dernier est le seul élément structuré de ma scène.

On retrouve également une lumière à l'avant du train, qui est une lumière ponctuelle (et directionnelle : voir ci-dessous).

### Illumination

Deux modes d’illumination sont disponibles :

- Flat shading (mode par défaut).
- Illumination réaliste avec une lumière de type `spotlight` à l'avant du train et une lumière directionnelle de faible intensité venant d'un point à l'infini au dessus de la scène.

La lumière placée à l'avant du train est à la fois ponctuelle et directionnelle, dans le sens où la luminosité de des objets dépendent à la fois de la distance à la source lumineuse et de l'angle formée avec cette source lumineuse. Pour cela, j'ai choisi de modifier légèrement le shader afin qu'il puisse prendre une lumière ayant pour quatrième coordonnée -1. Cela siginifie alors que cette lumière est de type spotlight, et que sa direction est la position de la lumière suivante.


### Améliorations réalisées 

Afin d'améliorer la scène 3D, j'ai choisi d'animer le train afin qu'il puisse avancer en suivant le circuit. Je  ne suis pas parvenu à réaliser totalement cette amélioration. En effet, le train ne parvient pas à suivre le circuit sur certains angles, et la lumière ne suit pas le train lorsqu'il avance.
