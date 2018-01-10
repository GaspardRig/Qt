#ifndef INCLUDE_H
#define INCLUDE_H

/////////////////// PROTOCOLE DE COMMUNICATION /////////////////////

/* Command list
 * MOVE :
 *      up : UP
 *      down  : DW
 *      right : RG
 *      left : LF
 *      up_right :UR
 *      up_left :UL
 *      down_right :DR
 *      down_left :Dl
 * DATA :
 *      send_matrice :
 *      receive_matrice : <ALL> (à changer en 2octets)
 * SIZE :
 *      2octects
 *      matrice 24 x 24
*/

struct {
    int id_cmd;
    int  length;          // longueur de la data, permet de savoir quand on a tout recu
    char*  data;         // donné
    int  crc;           // checksum pour valider la trame seulement dans le cas de la matrice
}

#endif // INCLUDE_H
