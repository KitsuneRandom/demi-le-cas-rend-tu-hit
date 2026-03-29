# demi le cas rend tu Hit - version Zero

## Description
Jeu 2048 jouable sur terminal. bougez les tuiles avec zqsd jusqu'à atteindre la tuile 2048. a la fin du jeu (plus de place ou tuile 2048 crée), le jeu affichera votre score, calculé en fonction des tuiles que vous avez assemblées.
<br>
Dans cette version, on peut jouer plusieurs parties en même temps depuis plusieurs terminaux.

## Jeu
![Le jeu](image.webp)<br>
Le but de ce projet est de recréer le célèbre jeu 2048 dans le langage C en utilisant des processus différents communiquant entre eux pour réaliser les différentes fonction de jeu.
<br>

## installation et lancement
```bash
$ git clone git@git.unistra.fr:m.deazevedo/demi-le-cas-rend-tu-hit.git

$ cd demi-le-cas-rend-tu-hit/

$ make fclean

$ make

$ cd bin/game

$ ./2048

$ cd ..

$ ./demi_le_cas             // répéter cette commande pour chaque partie à lancer
```
<br>

## Contributions
Abel GOMES : Communication entre les processus, déroulement des partie

Lucie TRIPIER : Fonctions de jeu, affichage en couleurs, README + rapport

Mathis DE AZEVEDO : Fonctions de jeu, fonctions d'affichage, interactions avec l'utilisateur README + rapport


## License
MIT License<br>