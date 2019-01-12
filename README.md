# stm32-auto_navigation_car
Code repository for the auto_navigation_car based on stm32. Contributed by the team A_star(champion team of the 20th Tsinghua University Electronic Design Competition)

# EDC20_Team_A_star_controlling_part
  
![Failed to load the image](https://github.com/Wuziyi616/EDC20_Team_A_star_strategy_part/blob/master/group_photo.jpg)
  
This is the strategy board source code for the EDC20, THU championship team A_star, mainly implemented by Ziyi Wu and Urkax. See the movement board code in [EDC20_Team_A_star_movement_part]() (to be updated)  
  
This part includes receiving the information from the host computer, making decisions about path-planning, communicating between two boards and also wiFi-control bluetooth output, etc.  

## Details
The detailed contents about the files is as follows:  
- wifiControl.h/c and Decode.h/c is about receiving messages from the host computer and decode them to get the correct information about scores, positions, etc.  
- Output.h/c is the implementation of bluetooth communication.  
- Core.h/c is mainly the decision-making part about which passenger to pick up and the nearest path to do so.  
- urkax.h/c and main.h/c is actually the real core of our strategy board (not Core.h/c hhh), which decides when to call all the functions to do path-search and send the movement board the next position to go to.  

## Implementation of Path Searching
Path search may be the most important issue in this competition, since the map is very complex and the rules are very strict.  
![Failed to load the image](https://github.com/Wuziyi616/EDC20_Team_A_star_strategy_part/blob/master/map.jpg)
After discussing with the teammate who takes in charge of movement, I decide to divide the map into several levels of units, and the path I give will be a sequence of points (coordinates).  
  
Concretely, I assign 40 Positions, 26 Edges, 12 Sections and 4 InterSections.  
- The struct Position is some key points in the map. Coordinate is measured from the CAD file.
- The struct Edge contains two Positions: start_pos and end_pos. And it also has an in_Edge bool function and length.
- The struct Section contains an array of Edges and two InterSections: start_is and end_is.
- The struct InterSection contains an in_InterSection function.
  
In map initialization, I pre-calculate some nearest paths between all the InterSections. Therefore, when I want to get the nearest path between two positions, I just find out the nearest InterSections of each and use the pre-calculated paths as the backbone of the path. If the backbone is determined, then the only thing I need to do is adding some transitional positions before and after the backbone.  
  
The kernel functions in Core.h/c are get_nearest_path and Search_Passenger. The first one is implemented as I've mentioned above. In the second function, I loop through all the existing passengers and compare their (score / (path_to_passenger.length + start_to_end.length)), which I call distance-average point. Then I just return the max point passenger to pick up.  
  
Other functions include avoid_circle, cross_dash, etc, which are used for path optimization.  

## Structure of Decision Making
The entire structure of strategy taking is also very significant. Since the structure is done by Urkax rather than me, here I'll just draw a rough outline of it.  
  
Every time a message is received from the host computer, we'll decode it and judge whether the passenger state has changed. If not, then go on following the current path planned by Core.h/c. Otherwise, if there's already a passenger on our car, we'll calculate the nearest path from current position to the destination of the passenger. If not, then the function Search_Passenger will be called to determine the next passenger to pick up.  
  
Other part including avoid enemy car or so can also be seen in the code.  

## EDCHost20 for Debug
This is the modified EDCHost20 for debugging the path planning and decision making. Instead of using the image from outside camera, I just use a map.jpg file as the central image.  
  
  
![Failed to load the image](https://github.com/Wuziyi616/EDC20_Team_A_star_strategy_part/blob/master/EDCHost20_wzy.png)  
  
The usage of it is exactly the same as the authentic EDCHost20. After connecting with a wifi which can be not only EDC20 but anything like a telephone hotspot, you also need to calibrate the map. Note that the order should be up-right, up-left, down-right, down-left, which is different from the original EDCHost20.  
  
Now you can start your simulation. There are lots of funtions which you can easily understand from the instructions in the UI. A very vital action is that you can click the right mouse on the map to move your car there! You can even change your car ID or change the passenger's start/end positions. It really helps our debug process a lot.  

## Author
Ziyi Wu  
wuzy17@mails.tsinghua.edu.cn  
Urkax  
1751772593@qq.com  
