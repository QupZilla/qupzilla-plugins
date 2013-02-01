#!/bin/bash

cd ../

for pluginPro in */*.pro
do
 lupdate $pluginPro -no-obsolete -ts $pluginPro/../translations/empty.ts
done

read -p "Press [ENTER] to close terminal"
exit
