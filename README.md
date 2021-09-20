# Door Lock System
## The system has 2 AVR microcontrollers communicate together using UART and an EEPROM to store passwords. EEPROM communicates with the controller using I2C protocol.

#### For the first time you run it, you will be asked to set a password by entering it twice, thet must be matched at two times if no you will be asked to set the password again. For the times after that you will choose 1 task from 
1. Change the password
2. 2. Open the door.

#### 1. Change the password:
To change the it, you will be asked about the current password, if you entered the password correctly, you will be able to change the password.
#### 2. Open the door:
You would be asked about the password, when you write it correctly, the door will open (DC motor will rotate in clock wise direction for 15 seconds), hold for 3 seconds then close the door (DC motor will rotate in counter clock wise direction for 15 seconds).

#### Note That:
if you entered wrong password for 3 times in a row in any task, there is a buzzer will be activated for 1 minute.
