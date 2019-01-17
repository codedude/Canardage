################################Projet Canardage################################
PARIS Valentin et GILLES Sammy, L3 Informatique

Programmes dévoloppés sous Linux (Ubuntu et Debian). Ils ne sont pas
compatibles Windows.
Langage : c++11 avec g++ à jour


Pour compiler : make
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

