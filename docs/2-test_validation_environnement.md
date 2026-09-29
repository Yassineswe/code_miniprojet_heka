# Test de validation de votre environnement ROS2

Ce document sert à vérifier que votre environnement est correctement installé et prêt pour le mini-projet. Suivez les étapes dans l'ordre — si une étape échoue, ne continuez pas, contactez-nous avec le message d'erreur exact.

---

## Étape 0 — Le conteneur tourne

```bash
docker ps
```
✅ Vous devez voir `ros2_humble_desktop` avec le statut `Up`.

Si rien n'apparaît :
```bash
cd ROS2_env
docker compose up -d
```

---

## Étape 1 — Accéder au bureau virtuel

Ouvrez votre navigateur à l'adresse : **http://localhost:6080**

✅ Un bureau Ubuntu doit s'afficher.
Si vous avez des difficultés à faire CTRL-C, CTRL-V : vérifiez les paneaux qui s'ouvre à gauche du bureau (petite fleche au milieu à gauche)

---

## Étape 2 — Compiler le package

```bash
cd ~/ros2_ws
colcon build --packages-select env_check
```
✅ Le message final doit contenir `Summary: 1 package finished`, sans ligne commençant par `Error`.

Puis chargez l'environnement :
```bash
source install/setup.bash
```

---

## Étape 3 — Lancer turtlesim

Dans **ce même terminal** (ou un nouveau, mais il faudra re-sourcer ROS2 avec `source /opt/ros/humble/setup.bash`) :

```bash
ros2 run turtlesim turtlesim_node
```
✅ Une fenêtre avec une tortue sur fond bleu doit s'ouvrir.

**Laissez cette fenêtre/ce terminal ouvert.**

---

## Étape 4 — Lancer le node de test

Ouvrez un **nouveau terminal** dans le bureau virtuel :

```bash
source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 run env_check circle_mover
```

✅ La tortue doit se mettre à **tourner en cercle** dans la fenêtre turtlesim.
✅ Le terminal doit afficher : `circle_mover demarre : la tortue devrait tourner en cercle.`

Arrêtez avec `Ctrl+C`.

---

## Étape 5 — Vérifier la communication entre nodes

Pendant que `circle_mover` tourne (relancez-le si besoin), ouvrez un **troisième terminal** :

```bash
source /opt/ros/humble/setup.bash
ros2 topic list
```
✅ Vous devez voir `/turtle1/cmd_vel` dans la liste.

```bash
ros2 topic echo /turtle1/cmd_vel
```
✅ Des messages doivent défiler en continu (`linear:`, `angular:`, etc.). `Ctrl+C` pour arrêter.

---

## Étape 6 — Vérifier Git

```bash
git --version
git config --global user.name "Votre Nom"
git config --global user.email "votre@email.com"
```
✅ Aucune erreur. Ces deux dernières commandes n'affichent rien si elles réussissent — c'est normal.

---

## Récapitulatif — checklist à cocher

- [-] Le conteneur tourne (`docker ps`)
- [-] Le bureau virtuel s'affiche dans le navigateur
- [-] `colcon build` compile sans erreur
- [-] `turtlesim_node` s'ouvre
- [-] `circle_mover` fait tourner la tortue en cercle
- [-] `/turtle1/cmd_vel` apparaît dans `ros2 topic list` et publie des messages
- [-] Git est configuré (nom + email)

**Si vous avez un problème avec cette étape contactez moi directement, je vous aiderais.**
