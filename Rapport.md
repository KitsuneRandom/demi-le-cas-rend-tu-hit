# Rapport SAE 4.3 - Back to 2048
Abel GOMES - Mathis DE AZEVEDO - Lucie TRIPIER

## difficultés 

| Difficulté | Conséquence | 
| -------------|------------- |
| Refonte de la version originale : De base, on lance un main qui lance le processus 2048. On devait avoir un seul processus 2048, donc on a du séparer complètement les 2 processus. | Perte de temps : le temps de s’en rendre compte + le temps de le corriger. |
| Difficultés d’organisation (on est répartis dans les 3 TDs donc pas fou pour se voir + on a énormément de trucs à rendre en 2 semaines). | Répartition des tâches très inégale et perte de temps lorsqu’un membre du groupe était bloqué et devait attendre les autres. |
| En séparant le processus 2048 et le processus main, on se retrouve avec 2048 qui attend en lecteur de pipe sans écrivain. | Pas de conséquence notable, on a trouvé une solution alternative : la mise en place d’un poll. |

## répartition des tâches
Abel : étape 1, étape 2, étape 3, tests

Mathis : tentatives désastreuses sur l’étape 1, documentation, tests

Lucie : rapport, préparation de la soutenance

## Temps de travail
Abel : 8h

Mathis : 5h inutiles, 2h utiles

Lucie : 1h

## les fonctionnalités qui marchent et celles qui ne marchent pas
### Ce qui marche : 

| étape | description |
|----|----|
| Etape 1 | 1 processus 2048, plusieurs processus main, 1 pipe nommé, 1 processus d’affichage. |
| Etape 2 |  Un sémaphore protège l’accès au segment de mémoire partagée (et au pipe au passage).|
| Etape 3 | Un segment de mémoire partagée stocke les données de toutes les parties . |

### Ce qui ne marche pas : 

| étape | description |
|----|----|
| Etape 2 |  Un segment de mémoire partagée qui stocke les données d’une seule partie : on est passé directement à l’étape 3 par manque de temps, et parce que c’est la solution qui paraissait la plus simple et logique.|
