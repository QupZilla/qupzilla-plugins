TabManager plugin for QupZilla
-------------------------------------------------
**Known Issues**

* TabManagerWidget::closeSelectedTabs() doesn't respect to "m_dontQuitWithOneTab" option.

* SideBySide doesn't use the last activated window, because MainApplication::getWindow() doesn't return it.

* Group By Domain doesn't support multi-part TLDs.

* TabManager doesn't detect changing in pinned tabs, because there is not a related signal in QupZilla code.

**TODOs**

* Using a more accurate domain name detection. see [public suffix list] (http://mxr.mozilla.org/mozilla-central/source/netwerk/dns/effective_tld_names.dat?raw=1)

* Modifying `refresh` algorithm for updating changed items and not all items.

* Fixing `Known Issues`
