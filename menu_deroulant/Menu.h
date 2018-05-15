#ifndef _MENU_H_
#define _MENU_H_

/* Structure d'un menu - Menu_t est la représentation en mémoire d'un menu */
typedef struct {
  const char* prompt; // titre du menu
  const char** items; // tableau de choix du menu
  const uint8_t nbItems; // nombre de choix possibles
  void (*callbackFnct) (uint8_t menuItemSelected); //pointeur sur fonction pour la validation
  /* c'est un pointeur qui peut pointer vers n'importe quelle fonction de la forme void |______| (byte) */
} Menu_t;

/* liste des touches de la shield lcd with key  -  enum = énumération */
/*
    select / left : pour un retour au menu précédent
    right : selectionner un choix du menu, ou ouvrir le sous-menu : validation
    haut / bas : naviguer dans la liste de choix
*/
typedef enum {
  BP_NONE, // aucun bouton appuyé
  BP_SELECT, // bouton select
  BP_LEFT, // bouton gauche
  BP_RIGHT, // bouton droite
  BP_UP, // bouton haut
  BP_DOWN, // bouton bas
} Button_t;

#endif /* _MENU_H_ */
