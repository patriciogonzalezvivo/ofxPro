# Take remotes pictures with your RaspberryCam

## 1. Setting up your Raspberry

### 1.a Installing everything you need

	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install avahi-daemon apache2 php5


### 1.b Enable camera

Access the configuration settings for the Pi by running the following command:

	sudo raspi-config

Navigate to “camera” and select “enable”.

![enable_cam](http://www.raspberrypi.org/wp-content/uploads/2013/05/image-2.jpg)
 
Camera device need 777 priviladge **( WARNING: this is dangerous )**
 
	sudo chmod 777 /dev/vchiq

### 1.c Copy the PHP code in his own special folder

Go to apache folder and make a folder to copy the PHP code that is going to take and deliver the photo

	cd /var/www
	sudo mkdir rpicam

The folder where is store the images needs all (777) priviladges **( WARNING: this is dangerous )**

	chmod 777 rpicam