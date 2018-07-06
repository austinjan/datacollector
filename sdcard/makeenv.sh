#! /bin/sh
PWD=$1
echo $PWD
# qt librarys
cd /usr
mkdir /usr/local
mkdir /usr/local/qt4.8
cp -r /opt/qt_arm48_p/* /usr/local/qt4.8
echo "copy qt library done"

# keyboard drivers
mkdir /opt/qt_arm48_p/plugins/kbddrivers
cp $1/libqlinuxinputkbddriver.so /opt/qt_arm48_p/plugins/kbddrivers/
echo "copy keyboard driver done"

# auto download files
cp $1/sdcard.rules /etc/udev/rules.d/
cp $1/sdcard-autorun  /home
chmod +x /home/sdcard-autorun
echo "copy udev script done"

cp $1/libiw.so.29  /libiw
echo "copy libiw"

mv /etc/wpa_supplicant.conf /etc/wpa_supplicant.conf.bak
cp $1/wpa_supplicant.conf /etc

