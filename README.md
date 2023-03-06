# CONTENU
- ## INTRODUCTION
- ## PROBLEMATIQUE
- ## PRESENTATION DE LA MAQUETTE
- ## LES DIFFERENTES PARTIES
- ## RESULTATS OBTENUES ET INTERPRETATIONS

## I- INTRODUCTION
L'etude d'une solution Internet of Things en francais objet connecte passe forcement
une etude d'un systeme embarque qui decoule d'une instrumentation
Dans l'etude d'un systeme operationnel temps reel (RTOS), un regard minutier sur les elements qui constitue le systeme est capital a savoir, les composant de puissance, les capteur, la batterie et le systeme de controle et traiment des donnees.
Dans un systeme PV plusieurs capteurs peuvent etre sourcew de defaut qui impactera considerablement, la production des module une fois deployer.
En Afrique nous importons plusieurs module PV, mais ces dernier ne satisfait a en production. cette problematique nous couduit a plusieurs interrogation a savoir :
Le manque suivie du veillissement des modules est il source de nos probleme?
Les defaut internes comme defaillamce de diode sont la principale source
Pour repondre a ces interrogation et bien d'autre que nous n'avons pas citer nous avons
developper un systeme de dignostique embarque au module PV qui permettra de prelever essentiellement des grandeurs comme la tension, le courant, la temperature, et l'ensoleilement, Ces donnes sont ensuite transmis suur um serveur distant afin d'utiliser des model de machine learning ou de deep learning pour faire une analyse profonde afin de situer les defaut en prodution.

## II- PROBLEMATIQUE

## III- PRESENTATION DE LA MAQUETTE
Le processus d'acquisition des donnees est fourni par le schema de principe representer a la figure "f2".Les donnees sont mesurer en temps reel et envoyer sur le cloud computing. Ce sont : la temperature, le courant fourni par le module, la tension reellement debuter et l'ensoleillement recu par le module PV. Le programme d'acquisition de donnees est developper sous arduino. Les donnees sont par la suite traiter par un algorithme de machine learning afin de pouvoir identifier et localiser un defaut dans un champ PV. 
## IV- PRESENTATION DE LA MAQUETTE
Le systeme est constituer de trois partie essentielle a savoir :
- La partie instrumentation qui contient les capteur necessaire pour la mesure des grandeurs.
- La partie acquisition de donnees, les donnees recue des capteurs sont organiser et ensuite stocker dans un fichier au format csv sur une carte SD. Ces dernieres sont ensuite transfere sur un cloud a travers un module internet.
- La parie traitement des donnees : les organiser au fichier csv avec une entete representatif des differents grandeurs est pretraiter a travers les technique de dataprocessing et ensuite subit une extration d'intelligence a travers un alghorithme de machine learning et les resultats sont fourni sur un tableau de bord.

Schema de montage 



### 1- INSTRUMENTATION


