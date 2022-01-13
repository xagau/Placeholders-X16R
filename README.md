Placeholder Core integration/staging tree
=====================================

https://www.placeh.io
 
What is Placeholders?
----------------

Placeholders is an experimental digital currency and cloud storage platform that enables instant payments to anyone, and virtual machine procurement anywhere in the world. Placeholders uses peer-to-peer technology to operate with no central authority: managing transactions and issuing money and virtual machines are carried out collectively by the network. 

License
-------

Placeholder Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Placeholders Core contains Patent Pending technology.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/xagau/Placeholders-X16R/tags) are created
regularly to indicate new official, stable release versions of Placeholder Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Developer Discord can be found on Discord at #placeholders.
https://discord.gg/qMkfbxG

Testing
-------
Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of placeholders.

Testnet is now up and running and available to use during development. There is an issue when connecting to the testnet that requires the use of the -maxtipage parameter in order to connect to the test network initially. 

After the initial launch the -maxtipage parameter is not required.

Use this command to initially start placehd on the testnet. <code>./placehd -testnet -maxtipage=259200</code>

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`


### Windows Dependencies

For Tapestry products, you are required to use the following console programs:
-mktorrent.exe
-aria2c.exe
-cygcrypto-1.0.0.dll
-cygwin1.dll
-cygz.dll
-prefix.exe

All of these projects are open source and can be found as forked sub-projects in the xagau workspace.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.


About Placeholders
----------------
A digital peer to peer network for the facilitation of decentralized compute platform transfer and operation.

Placeholders are typically used as a place to put something until you decide what is required.

Thank you to the Bitcoin developers. 
Thank you to the Raven developers. 

The Placeholders project is launched based on the hard work and continuous effort of over 400 Bitcoin developers who made over 14,000 commits over the life to date of the Bitcoin project. We are eternally grateful to you for your efforts and diligence in making a secure network and for their support of free and open source software development.  The Placeholders experiment is made on the foundation you built.


Abstract
----------------
Placeholders aims to implement a blockchain which is optimized specifically for the use case of transferring any electronic data, including entire Virtual Machines from one holder to another. Based on the extensive development and testing of Bitcoin, Placeholders is built on a fork of the Raven code. Key changes include decline curve for coin emission, smaller block reward (10.5M vs 21B). Placeholders is free and open source and will be issued and mined transparently with no pre-mine, developer allocation or any other similar set aside. Placeholders is intended to prioritize user control, privacy and censorship resistance and be jurisdiction agnostic while allowing simple optional additional features for users based on need.

To influence the direction or contribute to the project it is recommended to join our discord group and become active.

Placeholders is designed to be a use case specific blockchain designed to efficiently handle one specific function: a decentralized cloud platform.

Bitcoin is and always should be focused on its goals of being a better form of money. Bitcoin developers will unlikely prioritize improvements or features which are specifically beneficial to the facilitation of token transfers.  One goal of the Placeholders project is to see if a use case specific blockchain and development effort can create code which can add advantages for specific use cases.

In the new global economy, borders and jurisdictions will be less relevant as more assets are tradable and trade across borders is increasingly frictionless. In an age where people can move significant amounts of wealth instantly using Bitcoin, global consumers will likely demand the same efficiency for their securities and similar asset holdings. Computer scientists, political writers and the like will demand to have an affordable, censorship resistant way to publish content.

Placeholders contains [Patent Pending] technology.

