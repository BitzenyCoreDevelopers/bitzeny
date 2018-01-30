#!/usr/bin/zsh
# Copyright The 2017 Bitzeny Core Developer
#
# Usage
#
#   $ ./PubkeyHash2base58check.sh <PubkeyHash> <AddressPrefix(hex num)>
#
#	example
#
#   $ ./PubkeyHash2base58check.sh a4051c02398868af83f28f083208fae99a769263 51
#
#
# For your information: https://qiita.com/onokatio/items/d471a11e9894d01624df

set -e
export BC_LINE_LENGTH=999

length1=$(echo -n $1|wc -c)
length2=$(echo -n $2|wc -c)
if [[ "$length1" -ne 40 ]] ;then
	echo "PubkeyHash is not 40 byte." && exit
fi
if [[ "$length2" -ne 2 ]] ;then
	echo "AddresPrefix is not 2 byte." && exit
fi

p3=$1

p4=$(echo $2$p3)

p5=$(echo $p4|xxd -ps -r|sha256sum|cut -c-64)

p6=$(echo $p5|xxd -ps -r|sha256sum|cut -c-64)

p7=$(echo $p6|cut -c-8)

p8=$(echo $p4$p7)

p91=$(echo "obase=10; ibase=16; $(echo $p8|tr 'abcdef' 'ABCDEF')"|bc)

ans=$p91 && p92=( $(while [[ "$ans" != "0" ]];do r=$(echo "$ans%58"|bc);ans=$(echo "$ans/58"|bc);echo $r;done|tac|tr '\n' ' ') )

i=0 && base58=(1 2 3 4 5 6 7 8 9 A B C D E F G H J K L M N P Q R S T U V W X Y Z a b c d e f g h i j k m n o p q r s t u v w x y z) && p93=$(for i in $p92;do echo $base58[(($i+1))];done|tr -d '\n')

num=$(( ($(echo $p8|sed -e 's/\(0*\).*/\1/'|wc -c)-1)  / 2  )) && p94=$(echo $(yes 1|head -n$num|tr -d '\n')$p93)

echo $p94
