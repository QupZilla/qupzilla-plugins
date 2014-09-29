#!/bin/bash

cd ../plugins

for pluginPro in */*.pro
do
 lupdate $pluginPro -no-obsolete -ts $pluginPro/../translations/empty.ts
done

read -p "Press [ENTER] to close terminal"
exit
