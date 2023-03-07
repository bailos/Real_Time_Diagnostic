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
#### a- Les capteurs
- Capteur de temperature DS18B20

Le capteur de temperature DS18B20 nous permet de prelever la temperature en temp reel. Son principe est de communique sur un bus à un fil (onewire) qui, par définition, ne nécessite qu'une seule ligne de données (et une terre) pour communiquer avec un microprocesseur central. Utilisable avec les systèmes 3.0-5.5V. Le câble est gainé de PVC, nous vous suggérons donc de le garder sous 100 degrés max
- Capteur de luminosite TSL2561 

Le capteur de luminosité TSL2561 est un capteur de lumière numérique avancé, idéal pour une utilisation dans une large gamme de situations lumineuses. Comparé aux cellules CdS à faible coût, ce capteur est plus précis, permettant des calculs de lux exacts et peut être configuré pour différentes plages de gain/temporisation afin de détecter des plages de lumière allant de 0,1 - 40 000+ Lux à la volée. La meilleure partie de ce capteur est qu'il contient à la fois des diodes infrarouges et à spectre complet. Cela signifie que nous pouvons mesurer séparément la lumière infrarouge, à spectre complet ou visible par l'homme. La plupart des capteurs ne peuvent détecter que l'un ou l'autre, ce qui ne représente pas exactement ce que l'œil humain voit (puisque nous ne pouvons pas percevoir la lumière IR détectée par la plupart des photodiodes)

- Capteur de courant ACS712

Ce capteur nous permet de mesuer le courant reel fourni par le module PV.
Le principe de ce capteur de courant est basé sur un circuit à effet hall ACS724 permettant de mesurer un courant continu de 0 A à 30 A. Une sortie analogique dédiée, proportionnelle au courant mesuré (500 mV pour 0 A sous 5 Vcc) est alors orientée vers un circuit de conversion numérique (nombre de bit) en temps réel.

- Capteur de tension DCO-25V

Gracw a ce capteur nous avons la tension reel fourni par le module.
Le principe de ce capteur est basé sur un principe de séparation résistif, qui consiste à réduire la tension d'entrée du terminal de l'interface 5 fois ; la tension d'entrée analogique coté Arduino va jusqu'à 5 V ; la tension d'entrée du module de détection ne peut pas être supérieure à 5 V × 5 = 25 V. Nous avons choisi ce module qui est moins efficace que bien d’autre car il est facilement maniable, ce qui facilite nos teste qui ne nécessite pas de grande tension.


