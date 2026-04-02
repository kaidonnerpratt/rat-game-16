cd ./lib/kaireddit
gcc -std="c++20" -I./src ./src/main.cc -lstdc++ -lm -o kaireddit.out 2>log.txt
if [ $(stat -c%s log.txt) -gt 1 ];then
    echo "compileing failure"
    exit 1
fi

./kaireddit