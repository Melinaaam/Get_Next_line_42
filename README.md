# Get Next Line

> Because reading a line from a file descriptor is way too tedious.

## 1. Comprendre le sujet

Le but de ce projet est d’implémenter une fonction permettant de lire **une ligne** depuis n’importe quel descripteur de fichier (FD) ou depuis l’entrée standard (STDIN, c’est-à-dire FD = 0).  
L’enjeu principal est de **gérer efficacement la mémoire** et de **respecter les contraintes** imposées par les tampons de lecture (`BUFFER_SIZE`) et par les différents descripteurs de fichiers.

---

## 2. Lire et stocker les données

### 2.1 Lecture de n’importe quel fichier

Pour récupérer les données, on utilise la fonction `read()` qui lit un nombre d’octets défini par la macro `BUFFER_SIZE` :

1. Si `read()` renvoie `0`, on a atteint la **fin du fichier** (EOF).  
2. Si `read()` renvoie `-1`, une **erreur** est survenue.  
3. Sinon, on stocke les données lues dans un **buffer dynamique**.

### 2.2 Gestion du buffer et stockage des données

Le **buffer** ne doit pas être réinitialisé entre les appels à `get_next_line()` afin de conserver les données déjà lues mais non encore renvoyées. Pour cela, on utilise généralement une variable **statique** (ex. `static char *remains`) qui contiendra :

- Les **données résiduelles** après le saut de ligne `"\n"` trouvé dans le buffer.  
- Les **fragments** de texte nécessaires à la formation de la ligne suivante.

Concrètement, on concatène les **nouveaux morceaux** lus jusqu’à :

- Rencontrer un **saut de ligne** (`"\n"`).  
- Ou **atteindre la fin du fichier**.

Une fois qu’une ligne est délimitée, on la **retourne** et on met à jour `remains` avec la partie du buffer qui se situe **après** cette ligne. 

---

## 3. Extraire une ligne complète

Une ligne est considérée complète lorsqu’on :

- Rencontre un `"\n"` dans le buffer.  
- Ou qu’on arrive à la **fin du fichier**.

On peut créer une **fonction d’extraction** qui parcourt le buffer jusqu’au `"\n"` et renvoie une **sous-chaîne** correspondant à la ligne. Le buffer est ensuite mis à jour pour être prêt à recevoir ou traiter de nouvelles données lors des appels suivants.

---

## 4. Nettoyer la mémoire

Pour éviter les **fuites de mémoire** :

- On **libère** le buffer (`remains`) lorsque la lecture est terminée.  
- Une fois la **dernière ligne** retournée, le buffer doit également être libéré.  
- Si une **erreur** survient en cours de route (par exemple `read()` renvoie `-1`), on **libère** avant de renvoyer `NULL`.

---

## 5. Gestion de plusieurs descripteurs (Bonus)

Le bonus du projet demande de **gérer simultanément plusieurs descripteurs de fichiers** (FD).  
Pour cela, on associe un **espace de stockage distinct** à chaque FD. Ainsi :

- Les appels à `get_next_line(fd1)` et `get_next_line(fd2)` manipulent chacun leur **propre buffer**.  
- Chaque fichier ouvert **conserve** son avancement et ses données résiduelles indépendamment.

Grâce à cette approche, `get_next_line()` devient un outil encore plus **polyvalent** et **efficace** pour la lecture ligne par ligne sur plusieurs fichiers ou flux.

---

## Récapitulatif

- **`BUFFER_SIZE`** : Nombre d’octets lus à chaque appel de `read()`.  
- **`static char *remains`** : Espace de stockage qui retient les données non encore utilisées ou la partie déjà lue après un saut de ligne.  
- **Extraction de la ligne** : Se fait en découpant jusqu’au `"\n"` ou jusqu’à la fin du buffer.  
- **Nettoyage mémoire** : Libérer systématiquement les buffers quand la lecture se termine ou en cas d’erreur.  
- **Bonus** : Gérer plusieurs descripteurs en parallèle, chacun ayant son propre espace de stockage.

Avec ces principes, votre fonction `get_next_line()` pourra lire **n’importe quel fichier** ou l’**entrée standard**, ligne après ligne, tout en restant **robuste**, **efficace** et **facile** à maintenir.

