# API embase CdR - V3 (beta)

**A revoir**

## Sommaire

- [API embase CdR - V3 (beta)](#api-embase-cdr---v3-beta)
  - [Sommaire](#sommaire)
  - [Objectifs](#objectifs)
  - [Forme des commandes envoyées à la STM32](#forme-des-commandes-envoyées-à-la-stm32)
  - [Définition des commandes valides](#définition-des-commandes-valides)
    - [Destination : **task** (file de tâches)](#destination--task-file-de-tâches)
      - [**\[append/insert\]\_goto**](#appendinsert_goto)
      - [**\[append/insert\]\_move**](#appendinsert_move)
      - [**\[append/insert\]\_wait**](#appendinsert_wait)
      - [**\[append/insert\]\_send**](#appendinsert_send)
      - [**execution**](#execution)
    - [Destination : **config** (variables de configuration)](#destination--config-variables-de-configuration)
      - [**get**](#get)
      - [**set**](#set)
    - [Destination : **robot**](#destination--robot)
      - [**get**](#get-1)
      - [**set**](#set-1)

## Objectifs

L'objectif de cet API est de définir un moyen de communiquer avec une STM32 reliée à une embase holonum (stepper x 3) afin de la contrôler depuis l'extérieur (via liaison UART).

A priori moteurs contrôlés par L432KC et Servo42C.

## Forme des commandes envoyées à la STM32

Les commandes seront de la forme suivante :

```bash
command_destination command_name arg1 arg2 arg3 ...
```

Maybe faire une API python ou C++ qui donnerait :

```py
import EmbaseAREM

embase = EmbaseAREM(COM7) # Définition d'une embase par son port

embase.command_destination.command_name(arg1, arg2, ...)
```

**La fin d'une commande est toujours un retour à la ligne `\n` ou un `\r`**.

Les arguments sont des entiers ou des strings, détection automatique selon la commande utilisée

Avec : 

| Nom de l'argument    | Signification                              | Exemple                                       |
| -------------------- | ------------------------------------------ | --------------------------------------------- |
| command\_destination | Partie du code visée par la commande       | task (file de tâches), config (configuration) |
| command\_name        | Nom de la commande ou de la variable visée | get, set, append_move, insert_wait            |
| argn                 | arguments                                  | 100, hello, F                                 |

Exemples de commandes :

```bash
task insert_goto 0 0 # Insert the task [Go to coordinates (0, 0)] as the next task in the task queue
config set team B # Set team to B
task append_send done # Append the task [Send "done" through UART] at the end of the task queue
robot get x # get current x coordinate
```

Exemples sur l'API python / C++ :

```py
embase.task.insert_goto(0, 0, 0)
embase.config.set("team", 0)
embase.task.append_send("done")
embase.robot.get("x")
```

## Définition des commandes valides

### Destination : **task** (file de tâches)

Types de commandes valides :


#### **\[append/insert\]_goto**

Permet de se déplacer vers un point. L'origine (x = 0, y = 0, theta = 0) est la position **de départ** du robot.

Coordonnées :

- _x_ vers la droite de la position de départ du robot
- _y_ vers l'avant de la position de départ du robot

Args : 

| Nom     | Type | Valeur par défaut          | Description              |
| ------- | ---- | -------------------------- | ------------------------ |
| _x_     | int  | _valeur actuelle de x_     | Coordonnée selon x en mm |
| _y_     | int  | _valeur actuelle de y_     | Coordonnée selon y en mm |
| _theta_ | int  | _valeur actuelle de theta_ | Angle en °               |

Exemple :

```bash
task insert_goto 1000 0 0 # Go to (1000, 0) and rotate to 0°
```

#### **\[append/insert\]_move**

Permet de se déplacer vers un point. L'origine (x = 0, y = 0, theta = 0) est la position **actuelle** du robot.
Il s'agit donc d'un déplacement relatif.

Coordonnées :

- _x_ vers la droite du robot
- _y_ vers l'avant du robot

Args : 

| Nom     | Type | Valeur par défaut | Description              |
| ------- | ---- | ----------------- | ------------------------ |
| _x_     | int  | 0                 | Coordonnée selon x en mm |
| _y_     | int  | 0                 | Coordonnée selon y en mm |
| _theta_ | int  | 0                 | Angle en °               |

Exemple :

```bash
task insert_move 1000 0 0 # insert [Move right 1m]
task append_move 0 0 180 # append [Turn 180°]
```

#### **\[append/insert\]_wait**

Ajoute un temps d'attente en ms.

Args : 

| Nom     | Type | Valeur par défaut | Description           |
| ------- | ---- | ----------------- | --------------------- |
| _t\_ms_ | int  | 0                 | Temps d'attente en ms |

Exemple :

```bash
task insert_wait 1000 # insert [Wait 1s]
```

#### **\[append/insert\]_send**

Ajoute un envoi de données par UART.

Args : 

| Nom    | Type       | Valeur par défaut | Description      |
| ------ | ---------- | ----------------- | ---------------- |
| _data_ | int/string | "" (string vide)  | Donnée à envoyer |

Exemple :

```bash
task append_send done # append [Send "done" through UART]
task append_send 150 # append [Send "150" through UART]
```

#### **execution**

Lance ou arrête l'exécution de tâches depuis la file.

| Nom      | Type      | Valeur par défaut | Description              |
| -------- | --------- | ----------------- | ------------------------ |
| _action_ | bool/enum | NONE              | Active/désactive la file |

Exemple :

```bash
task execution start
task execution 0
```

### Destination : **config** (variables de configuration)

#### **get** 

Permet de récupérer une variable de configuration (indépendante de l'état du robot), par exemple `team`, `strategy`...

Args : 

| Nom        | Type   | Valeur par défaut | Description                                               |
| ---------- | ------ | ----------------- | --------------------------------------------------------- |
| _variable_ | string | help              | Nom de la variable. "help" affiche la liste des variables |

Exemple :

```bash
config get help
config get team
```

#### **set**

Permet de mettre à une certaine valeur une variable de configuration (indépendante de l'état du robot), par exemple `team`, `strategy`...

Args : 

| Nom        | Type               | Valeur par défaut | Description                                               |
| ---------- | ------------------ | ----------------- | --------------------------------------------------------- |
| _variable_ | string             | help              | Nom de la variable. "help" affiche la liste des variables |
| _valeur_   | string/int/enum... | _valeur actuelle_ | valeur à forcer                                           |

Exemple :

```bash
config set help # Affiche un menu d'aide
config set team blue
config set strategy 1
```

### Destination : **robot**


#### **get** 

Permet de récupérer une variable d'état , par exemple `x`, `theta`...

Args : 

| Nom        | Type   | Valeur par défaut | Description                                               |
| ---------- | ------ | ----------------- | --------------------------------------------------------- |
| _variable_ | string | help              | Nom de la variable. "help" affiche la liste des variables |

Exemple :

```bash
config get help
config get x
```

#### **set**

Permet de mettre à une certaine valeur une variable d'état , par exemple `x`, `theta`...

Args : 

| Nom        | Type               | Valeur par défaut | Description                                               |
| ---------- | ------------------ | ----------------- | --------------------------------------------------------- |
| _variable_ | string             | help              | Nom de la variable. "help" affiche la liste des variables |
| _valeur_   | string/int/enum... | _valeur actuelle_ | valeur à forcer                                           |

Exemple :

```bash
config set help # Affiche un menu d'aide
config set x 0
config set theta 360
```
