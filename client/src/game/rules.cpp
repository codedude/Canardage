
#include "game/rules.hpp"

#include <memory>
#include <utility>

using namespace std;

namespace Game {
    //ID card => nb of cards, name
    std::pair<int, std::string> const Rules::cards[] = {
        std::make_pair(6,   "Canard rouge"),
        std::make_pair(6,   "Canard vert"),
        std::make_pair(6,   "Canard bleu"),
        std::make_pair(6,   "Canard orange"),
        std::make_pair(6,   "Canard jaune"),
        std::make_pair(6,   "Canard rose"),
        std::make_pair(5,   "Carte eau"),
        std::make_pair(13,  "Pan"),
        std::make_pair(11,  "En joue"),
        std::make_pair(6,   "En avant marche"),
        std::make_pair(3,   "Canard presse"),
        std::make_pair(3,   "Canard blase"),
        std::make_pair(3,   "A couvert"),
        std::make_pair(2,   "Rectifier le tir <-"),
        std::make_pair(2,   "Rectifier le tir ->"),
        std::make_pair(2,   "Planque"),
        std::make_pair(1,   "Walking duck"),
        std::make_pair(1,   "2 pour le prix d'1"),
        std::make_pair(1,   "Ducky Luck"),
        std::make_pair(1,   "Danse des canards"),
        std::make_pair(1,   "Oups"),
        std::make_pair(1,   "Canards enchaines"),
        std::make_pair(1,   "Fulguro coin"),
        std::make_pair(1,   "Peace and love"),
        std::make_pair(0,   "Canarchie")
    };
    //ID card => card description
    std::string const Rules::cards_desc[] = {
        "Canard rouge",
        "Canard vert",
        "Canard bleu",
        "Canard orange",
        "Canard jaune",
        "Canard rose",
        "Carte eau",
        "Abattez un canard mis en joue. La cible est enlevee et le canard "
        "abattu est place devant son proprietaire.",
        "Posez une cible au dessus du canard de votre choix. Il est desormais "
        "a la merci des tireurs !",
        "Deplacez toute la ligne d'un emplacement vers l'avant. La premiere "
        "carte est remise sous la pile et une nouvelle vient occuper le dernier"
        " emplacement.",
        "Echangez l'un de vos canards avec la carte qui se trouve immediatement"
        " devant lui.",
        "Inversez l'un de vos canards avec la carte qui se trouve immediatement"
        " derriere lui.",
        "Placez cette carte sur n'importe quel canard de la ligne. Il est "
        "protege jusqu'a ce que la carte soit retiree au debut de votre "
        "prochain tour.",
        "Deplacez une cible d'une case vers la gauche (si cet emplacement n'est"
        " pas deja en joue).",
        "Deplacez une cible d'une case vers la droite (si cet emplacement n'est"
        " pas deja en joue).",
        "Cachez un de vos canards sous un canard voisin. La ligne avance pour "
        "combler le vide. Tant que le canard qui vous protege se deplace dans "
        "la ligne, vous restez cache a l'abri derriere lui.",
        "Choisissez un canard qui a ete abattu et remettez-le en dessous de la "
        "pile de cartes Canard.",
        "Placez deux cibles sur 2 emplacements adjacents.",
        "Tirez sur n'importe quelle carte de la ligne de tir, qu'elle soit "
        "mise en joue ou non.S'il y avait une cible, elle est retiree.",
        "Retirez toutes les cartes de la ligne et remettez-les dans la pile qui"
        " est ensuite melangee. Replacez 6 nouvelles cartes sur la ligne. Les "
        "cibles restent en place.",
        "Abattez n'importe quelle carte voisine d'une carte mise en joue. La "
        "cible est enlevee. Si la carte abattue etait deja mise en joue, sa "
        "cible reste en place.",
        "Abattez 2 canards adjacents mis en joue. Les 2 cibles sont enlevees et"
        " les canards abattus place face a leurs proprietaires. La ligne avance"
        " de 2 cases.",
        "Deplacez l'un de vos canards de la ligne en 1iere position et deplacez"
        " les autres derriere vous pour remplir la ligne.",
        "Retirez toutes les cibles de la ligne de tir.",
        "Melangez les canards de la ligne dans l'ordre que vous voulez. Toutes "
        "les cibles restent en place."
    };

    //Nb of water cards
    int const Rules::cards_water[] = {0, 2, 3, 3, 4, 5};
    //Nb player min, max, nb cards in hand
    int const Rules::info_player[] = {1, 6, 4};
    //Nb of slots on boardgame
    int const Rules::board_slots = 6;

}