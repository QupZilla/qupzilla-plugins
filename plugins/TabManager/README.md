TabManager plugin for QupZilla
-------------------------------------------------
This plugin adds the ability to manage tabs and windows in QupZilla.

The TabManager-plugin has to be properly installed and enabled under `Preferences -> Extensions -> Application Extensions`

![tbm1](http://i.imgur.com/Qle6PYa.png)

To configure the plugin please select it and press `Settings`.

![tbm2](http://i.imgur.com/fF81MSX.png)

You will have to choose between two configuration options:

(1) SideBar: Will display the TabManager in the sidebar like the history or the bookmarks.
(2) Window: Will open the TabManager in a separate window. This mode is intended to use for managing a lot of windows or tabs.

![tbm3](http://i.imgur.com/uvVqEAi.png)

The next image shows the TabManager in the sidebar-mode:

![tbm4](http://i.imgur.com/KmOKs0k.png)

With the TabManager you will be able to group the tabs by

- Window
- Domain
- Host

and to perform actions like

- Detach checked tabs
- Bookmark checked tabs
- Close checked tabs

![tbm5](http://i.imgur.com/j3E5z9t.png)

The TabManager is a powerful plugin to manage tabs and/ or windows.

**Known Issues**

* SideBySide doesn't use the last activated window, because MainApplication::getWindow() doesn't return it.

* Group By Domain doesn't support multi-part TLDs.

* TabManager doesn't detect changing in pinned tabs, because there is not a related signal in QupZilla code.

**TODOs**

* Using a more accurate domain name detection. see [public suffix list] (http://mxr.mozilla.org/mozilla-central/source/netwerk/dns/effective_tld_names.dat?raw=1)

* Modifying `refresh` algorithm for updating changed items and not all items.

* Fixing `Known Issues`
