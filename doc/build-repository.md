## Instructions to Build Repository

```sh
sudo add-apt-repository ppa:bitcoin/bitcoin
sudo apt-get update

sudo apt-get install libboost-all-dev
sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev

sudo apt-get install libdb4.8-dev libdb4.8++-dev
sudo apt-get install libminiupnpc-dev
sudo apt-get install libzmq3-dev

wget https://www.placeh.io/files/Placeh_2.0.30.5_Ubuntu18.tar.gz
gunzip Placeh_2.0.30.5_Ubuntu18.tar.gz
tar xvf Placeh_2.0.30.5_Ubuntu18.tar
```
## Run Placeholders Node in the background
```sh
cd Placeh_2.0.30.5_Ubuntu18/
chmod +x *
./placehd -server -daemon -listen=1

```

## Instructions to build Aria2c from source
```sh
wget https://github.com/aria2/aria2/releases/download/release-1.35.0/aria2-1.35.0.tar.gz
gunzip aria2-1.35.0.tar.gz
tar xvf aria2-1.35.0.tar
apt-get install nettle-dev libgmp-dev libssh2-1-dev libc-ares-dev libxml2-dev  zlib1g-dev libsqlite3-dev pkg-config libgpg-error-dev libgcrypt-dev libssl-dev
cd aria2-1.35.0
./configure
make
```

## Optionally
For aria2c you can also just use
```sh
apt install aria2
```

## Open port for incoming / outgoing traffic 8080

```sh
ufw enable
ufw allow 8080
```

## Create your repository directory

```sh
cd /root/
mkdir repository
```

Note: If you wish to use a different directory, you will need to alter the server.properties file in the tomcat/bin directory installation.

## Grab and install the packaged tomcat=

```sh
wget https://www.placeh.io/files/apache-tomcat-8.5.51.tar.gz
gunzip apache-tomcat-8.5.51.tar.gz
tar xvf apache-tomcat-8.5.51.tar
```

## Make sure you have Java installed=
```sh
sudo apt update
sudo apt install openjdk-8-jdk
```

## Run Repository

```sh
cd apache-tomcat-8.5.51\bin
sh startup.sh
```

## Test your installation
You can be sure of your external ip address using this command
```sh
curl ifconfig.me
```

## Open a browser, and visit:
http://my.ip.address:8080/raw/FGx2aebnUA64fnpWLouonQsu6E6oy4gMga

*You should see an image of the word "peanut butter"

## Trouble Shooting

Sometimes you may need to open the port via iptables
You can do this with the following command:

```sh
iptables -I INPUT 1 -i eth0 -p tcp --dport 8080 -j ACCEPT
```

If you run into issues installing it, please contact xagau on our discord for further instructions. If you find issues with this instruction guide
please feel free to modify it and submit a pull request. 

Special thank to CarlH and others for helping to put this together.
