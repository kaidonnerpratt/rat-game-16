cd ./lib/kaireddit
gcc -std="c++20" -I../../src -I./src ./src/main.cc -lstdc++ -lm -o kaireddit 2>log.txt
if [ $(stat -c%s log.txt) -gt 1 ];then
    echo "compileing failure"
    exit 1
fi

./kaireddit