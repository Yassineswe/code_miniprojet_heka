# Mini-projet ROS 2 : trajectoire de la tortue

Ce workspace contient trois packages ROS 2 :

- `trajectory_interfaces` : le message partagé `TrajectoryCommand`.
- `brain_commander` : calcule la trajectoire et réagit aux alertes d’obstacle.
- `turtle_executor` : convertit les commandes de trajectoire en messages `Twist` pour turtlesim.

## Compilation

Dans un terminal Ubuntu, sourcez ROS 2 Humble, puis placez-vous dans le workspace — le dossier qui contient directement `src/` :

```bash
source /opt/ros/humble/setup.bash
cd ~/Desktop/code_miniprojet_heka/ros2_ws
colcon build --packages-up-to brain_commander turtle_executor
source install/setup.bash
```

Si le projet se trouve ailleurs, adaptez le chemin de la commande `cd`.

## Lancement

Ouvrez un terminal distinct pour chaque nœud. Dans **chaque terminal**, sourcez ROS 2 et le workspace compilé :

```bash
source /opt/ros/humble/setup.bash
source ~/Desktop/code_miniprojet_heka/ros2_ws/install/setup.bash
```

Démarrez les nœuds dans cet ordre :

1. **Terminal 1 — turtlesim**

   ```bash
   ros2 run turtlesim turtlesim_node
   ```

2. **Terminal 2 — exécuteur Python**

   ```bash
   ros2 run turtle_executor executor_node
   ```

3. **Terminal 3 — observation facultative des commandes**

   ```bash
   ros2 topic echo /trajectory_cmd
   ```

4. **Terminal 4 — commandeur C++**, à démarrer en dernier

   ```bash
   ros2 run brain_commander commander_node
   ```

Le commandeur commence sa trajectoire dès son lancement. Démarrer l’exécuteur avant lui permet de recevoir les premières phases du mouvement.

## Trajectoire attendue

La tortue avance d’abord vers la droite pendant environ 2,75 secondes, tourne sur place pendant environ 1,57 seconde, puis suit un cercle.

## Test d’une alerte d’obstacle

Après le début du mouvement, ouvrez un autre terminal, sourcez ROS 2 et le workspace, puis publiez l’alerte :

```bash
ros2 topic pub /obstacle_alert std_msgs/msg/Bool "{data: true}"
```

Cette commande publie l’alerte de façon répétée. Dès que la tortue commence à reculer, faites **Ctrl+C** dans ce terminal. Elle doit reculer pendant environ une seconde sans tourner, puis s’arrêter. Le commandeur affiche la détection et la fin du recul; l’exécuteur signale l’alerte.