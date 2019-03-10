# soccer-bot
Robot that can play soccer. Autonomous and remote control modes.

Autonomous mode uses the Pixy camera on the soccerbot to identify the location of an orange soccer ball (image recognition is based on a shape and size of a given calibrated color). Once the soccer ball is located the robot will navigate to it.

Remote control mode uses a bluetooth connected arduino on a custom built remote that uses an accelerometer to control the soccerbot with tilt control.

There is also a third, tabletop mode for the robot used for presentation purposes where the robot only autonomously turns in place to face the orange ball.

Code files:
SoccerBotBrain: 1st Arduino on the soccerbot that computes the directions from either mode.
SoccerBotMuscle: 2nd Arduino on the soccerbot that excecutes the directions given by the brain.
BluetoothRemote: Program for the Arduino on the remote.
SoccerBotBrainTableMode: A different program that can be uploaded to the brain to run the 3rd mode.


NOTE:
Pictures and more in depth descriptions will be uploaded shortly. (right now just uploading all the code for each project)
