Data Collector setting SD-Card

This sd card provide 2 function

1. Set up Data Collector environment in Novakon P-Series machine.
2. Copy data and firmware in to Novakon P-Series machine

** IMPORTANT : You MUST set up environment first.

Set up environment
===================
1. plug in SD Card into machine
2. In to SD Card path (should be /run/media/mmcblk0p1 but you need check this)
3. Run scrip >>> $ ./makeenv.sh {sd path}
    ex: $ ./makeenv.sh /run/media/mmcblk0p1 
4. reboot let environment active.


Update data or firmware
==========================
When you done set up environment step and reboot machine. Just plug in SD Card in to machine again.
After plugin SD card 3 secs, It should be done. (Depend on sie of data)

Just put file into this folder then plug in sdcard

/data
    -- loginlist.csv : CSV file contain login information .. ( user name , password )
    -- department-{yyyyMMdd} : Department list, every single line is one department name.
/image
    -- put all item image file here.
/update
    -- put new firmware here (RT)