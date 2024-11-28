# API embase CdR - V2

**A revoir**

## Sommaire

- [API embase CdR - V2](#api-embase-cdr---v2)
  - [Sommaire](#sommaire)
  - [Objectifs](#objectifs)
  - [Forme des commandes envoyées à la STM32](#forme-des-commandes-envoyées-à-la-stm32)
  - [Définition des commandes valides](#définition-des-commandes-valides)
    - [Destination : **task** (file de tâches)](#destination--task-file-de-tâches)
      - [\[append/insert\] **goto**](#appendinsert-goto)
      - [\[append/insert\] **move**](#appendinsert-move)
      - [\[append/insert\] **wait**](#appendinsert-wait)
      - [\[append/insert\] **send**](#appendinsert-send)
      - [\[execution\] **start**](#execution-start)
      - [\[execution\] **stop**](#execution-stop)
    - [Destination : **config** (variables de configuration)](#destination--config-variables-de-configuration)
      - [\[get/set\]](#getset)
    - [Commande spéciale : \[get\] help](#commande-spéciale--get-help)
    - [Destination : **robot**](#destination--robot)
      - [\[get/set\]](#getset-1)

## Objectifs

L'objectif de cet API est de définir un moyen de communiquer avec une STM32 reliée à une embase holonum (stepper x 3) afin de la contrôler depuis l'extérieur (via liaison UART).

A priori moteurs contrôlés par L432KC et Servo42C.

## Forme des commandes envoyées à la STM32

Les commandes seront de la forme suivante :

```bash
command_destination command_type command_name arg1 arg2 arg3 ...
```

Maybe faire une API python ou C++ qui donnerait :

```py
import EmbaseAREM

embase = EmbaseAREM(COM7) # Définition d'une embase par son port

embase.command_destination.command_type(command_name, arg1, arg2, ...)
```

**La fin d'une commande est toujours un retour à la ligne `\n`**.

Les arguments sont :

- des entiers par défaut
- des strings si ils commencent par `"`

Avec : 

| Nom de l'argument    | Signification                               | Exemple                                        |
| -------------------- | ------------------------------------------- | ---------------------------------------------- |
| command\_destination | Partie du code visée par la commande       | task (file de tâches), config (configuration) |
| command\_type        | Type de la commande                         | get, set, append, insert                       |
| command\_name        | Nom de la commande ou de la variable visée | wait , send, team, microstep                   |
| argn                 | arguments (tous optionnels)                 | 100, "hello", F                                |

Exemples de commandes :

```bash
task insert goto 0 0 # Insert the task [Go to coordinates (0, 0)] as the next task in the task queue
config set team B # Set team to B
task append send "done" # Append the task [Send "done" through UART] at the end of the task queue
robot get x # get current x coordinate
```

Exemples sur l'API python / C++ :

```py
embase.task.insert("goto", 0, 0)
embase.config.set("team", 0)
embase.task.append("send", "done")
embase.robot.get("x")
```

## Définition des commandes valides

### Destination : **task** (file de tâches)

Types de commandes valides :

- **append** (ajout en fin de file)
- **insert** (ajout remplaçant la prochaine tâche à exécuter, décale les autres)
- **execution** (gestion de l'exécution des taches, s'applique à l'ensemble de la file)

#### \[append/insert\] **goto**

Permet de se déplacer vers un point. L'origine (x = 0, y = 0, theta = 0) est la position **de départ** du robot.

Coordonnées :

- _x_ vers la droite de la position de départ du robot
- _y_ vers l'avant de la position de départ du robot

Args : 

| Nom       | Type | Valeur par défaut           | Description               |
| --------- | ---- | ---------------------------- | ------------------------- |
| _x_     | int  | _valeur actuelle de x_     | Coordonnée selon x en mm |
| _y_     | int  | _valeur actuelle de y_     | Coordonnée selon y en mm |
| _theta_ | int  | _valeur actuelle de theta_ | Angle en °               |

Exemple :

```bash
task insert goto 1000 0 0 # Go to (1000, 0) and rotate to 0°
```

#### \[append/insert\] **move**

Permet de se déplacer vers un point. L'origine (x = 0, y = 0, theta = 0) est la position **actuelle** du robot.
Il s'agit donc d'un déplacement relatif.

Coordonnées :

- _x_ vers la droite du robot
- _y_ vers l'avant du robot

Args : 

| Nom       | Type | Valeur par défaut | Description               |
| --------- | ---- | ------------------ | ------------------------- |
| _x_     | int  | 0                  | Coordonnée selon x en mm |
| _y_     | int  | 0                  | Coordonnée selon y en mm |
| _theta_ | int  | 0                  | Angle en °               |

Exemple :

```bash
task insert move 1000 0 0 # insert [Move right 1m]
task append move 0 0 180 # append [Turn 180°]
```

#### \[append/insert\] **wait**

Ajoute un temps d'attente en ms.

Args : 

| Nom       | Type | Valeur par défaut | Description           |
| --------- | ---- | ------------------ | --------------------- |
| _t\_ms_ | int  | 0                  | Temps d'attente en ms |

Exemple :

```bash
task insert wait 1000 # insert [Wait 1s]
```

#### \[append/insert\] **send**

Ajoute un envoi de données par UART.

Args : 

| Nom      | Type       | Valeur par défaut | Description        |
| -------- | ---------- | ------------------ | ------------------ |
| _data_ | int/string | "" (string vide)   | Donnée à envoyer |

Exemple :

```bash
task append send "done" # append [Send "done" through UART]
task append send 150 # append [Send "150" through UART]
```

#### \[execution\] **start**

Lance l'exécution de tâches.

Exemple :

```bash
task execution start
```

#### \[execution\] **stop**

Arrête l'exécution de tâches.

Args : 

| Nom              | Type                         | Valeur par défaut | Description                                                                                                 |
| ---------------- | ---------------------------- | ------------------ | ----------------------------------------------------------------------------------------------------------- |
| save\_unfinished | int (bool mais c'est un int) | true (aka 1)       | Si mis à `true`, la partie restante d'une tâche interrompue sera insérée automatiquement dans la file |

Exemple :

```bash
task execution stop
```

### Destination : **config** (variables de configuration)

Types de commandes valides :

- **get** (récupère la valeur de la variable)
- **set** (modifie la valeur de la variable)

#### \[get/set\] 

Permet de gérer une variable de configuration (indépendante de l'état du robot), par exemple `team`, `strategy`...

Args : 

| Nom       | Type       | Valeur par défaut                 | Description                                           |
| --------- | ---------- | ---------------------------------- | ----------------------------------------------------- |
| _value_ | int/string | _valeur actuelle de la variable_ | Argument uniquement pour les commandes de type_set_ |

Exemple :

```bash
config set microstep_mode "half"
config get team
```

### Commande spéciale : \[get\] help

Permet de récupérer un menu d'aide contenant la liste des variables de configuration existantes (sur la console UART).

Exemple :

```bash
config get help
``` 


### Destination : **robot**

Types de commandes valides :

- **get** (récupère la valeur de la variable)
- **set** (modifie la valeur de la variable)

#### \[get/set\] 

Permet de gérer les variables d'état (état dynamique) du robot (par exemple x, y, theta...).

Args : 

| Nom       | Type       | Valeur par défaut                 | Description                                           |
| --------- | ---------- | ---------------------------------- | ----------------------------------------------------- |
| _value_ | int/string | _valeur actuelle de la variable_ | Argument uniquement pour les commandes de type_set_ |

Exemple :

```bash
robot get x
robot set y 0
```
