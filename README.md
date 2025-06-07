# Dual-Ultrasonic-sensor-based-Obstacle-detection-and-avoidance-robot

## 🔌 Hardware List
- ESP32 Dev Board
- 2x HC-SR04 Ultrasonic Sensors
- L298N Motor Driver
- 4x 100rpm DC motors
- Servo Motor (SG90 or equivalent)
- IC7805 voltage regulator (mandatory for voltage supply to sensors from voltage source)
- Robot chassis and wheels
- Proper BMS equiped chargable Battery pack and adapter

---

🔄 **Algorithm for ESP32-Based Obstacle Avoidance Robot**

**1. Initialization**

* Begin serial communication.
* Configure GPIO pins for ultrasonic sensors, motor drivers, and servo motor.
* Attach servo motor and set it to the default forward-facing angle.
* Set the robot’s movement state to "moving forward".


**2. Main Loop Execution**

**A. If the robot is moving forward:**

1. **Measure front distance** using the forward-facing ultrasonic sensor.
2. **If obstacle is detected** (distance < 30 cm):

   * Stop the robot.
   * Rotate the front sensor to the right.
   * Read distance using the **rear sensor** (assumed facing right due to servo rotation).
   * Read distance again using **front sensor** (assumed now facing left).
   * Compare left and right distances:

     * If **left is clear** and **right is blocked**, turn left.
     * If **right is clear** and **left is blocked**, turn right.
     * If **both sides are clear**, turn toward the side with **more space**.
     * If **both sides are blocked**, move backward.
   * After decision, stop, reset servo to center, and resume moving forward.
3. **If no obstacle**, continue moving forward.


**B. If the robot is moving backward:**

1. **Measure back distance** using the rear ultrasonic sensor.
2. **If obstacle is detected** behind (distance < 30 cm):

   * Stop the robot.
   * Rotate front sensor to right.
   * Read distances from both sensors.
   * Compare:

     * If **left is clear** and **right is blocked**, turn left.
     * If **right is clear** and **left is blocked**, turn right.
     * If **both sides are clear**, turn toward the side with **more distance**.
     * If **both sides are blocked**, stop and move forward.
   * After decision, stop, reset servo to center, and resume moving backward.
3. **If no obstacle**, continue moving backward.


**3. Loop Continuation**

* Add a small delay at the end of each cycle.
* Repeat the entire loop continuously to maintain obstacle awareness.


## 🔚 **End of Algorithm**

---


For code and circuit diagram should to be referred. And it is better if we have a prior and good knowledge about all the components going into the model like pin descriptions of ESP-32 and driver circuits, current ratings, etc. It is also important to choose all the components carefully according to requirments.

the major problem we faced:-
* ESP-32 needs proper 5v supply orelse the entire robot funtioning will be affected.
* proper BMS attached chargable battery pack is preffered since motors can drain the battery fastly and its cost effective also.
* the 15 degree coverage of ultrasonic sensor needs to be taken care off.


---

There is a lot of scope on the robot and several attachment and modifications can be made in the code as well as to the model to bring more productivity out of it (refer problem statements in the ppt attached)

---

