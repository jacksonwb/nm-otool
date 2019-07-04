RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
let "COUNT=0"
let "TOTAL=0"
for i in /usr/lib/*.a; do
	if [[ -d $i ]]; then
		continue 
	fi;
	otool -t $i > real_otool
	./ft_otool $i > my_otool
	diff my_otool real_otool > res
	let "TOTAL += 1"
	if [[ -s res ]]; then echo "${RED}Error:${NC} " $i ; else echo "${GREEN}Valid.${NC}" && let "COUNT += 1";fi;
done
rm -rf res real_otool my_otool
echo "PASSED " $COUNT " / " $TOTAL