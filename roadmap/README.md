# Placeholders Roadmap

### Phase 1 - Complete

Placeholders (PHL) is a Proof of Work coin built on the Bitcoin UTXO model. As with other Bitcoin derivatives, PHL coins are distributed to persons augmenting the Placeholder network by mining Placeholders.
*  1/2 coin distribution (10.5 Million Total)
*  10x faster blocks (1 per minute)
*  Much more aggressive supply curve reduction.
*  Dark Gravity Wave difficulty adjustment (180 block average)
*  Addresses start with F... for regular addresses.
*  Network Port: 6607
*  RPC Port: 6608

### Phase 2 - Cloud Services (in progress)

Upload binary files to artifacts.
Artifact files marked as content-type torrent and end with the extension VDI will be used for distributed services.

Cloud services is provided by users of Placeholders.
There are 2 types of users, but any user can make use of both types of users.
These users are:
-Service Providers
-Service Consumers

Service providers provide the hardware and virtual machine container.
Service consumers consume the resources provided by the service providers in exchange for compensation (Placeholder coins).

#### ASIC Resistance

ASIC Resistance - A published commitment to continual attempts at ASIC resistance. If ASICs are created for x16r, then we will, at a specific block number, modify one of the algorithms to add some varients of Equihash or similar efforts to increase the resistance to ASIC miners for Placeh.

#### Cloud Services

Use-Case Implementation Overview:
 

Placeholders’ blockchain can store a link in the blockchain (this is an example of an "artifact") which points at a standard torrent file (copies of a file that are held by various people across the internet, commonly known as "seeders"). 

A torrent is a directory file, which holds information on where to find pieces of another file.  Let’s take an example of a normal file (can be any file like a movie or software): This file can be split up into 20 or more different pieces (the torrent file holds this information).  Not every person needs to hold all 20 pieces as long as when someone wants the file, they download all 20 pieces onto their computer. When the file is requested by a person, the seeders will upload pieces of the file to the requester until the requester has all of the pieces and thus the full file.  This ensures availability and redundancy for the data while reducing risk of losing data.  
 
Most people use torrents for sharing large files (i.e. movies and software).  In Placeholders’ case, the torrent file will be a file similar to Virtual Desktop Infrastructure (VDI).  VDI’s are commonly used for storing an image of an operating system (windows/linux/mac) in a single file.  Meaning, if you have access to this file, you can run software (e.g., VirtualBox or VMware’s workstation) to interact with the virtual operating system.  This would be similar to running a completely different computer, where the software is isolated to that virtual machine. This concept is typically referred to as running a "virtual machine".

Virtual machines are an instance of an operating system inside another operating system. For example, you may be running Windows right now, and if you wanted a virtual Windows pc (to test out software), you could run the VirtualBox software with a VDI image file to create a virtual PC that has no effect on your current Windows environment. Placeh takes this a step further and consolidates resources from different PHL wallet users and combines those resources to create an instance of a virtual private server in the cloud, which is then available to the buyer.

In essence, Placeholders is creating an ecosystem to compete with Amazon Web Services and Google Cloud.  

There are two parts to this ecosystem, suppliers and buyers.
  

Suppliers (i.e. people who offer their computing resources and will be paid for it):

For example, a Placeholders wallet user will be able to rent part of their hard-drive space out to store portions of a VDI file, or rent out part of their CPU for distributed computing, or part of their RAM. These will all have various rates based upon the supply/demand and marketplace prices.  


Buyers (people that want to launch a decentralized virtual server/pc):

Currently, if a person wanted to launch a VPS (virtual private server), they would normally need to sign up and pay for a centralized provider like Amazon Web Services (AWS) or Google Cloud.  These providers have established rates to launch a server, based upon the configuration of the server (number of cpus, amount of hard-drive space, and amount of RAM (memory)).  An hourly rate is computed for the amount of resources desired, and the buyer is billed by the provider for launching and running that server instance. 

#### Metadata

Metadata about the token can be stored in IPFS. Initially this cannot be changed. If there is a demand, the system can be updated to allow updating the metadata by the token issuer.

#### Block Size

Placeh may increase the blocksize from 1 MB to X MB to allow for more on-chain transactions.


