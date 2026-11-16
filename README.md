# FPSZombies

## Call Of Duty Zombies style game made in Unreal Engine 5

* Player movement with Enhanced Input System (Input actions & Mapping Context)
* Player animations using Blend Spaces (Walking & Running) and Aim Offset animations
* Shooting functionality using Line Trace and includes Muzzle flash, sound and shake effect
* Reload animation and sound

### Zombie AI System:
* Chase player using nav mesh (AIMoveTo function)
* Attack the player when it's in reach (less than 1 meter) (Functionality & Animation)
* Reload attack to meet the attack rate specified

### Money System
* Player recieves money (points) for shooting the zombies
* Each bullet hit gives 25$ to the player
* Use the Money to open Doors and Escape

## Houston driving prototype

The default startup map now uses an arcade-driving prototype with a procedural placeholder city and a simple primitive car. No Houston RealityScan reconstruction or vehicle model is included; see `Content/HoustonRealityScan/README.md` for the import location and workflow.

### Controls
* W / S: accelerate / reverse
* A / D: steer
* Space: brake
