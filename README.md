# Introduction

Ce code montre comment écrire une image en BMP sans compression.

**Attention, ce code ne tient pas compte de l'endianness, il présuppose être exécuté sur une plateforme little-endian.**

# Compilation

Pour compiler cet exemple, vous pouvez suivre la suite de commandes suivantes :
```bash
mkdir build
cd build
cmake ..
make
```

Ensuite, vous pouvez exécuter le programme créé :
```bash
./gen_image
```

Cela va créer une image `test.bmp` dans le répertoire où a été exécuté le programme.


# Informations supplémentaires

Ce dépôt contient les exemples de programmes que nous étudions au cours d’un série de vidéo YouTube dédiée à l’apprentissage de techniques de base d’informatique graphique. Pour voir les vidéos, rendez-vous sur https://www.youtube.com/channel/UCD4KePGBPe1QHb_iXhAqBCg

Pour chaque vidéo, il existe une branche contenant le code écrit au cours de la vidéo.
