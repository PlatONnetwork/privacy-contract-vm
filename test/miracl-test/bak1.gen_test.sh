#!/bin/bash

function gen()
{

    OPTIMIZED=$1
    optimized=$2
    N=$3
    dst_dir=miracl_${optimized}_${N}

    msg="[OPTMIZE:$*] "
    echo -e "\n"${msg}

    rm -rf ${dst_dir}
    unzip -j -aa -L MIRACL-master.zip -d ${dst_dir} >/dev/null
    cd ${dst_dir}

    sed -i '1,4s/^rm/rm\ -f/' linux64


    if [ $# -gt 1 ]; then

        gcc config.c -oconfig -Wno-format
        gcc mex.c -omex -Wno-format-security

        ./mex ${N} amd64 ${optimized}

        #bash linux64
        cp mirdef.h64 mirdef.h
        sed -i '1,4s/^cp/#cp/' linux64
        sed -i 's/^ar\ rc/gcc -c -m64 -msse2 -O2 '${optimized}'.c\nar\ rc/' linux64
        sed -i 's/^rm\ mr/ar\ r\ miracl.a\ '${optimized}'.o\nrm\ mr/' linux64
        #cat linux64

        echo "#define "${OPTIMIZED}" "${N} >> mirdef.h
        #cat mirdef.h

    fi

    bash linux64


    rm -f test test.cpp
    cp ../test.cpp ./

    g++ -O2 -std=c++11 test.cpp miracl.a -o test

    echo -e ${msg}"\c" >> ../test_report.txt
    time ./test  >> ../test_report.txt
   # echo -e ${msg}"\c" >> ../test_report.txt
   # time ./test 512 256 1024 >> ../test_report.txt
   # echo -e ${msg}"\c" >> ../test_report.txt
   # time ./test 1024 1024 102400 >> ../test_report.txt

    cd ..
}

echo "" > test_report.txt

gen
gen MR_COMBA mrcomba 4
gen MR_KCM mrkcm 4
gen MR_COMBA mrcomba 8
gen MR_KCM mrkcm 8
gen MR_COMBA mrcomba 16
gen MR_KCM mrkcm 16

cat ./test_report.txt

exit 0


