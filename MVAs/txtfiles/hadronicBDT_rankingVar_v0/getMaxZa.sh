#file=$1

#fixedTime=$(date -r getMaxZa.sh +%s)
#echo $fixedTime
baseZa=`awk -v max=0 '{if($3>max){max=$3}}END{print max} ' testBaseBDT.txt `
echo "base BDT "$baseZa

for txtfile in rank*txt
do
#    this_time=$(date -r "$txtfile" +%s)
#    echo $txtfile, $this_time
#    if [ "$this_time" -gt "$fixedTime" ]; then
#	echo "test"
       maxZa=`awk -v max=0 '{if($3>max){max=$3}}END{print max} ' $txtfile`
       echo $txtfile' '$maxZa
#    fi
done

