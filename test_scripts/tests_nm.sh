#!/bin/bash

RED="\033[0;31m"
GREEN="\033[0;32m"
BLUE="\e[34m"
NC="\033[0m" # No Color

DIR="tests/"
if [ ! -d ${DIR} ]; then
	mkdir ${DIR}
else
	rm -rf ${DIR}
	mkdir ${DIR}
fi

nbr=0
total=0
pass=0
printf "Test: ${BLUE}/bin/*${NC}\n"
for i in /bin/*; do
	NAME=${DIR}output_$nbr
	if [ -f $i ]; then
		diff <(./ft_nm $i) <(nm $i) > ${NAME}
		if [[ $? == 0 ]]; then
			printf "[%3d] ${GREEN}PASS!  ${NC}: %s\n" $nbr $i;
			pass=$((pass + 1));
			rm -f ${NAME};
		else
			printf "[%3d] ${RED}FAILED!${NC}: %s\n" $nbr $i;
			printf "\n\n$i" >> ${NAME};
		fi
		nbr=$((nbr + 1));
		total=$((total + 1));
	fi
done

printf "Test: ${BLUE}/usr/bin/*${NC}\n"
for i in /usr/bin/*; do
	NAME=${DIR}output_$nbr
	if [ -f $i ]; then
		diff <(./ft_nm $i) <(nm $i) > ${NAME}
		if [[ $? == 0 ]]; then
			printf "[%3d] ${GREEN}PASS!  ${NC}: %s\n" $nbr $i;
			pass=$((pass + 1));
			rm -f ${NAME};
		else
			printf "[%3d] ${RED}FAILED!${NC}: %s\n" $nbr $i;
			printf "\n\n$i" >> ${NAME};
		fi
		nbr=$((nbr + 1));
		total=$((total + 1));
	fi
done

printf "\nTest: ${BLUE}/usr/lib/*${NC}\n"
for i in /usr/lib/*; do
	NAME=${DIR}output_$nbr
	if [ -f $i ]; then
		diff <(./ft_nm $i) <(nm $i) > ${NAME}
		if [[ $? == 0 ]]; then
			printf "[%3d] ${GREEN}PASS!  ${NC}: %s\n" $nbr $i;
			pass=$((pass + 1));
			rm -f ${NAME};
		else
			printf "[%3d] ${RED}FAILED!${NC}: %s\n" $nbr $i;
			printf "\n\n$i" >> ${NAME};
		fi
		nbr=$((nbr + 1));
		total=$((total + 1));
	fi
done

printf "\n\tTotal ${GREEN}PASS${NC}: %d/%d\n" $pass $total;
