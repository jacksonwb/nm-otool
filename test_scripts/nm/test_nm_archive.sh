#!bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
let "COUNT=0"
let "TOTAL=0"
for i in /usr/lib/*.a; do
	nm $i > real_nm
	./ft_nm $i > my_nm
	diff my_nm real_nm > res
	let "TOTAL += 1"
	if [[ -s res ]]; then echo "${RED}Error:${NC} " $i ; else echo "${GREEN}Valid.${NC}" && let "COUNT += 1";fi;
done
rm -rf res real_nm my_nm
echo "PASSED " $COUNT " / " $TOTAL