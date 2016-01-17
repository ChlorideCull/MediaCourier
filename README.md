#Media Courier
Media Courier is a streaming server designed by Media Courier UF, a company in the [Junior Achievement Sweden](http://www.ungforetagsamhet.se/om-oss/about-junior-achievement-sweden) program, for use in the period between 12-2015 and 5-2016.

##Advisory
**Media Courier is still under heavy active development, and is not recommended for production use yet. Files and packages mentioned in this or other documents may actually not exist yet.**

##Components
The Media Courier suite is split into three programs, maintaining different roles:

* `MC-TransmissionServer` - Obsolete server that handles incoming RTMP streams, media decoding and encoding, and other media-related work. Has been replaced by `MC-ChangelingShim`.
* `MC-ChangelingShim` - New edge server which utilizes [nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module/) for the heavy work. It essentially only handles authentication and minor features.
* `MC-HiveServer` - Server that handles user authentication, and transmission server management.
* `MC-WebUI` - Server that serves a Web UI for public streams and end-user configuration.

You can have multiple transmission servers and Web UI servers, but only one hive.

##Building
*Media Courier currently only supports operating systems with LD_PRELOAD support (Linux, OSX).*

###Arch Linux
Arch Linux users can run `makepkg` in this folder to create all three packages automagically.

###Other Distributions
Other distributions can use `makepkg-shim.sh`, a bash shim that exploits the fact that PKGBUILD files are valid bash scripts.

Run `./makepkg-shim.sh <output folder>` to create a folder containing the compiled software for packaging, along with a sourceable `pkginfo.sh` file that exports the package metadata.

