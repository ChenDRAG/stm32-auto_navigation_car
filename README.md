# stm32-auto_navigation_car
Code repository for the auto_navigation_car based on stm32. Contributed by the team A_star(champion team of the 20th Tsinghua University Electronic Design Competition)

# EDC20_Team_A_star_controlling_part
  
![Failed to load the image](https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/group_photo.jpg)
  
This is the controlling board source code for the EDC20, THU championship team A_star, mainly implemented by ChenHuayu.
  
This part includes Sensor Data Acquiring, filtering and Fusion, a Four-stage PID controlling strategy, Usart commuciation with the Strategy borad, and bluetooth communication with smartphone and laptop, etc.The controlling board allows the car to be controlled by both computer ,smartphone and the strategyboard, and has 3 different modes(hardware PID debug mode,strategy debug mode and competition mode).The state of the car can be monitered with a RGBled and your smartphone(based on APPs that can be commonly found on the internet, links provided later).

This hardware used is a stm32rct6 board(and another stm32rct6 board for strategy).

## Folder Details
The detailed contents about the files is as follows:  
- /BALANCE contains the impletation of filtering, debug, PID controlling, etc. 
- /HARDWARE contains hardware drivers and basic data processing implementation. 
- /OBJ conatins some Compiled files.
- /STM32F10x_FWLib conatins the offical hardware library of stm32 board,which we use a lot in our project.
- /SYSTEM contains some most basic functions,like delay function or usart communication implementation.
- /USER contain the main logic function.

## Author
Chen Huayu
chenhuay17@mails.tsinghua.edu.cn
18811397006
