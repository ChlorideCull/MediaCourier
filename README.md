#Media Courier
Media Courier is a streaming server designed by Media Courier UF, a company in the [Junior Achievement Sweden](http://www.ungforetagsamhet.se/om-oss/about-junior-achievement-sweden) program, for use in the period between 12-2015 and 5-2016. It is licensed under the AGPL, version 3.

##Advisory
**Media Courier is still under heavy active development, and is not recommended for production use yet. Files and packages mentioned in this or other documents may actually not exist yet.**

##Components
The Media Courier suite is split into three programs, maintaining different roles:

* `MC-CSG2` - [CSG2](https://github.com/ChlorideCull/CSG2) powered site, which also handles authentication. RTMP streaming is provided by [nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module/).
* `MC-StreamPlexer` - Program started by nginx-rtmp-module, handles stream forwarding.

##Building
*Media Courier currently only supports POSIX compliant systems, such as Linux and OS X.*

    cmake ..
    make
    make install

