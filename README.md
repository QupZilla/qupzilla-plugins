QupZilla Web Browser
----------------------------------------------------------------------------------------

Homepage: [http://www.qupzilla.com](http://www.qupzilla.com)   
Blog: [http://blog.qupzilla.com](http://blog.qupzilla.com)   
IRC: `#qupzilla` at `irc.freenode.net`

Building
----------------------------------------------------------------------------------------
The easiest way to build external plugins is to copy plugin directories into QupZilla's
source tree (eg. into `src/plugins`).  

You can also build plugins out of the main source tree. In order to do that, you need to
set two environment variables, namely `$QUPZILLA_PLUGINS_SRCDIR and $QUPZILLA_SRCDIR`,
both pointing to the correct directory.  

    echo $QUPZILLA_PLUGINS_SRCDIR $QUPZILLA_SRCDIR
    
    /home/david/Programming/Qt-C++/QupZilla-plugins
    /home/david/Programming/Qt-C++/QupZilla
