TabManager extension for QupZilla
-------------------------------------------------
This extension adds the ability to manage tabs and windows in QupZilla.

![tbm3](http://i.imgur.com/Gh8bEXo.png)

You will find more information about the configuration and usage of this extension in the [wiki](https://github.com/QupZilla/qupzilla-plugins/wiki/Tab-Manager).

**Known Issues**

* SideBySide doesn't use the last activated window, because MainApplication::getWindow() doesn't return it.

* TabManager doesn't detect changing in pinned tabs, because there is not a related signal in QupZilla code.

**TODOs**

* Modifying `refresh` algorithm for updating changed items and not all items.

* Fixing `Known Issues`
