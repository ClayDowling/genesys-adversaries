#!/bin/sh

../build/adversaries -I ../data samplenpcs.adv > actual.txt
diff golden.txt actual.txt