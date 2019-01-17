################################Projet Canardage################################
PARIS Valentin et GILLES Sammy, L3 Informatique

Programmes dévoloppés sous Linux (Ubuntu, Debian 64bits). Ils ne sont pas
compatibles Mac, Windows, et probablement RedHat/BSD.
Langage : c++11 avec g++ à jour


Pour compiler : make (make -j 4 pour utiliser plusieurs threads)
Pour nettoyer : make clean
Pour vérifier les fuites mémoires : make valg


###Serveur###
Pour executer   : ./canardage IP PORT MODE
Pour executer en local  : ./canardage 127.0.0.1 5650 0
Biblis tierces  : aucune

###Client###
Pour executer   : ./canardage IP PORT
Pour executer   : ./canardage 127.0.0.1 5650
Biblis tierces  : libncurses5, libncurses5-dev

=> Lancer le serveur, puis les clients.

###Commandes###
Se déplacer     : Flèches directionnelles
Valider         : Entrée
Annuler/Retour  : q
Quitter         : Echap


### Infos ###
Les programmes compilent SANS erreurs NI warnings. Si il y en a, cela vient de
votre configuration.
En fonctionnement normal (c-à-d le processus n'est pas arreté manuellement), il
n'y a PAS de fuites mémoires (en dehors de celle de ncurses, qui sont normales).


###Rapport###
Le rapport au format pdf, les diagrammes UML ainsi que des infos sur le jeu sont
dans le dossier "report"

