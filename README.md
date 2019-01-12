# stm32-auto_navigation_car
Code repository for the auto_navigation_car based on stm32. Contributed by the team A_star(champion team of the 20th Tsinghua University Electronic Design Competition)  

Basically our car can navigate itself arcoss a virtual city and obeying some "traffic rules". Given multiple starting points and ending points and their corresponding awards at the same time, the car can calculate the 'best route'. Our design can do this task with great effiency and speed, and has distinguished itself from other 95 teams from tsinghua university by winning the championship. 

![Failed to load the image](https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/group_photo.jpg)

map of the city is like:
![Failed to load the image](https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/map.jpg)

For the detailed rule of the EDC20 game, See :  
https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/EDC20_rules(Chinese).pdf

For the chinese report of our team, see:   
https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/EDC20_A_star_report_Chinese_version.pdf

# EDC20_Team_A_star_controlling_part
    
This is the controlling board source code for the EDC20, THU championship team A_star, mainly implemented by ChenHuayu.For the Strategy part please see:(To be updated)
  
This part includes Sensor Data Acquiring, filtering and Fusion, a Four-stage PID controlling strategy, Usart commuciation with the Strategy borad, and bluetooth communication with smartphone and laptop, etc.The controlling board allows the car to be controlled by both computer ,smartphone and the strategyboard, and has 3 different modes(hardware PID debug mode,strategy debug mode and competition mode).The state of the car can be monitered with a RGBled and your smartphone(based on APPs that can be commonly found on the internet, links provided later).

The hardware used is a stm32rct6 board(and another stm32rct6 board for strategy).

![Failed to load the image](https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/car2.jpg)


## Folder Details
The detailed contents about the files is as follows:  
- /BALANCE         contains the impletation of filtering, debug, PID controlling, etc. 
- /HARDWARE        contains hardware drivers and basic data processing implementation. 
- /OBJ             conatins some Compiled files.
- /STM32F10x_FWLib conatins the offical hardware library of stm32 board,which we use a lot in our project.
- /SYSTEM          contains some most basic functions,like delay function or usart communication implementation.
- /USER            contain the main logic function.

## Notice
The structure of the code is based on the miniBalance project with an open_source permission, and our 
Communication Coding makes it easy for the car to communicate with the APP 'MiniBalance', which can be 
downloaded here:   
https://cloud.tsinghua.edu.cn/d/2239dcf7c8164cbfb2b0/.

## Author
Chen Huayu  
chenhuay17@mails.tsinghua.edu.cn  
18811397006  

## Team members
- Chen Huayu (Captain)
- Liang Wenhao
- Urkax
- Wu Ziyi

For brief introduction of the team, see:  
https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/edc20_A_star_members.pdf

![Failed to load the image](https://github.com/ChenDRAG/stm32-auto_navigation_car/blob/master/logo.jpg)


