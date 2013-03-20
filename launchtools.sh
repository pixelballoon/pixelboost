#!/bin/bash

echo "Choose a task"
echo "----------------------"
echo "1. Make Projects"
echo "2. Build Assets"
echo "----------------------"
echo "3. Run Editor"
echo "4. Run GameTool"
echo "----------------------"
echo -n "Please type option and press return: "
read opt
case $opt in
	1) ./pixelboost/makeprojects.sh;;
	2) rake;;
	3) open pixelboost/bin/editor.app --args `pwd`/resources/database/;;
	4) open pixelboost/bin/gametool.app --args --project=`pwd`;;
	*) echo "$opt is an invalid option.";
	 echo "Press [enter] key to continue...";
	 read enterKey;;
esac
